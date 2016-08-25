#ifndef CONV_LAYER_H
#define CONV_LAYER_H

#include <Halide.h>
#include <cstddef>
#include "SArray.h"
#include "Typedefs.h" 

#include <iostream>
using namespace Halide;

template<int M, int N, int S, int K, int B_S>
class ConvLayer{
public:
    SArray<int16_t, M*N*K*K> *w;
    
    ConvLayer() {
        static_assert(K==3, "Conv layer only works for 3x3 convolutions!\n");
        w = new SArray<int16_t, M*N*K*K>;
    }

    ~ConvLayer() {
        delete w;
    }

    Func get_output(Func input, std::vector<Argument> &args) {
        Var i("i"), j("j"), x("x"), y("y");

        ImageParam kernel_param(Int(16), 4);
        kernel_param.set(Buffer(Int(16), K, K, M, N, (uint8_t*)w->ptr(), "kernel"));
        args.push_back(kernel_param);

		Func kernel;
		kernel = BoundaryConditions::repeat_image(kernel_param);
		kernel.compute_root();

        Func padded("padded"), res("res"), out("out");

        padded(x, y, j, i) = select(x>=0&&x<S&&y>=0&&y<S, input(clamp(x, 0, S-1),clamp(y, 0, S-1),j,i), 0);

        RDom r(0, K, 0, K, 0, M);
        res(x, y, j, i) += kernel(r.x, r.y, r.z, j) * padded(x+1-r.x, y+1-r.y, r.z, i);

//        converted.compute_root();
        padded.compute_root();
#ifdef CPU_SCHEDULE
        Var fused;
        int vector_size=16;
        if (S<16) vector_size=S;
        res.update(0).fuse(j,i,fused).vectorize(x,vector_size).parallel(fused)
            .reorder(r.x,r.y,r.z,x,y,fused);
#endif

#ifdef GPU_SCHEDULE
        Var fused;
        int vector_size=16;
        if (S<16) vector_size=S;
        res.update(0).fuse(j,i,fused).gpu_tile(x,y,fused,4,4,16);
#endif

        res.compute_root();
        
        return res;
    }
    
};

#endif
