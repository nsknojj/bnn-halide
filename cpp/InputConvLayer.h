#ifndef INPUT_CONV_LAYER_H
#define INPUT_CONV_LAYER_H

#include <Halide.h>
#include <cstddef>
#include <SArray.h>
using namespace Halide;

template<int M, int N, int S, int K, int B_S>
class InputConvLayer{
public:
    SArray<float, M*N*K*K> w;
    SArray<float, N> k;
    SArray<float, N> h;
    InputConvLayer() {
        static_assert(K==3, "Conv layer only works for 3x3 convolutions!\n");
        static_assert(M==3, "Input conv layer assumes 3 input channels!\n");
    }
    
    void get_output(Image<float> &input, Image<float> &output) {
        Var d("d"), c("c"), x("x"), y("y");
        
        Image<float> kernel(Buffer(Float(32), K, K, M, N, (uint8_t*)w.ptr(), "kernel"));
        Image<float> kk(Buffer(Float(32), N, 0, 0, 0, (uint8_t*)k.ptr(), "kk"));
        Image<float> hh(Buffer(Float(32), N, 0, 0, 0, (uint8_t*)h.ptr(), "hh"));


        
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
        //res.parallel(r.z);
        out(x, y, c, d) = (select(res(x, y, c, d) * kk(c) + hh(c) < 0, -1.0f, 1.0f));
        //out.parallel(x);
        output = out.realize(S, S, N, B_S);
        
    }
    
    
};

#endif
