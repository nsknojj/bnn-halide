#ifndef BATCH_NORM_LAYER_H
#define BATCH_NORM_LAYER_H

#include <Halide.h>
#include <cstddef>
#include "SArray.h"
#include "Typedefs.h"

using namespace Halide;
  
//------------------------------------------------------------------------
// BatchNormLayer
//------------------------------------------------------------------------

template<int N, int S, int B_S>
class BatchNormLayer
{
public:

    SArray<float, N> *k;
    SArray<float, N> *h;

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------
    BatchNormLayer()
    {
        k = new SArray<float, N>;
        h = new SArray<float, N>;
    }

    ~BatchNormLayer()
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
        Func out("out");
       
        if (S>1)
            out(x,y,j,i) = cast<itype>(select(input(x,y,j,i) * kk(j) + hh(j) <= 0, -1, 1));
        else
            out(j,i) = cast<itype>(select(input(j,i) * kk(j) + hh(j) <= 0, -1, 1));
        
        return out;
    }
};

#endif
