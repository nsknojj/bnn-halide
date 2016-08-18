#ifndef MAX_NORM_LAYER_H
#define MAX_NORM_LAYER_H

#include <SArray.h>
  
//------------------------------------------------------------------------
// Max2NormLayer
//  - output map contains same number of maps, but each dim is shrinked
//  by factor of 2
//  - fused with the succeeding BatchNorm layer
//------------------------------------------------------------------------
template<int N, int S, int B_S>
class MaxNormLayer
{
    static const int S2 = S/2;
    static const int pool_size = 2;

public:

    SArray<float, N> *k;
    SArray<float, N> *h;

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------
    MaxNormLayer()
    {
        static_assert(S%2 == 0, "Max2Layer: S must be a multiple of 2\n");
	k = new SArray<float, N>;
	h = new SArray<float, N>;
    }

    ~MaxNormLayer()
    {
	delete k;
	delete h;
    }

};

#endif
