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
        Func out("out");
       
        if (S>1) {
            out(x,y,j,i) = cast<itype>(select(input(x,y,j,i) * kk(j) + hh(j) <= 0, -1, 1));

#ifdef GPU_SCHEDULE
			//out.fuse(j,i,i).gpu_tile(x,y,i,4,4,64);
#endif
		}
        else {
            out(j,i) = cast<itype>(select(input(j,i) * kk(j) + hh(j) <= 0, -1, 1));

#ifdef GPU_SCHEDULE
			//out.fuse(j,i,i).gpu_tile(i,64);
#endif
		}
        
        return out;
    }
};

#endif
