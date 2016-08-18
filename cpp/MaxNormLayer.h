#ifndef MAX_NORM_LAYER_H
#define MAX_NORM_LAYER_H

#include <Halide.h>
#include <cstddef>
#include "SArray.h"
#include "Typedefs.h"

using namespace Halide;
  
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
        k = new SArray<float,N>;
        h = new SArray<float,N>;
    }

    ~MaxNormLayer()
    {
        delete k;
        delete h;
    }

    Func get_output(Func input, std::vector<Argument> &args) {
        ImageParam kk(Float(32), 1);
        ImageParam hh(Float(32), 1);
        kk.set(Buffer(Float(32), N, 0, 0, 0, (uint8_t*)k->ptr(), "kk"));
        hh.set(Buffer(Float(32), N, 0, 0, 0, (uint8_t*)h->ptr(), "hh"));
        
        args.push_back(kk);
        args.push_back(hh);

        Var x("x"), y("y"), i("i"), j("j");
        Func pooled("pool"), out("out");
        RDom r(0, pool_size, 0, pool_size);
        
        pooled(x,y,j,i) = maximum(input(pool_size*x + r.x, pool_size*y + r.y, j, i));
        
        out(x,y,j,i) = cast<itype>(select(pooled(x,y,j,i) * kk(j) + hh(j) <= 0, -1, 1));
    
        pooled.compute_root();
        
        return out;
    }
};

#endif
