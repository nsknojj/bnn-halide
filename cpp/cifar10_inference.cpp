#include <iostream>
#include <algorithm>

#include "ParamIO.h"
#include "DataIO.h"
#include "Timer.h"
#include "Halide.h"
#include "InputConvLayer.h"
#include "ConvLayer.h"
#include "MaxNormLayer.h"
#include "BatchNormLayer.h"
#include "DenseLayer.h"

using namespace Halide;

const auto batch_size = 100;
const auto N = 1000;

std::vector<Argument> args;

// layer definition
InputConvLayer<3,128,32,3,batch_size>   input_conv1;
ConvLayer<128,128,32,3,batch_size>      conv2;
MaxNormLayer<128,32,batch_size>         norm2;
ConvLayer<128,256,16,3,batch_size>      conv3;
BatchNormLayer<256,16,batch_size>       norm3;
ConvLayer<256,256,16,3,batch_size>      conv4;
MaxNormLayer<256,16,batch_size>         norm4;
ConvLayer<256,512,8,3,batch_size>       conv5;
BatchNormLayer<512,8,batch_size>        norm5;
ConvLayer<512,512,8,3,batch_size>       conv6;
MaxNormLayer<512,8,batch_size>          norm6;
DenseLayer<512*16,1024,batch_size>      dense1;
BatchNormLayer<1024,1,batch_size>       norm7;
DenseLayer<1024,1024,batch_size>        dense2;
BatchNormLayer<1024,1,batch_size>       norm8;
DenseLayer<1024,10,batch_size>          dense3;

Func img2line(Func input, int N, int S) {
//    const int S = 4;
    int stride = S*S;
    Var i("i"), x("x");
    Func out("out");
//    input.compute_root();
    out(x, i) = input((x % stride) % S, (x % stride) / S, x / stride, i);
//    out.compute_root();
    return out;
}

int main(int argc, char** argv) {
    
    Cifar10TestInputs X(N);
    Cifar10TestLabels Y(N);
    
    Params params(get_root_dir() + params_file);
   
    // deliver params to each layer
    input_conv1.w.binarize_from(params.float_data(0));
    input_conv1.k.copy_from(params.float_data(1));
    input_conv1.h.copy_from(params.float_data(2));
    
    conv2.w.binarize_from(params.float_data(3));
    norm2.k.copy_from(params.float_data(4));
    norm2.h.copy_from(params.float_data(5));
    
    conv3.w.binarize_from(params.float_data(6));
    norm3.k.copy_from(params.float_data(7));
    norm3.h.copy_from(params.float_data(8));
    
    conv4.w.binarize_from(params.float_data(9));
    norm4.k.copy_from(params.float_data(10));
    norm4.h.copy_from(params.float_data(11));
    
    conv5.w.binarize_from(params.float_data(12));
    norm5.k.copy_from(params.float_data(13));
    norm5.h.copy_from(params.float_data(14));
    
    conv6.w.binarize_from(params.float_data(15));
    norm6.k.copy_from(params.float_data(16));
    norm6.h.copy_from(params.float_data(17));
    
    dense1.w = params.float_data(18);
    norm7.k.copy_from(params.float_data(19));
    norm7.h.copy_from(params.float_data(20));
    dense2.w = params.float_data(21);
    norm8.k.copy_from(params.float_data(22));
    norm8.h.copy_from(params.float_data(23));
    dense3.w = params.float_data(24);
    
    // test
    int n_errors = 0;

    for (int n=0;n<N;n+=batch_size) {
        float* data = X.data + n*3*32*32;
        ImageParam input(Float(32), 4);
        input.set(Buffer(Float(32), 32, 32, 3, batch_size, (uint8_t*)data, "input"));
        // add input param to args
        args.push_back(input);

        Func out("out");
        out = input_conv1.get_output(input, args);
        out = conv2.get_output(out, args);
        out = norm2.get_output(out, args);
        out = conv3.get_output(out, args);
        out = norm3.get_output(out, args);
        out = conv4.get_output(out, args);
        out = norm4.get_output(out, args);
        out = conv5.get_output(out, args);
        out = norm5.get_output(out, args);
        out = conv6.get_output(out, args);
        out = norm6.get_output(out, args);
        out = img2line(out, 512, 4);
        out = dense1.get_output(out, args);
        out = norm7.get_output(out, args);
        out = dense2.get_output(out, args);
        out = norm8.get_output(out, args);
        out = dense3.get_output(out, args);

        out.print_loop_nest();
        out.compile_to_file("compiled_network", args);

        int b_s = std::min(N-n, batch_size);
        Image<int16_t> output(10,b_s);
      
//        out.realize(output);
        
//        for(int l=0;l<10;l++){
//        for(int c=0;c<1024;c++)
//        for(int i=0;i<4;i++) {
//            for(int j=0;j<4;j++)
//                printf("%d ", output(c,l));
//            printf("\n");
//        }

        for (int j = 0;j < b_s; j++) {
            int prediction = 11;
            float maxval = -1e20;
            for (int i = 0; i < 10; ++i) {
                float k = params.float_data(25)[i];
                float h = params.float_data(26)[i];
                float val = output(i, j) * k + h;
                //printf("%d ", output(i,j));
                if (val > maxval) {
                    prediction = i;
                    maxval = val;
                }
            }

            int label = Y.data[n+j];
            n_errors += prediction!=label;

            printf ("  Pred/Label:\t%2u/%2d\t[%s]\n", prediction, label,((prediction==label)?" OK ":"FAIL"));
        }

                
        Timer t;
        t.start();
        out.realize(output);
        t.stop();

    }
    
    printf(" error rate = %d / %d\n", n_errors, N);
}
