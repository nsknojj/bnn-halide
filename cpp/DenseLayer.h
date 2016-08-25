#ifndef DENSE_LAYER_H
#define DENSE_LAYER_H

#include <Halide.h>
#include <cstddef>
#include "SArray.h"
#include "Typedefs.h"

using namespace std;
  
template<int M, int N, int B_S>
class DenseLayer
{
public:
  
    const float* w;

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------
    DenseLayer()
    {
    }
    
    Func get_output(Func input, std::vector<Argument> &args) {
        ImageParam w_param(Float(32), 2);
        w_param.set(Buffer(Float(32), N, M, 0, 0, (uint8_t*)w, "kk"));

        args.push_back(w_param);

		Func ww;
		ww = BoundaryConditions::repeat_image(w_param);
		ww.compute_root();

        Var i("i"), bs("bs");
        RDom j(0,M,"j");
        Func out("out");
 
        out(i, bs) += input(j, bs) * cast<itype>(select(ww(i, j)<=0, -1, 1));

        out.update().parallel(i);
#ifdef CPU_SCHEDULE        
        if (N>=16&&N%16==0) out.update().vectorize(i,16);
#endif

#ifdef GPU_SCHEDULE
		if (N>=16&&N%16==0) 
			out.gpu_tile(i,bs,1024,1);
#endif
        out.compute_root();
        
        return out;
    }

};

#endif
