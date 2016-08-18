#ifndef INPUT_CONV_LAYER_H
#define INPUT_CONV_LAYER_H

#include <SArray.h>

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
};

#endif
