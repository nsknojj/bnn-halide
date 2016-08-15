#ifndef DENSE_LAYER_H
#define DENSE_LAYER_H

#include <Halide.h>
#include <cstddef>
#include <SArray.h>

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
        ImageParam ww(Float(32), 2);
        ww.set(Buffer(Float(32), N, M, 0, 0, (uint8_t*)w, "kk"));

        args.push_back(ww);

        Var i("i"), bs("bs");
        RDom j(0,M,"j");
        Func out("out");
 
        out(i, bs) += input(j, bs) * cast<int16_t>(select(ww(i, j)<=0, (int16_t)-1, (int16_t)1));

        //Var x_outer, x_inner, w_outer, w_inner;
        //out.update().tile(x, w, x_outer, w_outer, x_inner, w_inner, 64, 16);

        out.update().parallel(i);
        if (N>=16&&N%16==0) out.update().vectorize(i,16);
        out.compute_root();
        
        return out;
    }

};

#endif
