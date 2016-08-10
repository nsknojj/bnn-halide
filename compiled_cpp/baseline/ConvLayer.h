#ifndef CONV_LAYER_H
#define CONV_LAYER_H

#include <SArray.h>

template<int M, int N, int S, int K, int B_S>
class ConvLayer{
public:
    SArray<int16_t, M*N*K*K> w;
    
    ConvLayer() {
        static_assert(K==3, "Conv layer only works for 3x3 convolutions!\n");
        static_assert(M==128, "Input conv layer assumes 128 input channels!\n");
    }
};

#endif
