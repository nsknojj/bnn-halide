#ifndef BATCH_NORM_LAYER_H
#define BATCH_NORM_LAYER_H

#include <SArray.h>
  
//------------------------------------------------------------------------
// BatchNormLayer
//------------------------------------------------------------------------

template<int N, int S, int B_S>
class BatchNormLayer
{
public:

    SArray<float, N> k;
    SArray<float, N> h;

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------
    BatchNormLayer()
    {
    }
};

#endif
