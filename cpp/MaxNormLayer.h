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
        ImageParam k_param(Float(32), 1);
        ImageParam h_param(Float(32), 1);
        k_param.set(Buffer(Float(32), N, 0, 0, 0, (uint8_t*)k->ptr(), "kk"));
        h_param.set(Buffer(Float(32), N, 0, 0, 0, (uint8_t*)h->ptr(), "hh"));
        
        args.push_back(k_param);
        args.push_back(h_param);

		Func kk, hh;
		kk = BoundaryConditions::repeat_image(k_param);
		hh = BoundaryConditions::repeat_image(h_param);
		kk.compute_root();
		hh.compute_root();

        Var x("x"), y("y"), i("i"), j("j");
        Func pooled("pool"), out("out");
        RDom r(0, pool_size, 0, pool_size);
        
        pooled(x,y,j,i) = maximum(input(pool_size*x + r.x, pool_size*y + r.y, j, i));
        
        out(x,y,j,i) = cast<itype>(select(pooled(x,y,j,i) * kk(j) + hh(j) <= 0, -1, 1));
    
//        pooled.compute_root();
#ifdef GPU_SCHEDULE
		out.compute_root().fuse(j,i,i).gpu_tile(x,y,i,4,4,64);
#endif
        
        return out;
    }
};

#endif
