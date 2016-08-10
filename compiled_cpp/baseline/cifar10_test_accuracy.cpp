#include <iostream>

#include "ParamIO.h"
#include "DataIO.h"
#include "Timer.h"
#include "compiled_network.h"
#include "InputConvLayer.h"
#include "ConvLayer.h"

const int batch_size = 1;
const int N = 10;

void set_buf(buffer_t &buf, uint8_t* pt, int x=0, int y=0, int z=0, int w=0, int size=0) {
    buf.host=(uint8_t*)pt;
    buf.extent[0]=x;
    buf.extent[1]=y;
    buf.extent[2]=z;
    buf.extent[3]=w;
    buf.stride[0]=1;
    if (y>0) buf.stride[1]=x;
    if (z>0) buf.stride[2]=x*y;
    if (w>0) buf.stride[3]=x*y*z;
    buf.elem_size=size;
}

int main(int argc, char** argv) {
    Cifar10TestInputs X(N);
    Cifar10TestLabels Y(N);
    
    Params params(get_root_dir() + params_file);
   
    // layer definition
    InputConvLayer<3,128,32,3,batch_size> input_conv1;
    ConvLayer<128,128,32,3,batch_size> conv2;
   
    // test
    for (int n=0;n<N;n+=batch_size) {
        float* data = X.data + n*3*32*32;
        short output[batch_size*128*32*32];
        
        // deliver params to each layer
        input_conv1.w.binarize_from(params.float_data(0));
        input_conv1.k.copy_from(params.float_data(1));
        input_conv1.h.copy_from(params.float_data(2));
        conv2.w.binarize_from(params.float_data(3));
        
        // input, output buffer
        buffer_t input_buf={0}, output_buf={0};
        set_buf(input_buf, (uint8_t*)data, 32, 32, 3, batch_size, sizeof(float));
        set_buf(output_buf, (uint8_t*)&output[0], 32, 32, 128, batch_size, sizeof(short));
        // set inputConvLayer1 buffer
        buffer_t input_conv1_w={0}, input_conv1_k={0}, input_conv1_h={0};
        set_buf(input_conv1_w, (uint8_t*)input_conv1.w.ptr(), 3, 3, 3, 128, sizeof(float));
        set_buf(input_conv1_k, (uint8_t*)input_conv1.k.ptr(), 128, 0, 0, 0, sizeof(float));
        set_buf(input_conv1_h, (uint8_t*)input_conv1.h.ptr(), 128, 0, 0, 0, sizeof(float));
        // set ConvLayer2 buffer
        buffer_t conv2_w={0};
        set_buf(conv2_w, (uint8_t*)conv2.w.ptr(), 3, 3, 128, 128, sizeof(int16_t));        
        
        Timer t;
        t.start();
        
        compiled_network(
            &input_buf,
            &input_conv1_w, &input_conv1_k, &input_conv1_h,
            &conv2_w,
            &output_buf
        );
        
        t.stop();

        // for(int i=0;i<32;i++) {
        //     for(int j=0;j<32;j++)
        //         printf("%d ", output(j,i,0,9));
        //     printf("\n");
        // }
    }
}
