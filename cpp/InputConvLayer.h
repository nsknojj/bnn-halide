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
    
    Func get_output(ImageParam &input, std::vector<Argument> &args) {
        Var i("i"), j("j"), x("x"), y("y");
        
        ImageParam kernel(Float(32), 4);
        ImageParam kk(Float(32), 1);
        ImageParam hh(Float(32), 1);
        kernel.set(Buffer(Float(32), K, K, M, N, (uint8_t*)w->ptr(), "kernel"));
        kk.set(Buffer(Float(32), N, 0, 0, 0, (uint8_t*)k->ptr(), "kk"));
        hh.set(Buffer(Float(32), N, 0, 0, 0, (uint8_t*)h->ptr(), "hh"));

        args.push_back(kernel);
        args.push_back(kk);
        args.push_back(hh);   
   
        Func padded("padded"), res("res"), out("out");
        padded(x, y, j, i) = select(x>=0&&x<S&&y>=0&&y<S, input(clamp(x,0,S-1),clamp(y,0,S-1), j, i), 0.0f);
        
        RDom r(0, K, 0, K, 0, M);
        res(x, y, j, i) += kernel(r.x, r.y, r.z, j) * padded(x+1-r.x, y+1-r.y, r.z, i);
        
        out(x, y, j, i) = cast<itype>(select(res(x, y, j, i) * kk(j) + hh(j) <= 0, -1, 1));

        padded.compute_root();
#ifdef SCHEDULE
        Var fused("fused");
        res.update(0).fuse(j,i,fused).reorder(r.x,r.y,x,y,r.z,fused)
            .parallel(fused).vectorize(x,16);
#endif
        res.compute_root();
            
        return out; 
    }
    
    void debug_print() {
        //        for (int channel=0;channel<3;channel++){
        //            for (int i=0;i<3;i++) {
        //                for (int j=0;j<3;j++)
        //                    printf("%.3f ", input(j,i,channel,0));
        //                printf("\n");
        //            }
        //            printf("\n");
        //        }
        //
        //        for (int channel=0;channel<3;channel++){
        //            for (int i=0;i<3;i++) {
        //                for (int j=0;j<3;j++)
        //                    printf("%.1f ", kernel(j,i,channel,0));
        //                printf("\n");
        //            }
        //            printf("\n");
        //        }
    }
    
    
};

#endif
