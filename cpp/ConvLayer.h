#ifndef CONV_LAYER_H
#define CONV_LAYER_H

#include <Halide.h>
#include <cstddef>
#include <SArray.h>
#include <Timer.h>

#include <iostream>
using namespace Halide;

template<int M, int N, int S, int K, int B_S>
class ConvLayer{
public:
    SArray<int16_t, M*N*K*K> w;
    
    ConvLayer() {
        static_assert(K==3, "Conv layer only works for 3x3 convolutions!\n");
    }

    Func get_output(const Func input, std::vector<Argument> &args) {
        Var i("i"), j("j"), x("x"), y("y");

        ImageParam kernel(Int(16), 4);
        kernel.set(Buffer(Int(16), K, K, M, N, (uint8_t*)w.ptr(), "kernel"));
        args.push_back(kernel);

        Func converted("converted"), padded("padded"), res("res"), out("out");

        converted(x, y, j, i) = input(clamp(x, 0, S-1),clamp(y, 0, S-1),j,i);
        padded(x, y, j, i) = select(x>=0&&x<S&&y>=0&&y<S, converted(x,y,j,i), 0);

        RDom r(0, K, 0, K, 0, M);
        res(x, y, j, i) += kernel(r.x, r.y, r.z, j) * padded(x+1-r.x, y+1-r.y, r.z, i);

//        converted.compute_root();
        padded.compute_root();

        Var fused;
        int vector_size=16;
        if (S<16) vector_size=S;
        res.update(0).fuse(j,i,fused).vectorize(x,vector_size).parallel(fused)
            .reorder(r.x,r.y,r.z,x,y,fused);
        res.compute_root();
        
        return res;
    }
    
};

#endif
