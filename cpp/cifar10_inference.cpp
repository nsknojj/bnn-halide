#include <iostream>

#include "ParamIO.h"
#include "DataIO.h"
#include "Timer.h"
#include "Halide.h"
#include "InputConvLayer.h"
#include "ConvLayer.h"

using namespace Halide;

const auto batch_size = 10;
const auto N = 1;

int main(int argc, char** argv) {
    
    Cifar10TestInputs X(N);
    Cifar10TestLabels Y(N);
    
    Params params(get_root_dir() + params_file);
   
    // layer definition
    InputConvLayer<3,128,32,3,batch_size> input_conv;
    ConvLayer<128,128,32,3,batch_size> conv1;
   
    // test
    for (int n=0;n<N;n+=batch_size) {
        float* data = X.data + n*3*32*32;
        Image<float> input(Buffer(Float(32), 32, 32, 3, batch_size, (uint8_t*)data, "input"));
        Image<float> output(32, 32, 128, batch_size);
        input_conv.w.binarize_from(params.float_data(0));
        input_conv.k.copy_from(params.float_data(1));
        input_conv.h.copy_from(params.float_data(2));
        input_conv.get_output(input, output);
        
        
        Image<float> output2(32, 32, 128, batch_size);
        conv1.w.binarize_from(params.float_data(3));
        conv1.get_output(output, output2);
       
        for(int i=0;i<32;i++) {
            for(int j=0;j<32;j++)
                //printf("%.1f ", input(j,i,2,0));
                printf("%.0f ", output2(j,i,0,0));
            printf("\n");
        }
    }
        
    // binarize_from(params.float_data(0));
}
