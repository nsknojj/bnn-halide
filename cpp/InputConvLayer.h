#ifndef INPUT_CONV_LAYER_H
#define INPUT_CONV_LAYER_H

#include <Halide.h>
#include <cstddef>
#include "SArray.h"
#include "Typedefs.h"

using namespace Halide;

template<int M, int N, int S, int K, int B_S>
class InputConvLayer{
public:
    SArray<float, M*N*K*K> *w;
    SArray<float, N> *k;
    SArray<float, N> *h;

    InputConvLayer() {
        static_assert(K==3, "Conv layer only works for 3x3 convolutions!\n");
        static_assert(M==3, "Input conv layer assumes 3 input channels!\n");
        w = new SArray<float, M*N*K*K>;
        k = new SArray<float, N>;
        h = new SArray<float, N>;
    }

    ~InputConvLayer() {
        delete w;
        delete k;
        delete h;
    }
    
    Func get_output(ImageParam &input_param, std::vector<Argument> &args) {
        Var i("i"), j("j"), x("x"), y("y");
        
        ImageParam kernel_param(Float(32), 4);
        ImageParam k_param(Float(32), 1);
        ImageParam h_param(Float(32), 1);
        kernel_param.set(Buffer(Float(32), K, K, M, N, (uint8_t*)w->ptr(), "kernel"));
        k_param.set(Buffer(Float(32), N, 0, 0, 0, (uint8_t*)k->ptr(), "kk"));
        h_param.set(Buffer(Float(32), N, 0, 0, 0, (uint8_t*)h->ptr(), "hh"));

        args.push_back(kernel_param);
        args.push_back(k_param);
        args.push_back(h_param);   
  
  		Func input, kernel, kk, hh;
		input = BoundaryConditions::repeat_image(input_param);
		kernel = BoundaryConditions::repeat_image(kernel_param);
		kk = BoundaryConditions::repeat_image(k_param);
		hh = BoundaryConditions::repeat_image(h_param);
		input.compute_root();
		kernel.compute_root();
		kk.compute_root();
		hh.compute_root();

        Func padded("padded"), res("res"), out("out");
        padded(x, y, j, i) = select(x>=0&&x<S&&y>=0&&y<S, input(clamp(x,0,S-1),clamp(y,0,S-1), j, i), 0.0f);
        
        RDom r(0, K, 0, K, 0, M);
        res(x, y, j, i) += kernel(r.x, r.y, r.z, j) * padded(x+1-r.x, y+1-r.y, r.z, i);
        
        out(x, y, j, i) = cast<itype>(select(res(x, y, j, i) * kk(j) + hh(j) <= 0, -1, 1));

        padded.compute_root();
#ifdef CPU_SCHEDULE
        Var fused("fused");
        res.update(0).fuse(j,i,fused).reorder(r.x,r.y,x,y,r.z,fused)
            .parallel(fused).vectorize(x,16);
#endif

#ifdef GPU_SCHEDULE
		Var fused("fused");
        res.update(0).fuse(j,i,fused).gpu_tile(x,y,fused,4,4,64);
#endif
        res.compute_root();
            
        return out; 
    }
    
};

#endif
