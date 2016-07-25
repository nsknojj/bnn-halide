#ifndef CONV_LAYER_H
#define CONV_LAYER_H

#include <Halide.h>
#include <cstddef>
#include <SArray.h>
#include <Timer.h>
using namespace Halide;

template<int M, int N, int S, int K, int B_S>
class ConvLayer{
public:
    SArray<float, M*N*K*K> w;
    SArray<float, N> k;
    SArray<float, N> h;
    ConvLayer() {
        static_assert(K==3, "Conv layer only works for 3x3 convolutions!\n");
        static_assert(M==128, "Input conv layer assumes 128 input channels!\n");
    }
    
    void get_output(Image<float> &input, Image<float> &output) {
        Var d("d"), c("c"), x("x"), y("y");
        
        Image<float> kernel(Buffer(Float(32), K, K, M, N, (uint8_t*)w.ptr(), "kernel"));

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

        Func padded("padded"), res("res"), out("out");
        padded = BoundaryConditions::constant_exterior(input, 0);
        
        RDom r(0, K, 0, K, 0, M);
        res(x, y, c, d) += kernel(r.x, r.y, r.z, c) * padded(x+1-r.x, y+1-r.y, r.z, d);
        res.parallel(x);
        res.update(0).parallel(y);
        Timer t("timer");
        res.print_loop_nest();
        t.start();
        output = res.realize(S, S, N, B_S);
        t.stop();
    }
    
    
};

#endif
