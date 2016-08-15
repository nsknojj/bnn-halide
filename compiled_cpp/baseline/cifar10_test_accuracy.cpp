#include <iostream>

#include "ParamIO.h"
#include "DataIO.h"
#include "Timer.h"
#include "compiled_network.h"
#include "InputConvLayer.h"
#include "ConvLayer.h"
#include "BatchNormLayer.h"
#include "MaxNormLayer.h"
#include "DenseLayer.h"

const int batch_size = 10;
const int N = 100;

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
   
    // test
    int n_errors = 0;
    Timer t;

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
    
    // set inputConvLayer1 buffer
    buffer_t input_conv1_w={0}, input_conv1_k={0}, input_conv1_h={0};
    set_buf(input_conv1_w, (uint8_t*)input_conv1.w.ptr(), 3, 3, 3, 128, sizeof(float));
    set_buf(input_conv1_k, (uint8_t*)input_conv1.k.ptr(), 128, 0, 0, 0, sizeof(float));
    set_buf(input_conv1_h, (uint8_t*)input_conv1.h.ptr(), 128, 0, 0, 0, sizeof(float));
    // set ConvLayer2 buffer
    buffer_t conv2_w={0}, norm2_k={0}, norm2_h={0};
    set_buf(conv2_w, (uint8_t*)conv2.w.ptr(), 3, 3, 128, 128, sizeof(int16_t));
    set_buf(norm2_k, (uint8_t*)norm2.k.ptr(), 128, 0, 0, 0, sizeof(float));
    set_buf(norm2_h, (uint8_t*)norm2.h.ptr(), 128, 0, 0, 0, sizeof(float));
    // ConvLayer3
    buffer_t conv3_w={0}, norm3_k={0}, norm3_h={0};
    set_buf(conv3_w, (uint8_t*)conv3.w.ptr(), 3, 3, 128, 256, sizeof(int16_t));
    set_buf(norm3_k, (uint8_t*)norm3.k.ptr(), 256, 0, 0, 0, sizeof(float));
    set_buf(norm3_h, (uint8_t*)norm3.h.ptr(), 256, 0, 0, 0, sizeof(float));
    // ConvLayer4
    buffer_t conv4_w={0}, norm4_k={0}, norm4_h={0};
    set_buf(conv4_w, (uint8_t*)conv4.w.ptr(), 3, 3, 256, 256, sizeof(int16_t));
    set_buf(norm4_k, (uint8_t*)norm4.k.ptr(), 256, 0, 0, 0, sizeof(float));
    set_buf(norm4_h, (uint8_t*)norm4.h.ptr(), 256, 0, 0, 0, sizeof(float));        
    // ConvLayer5
    buffer_t conv5_w={0}, norm5_k={0}, norm5_h={0};
    set_buf(conv5_w, (uint8_t*)conv5.w.ptr(), 3, 3, 256, 512, sizeof(int16_t));
    set_buf(norm5_k, (uint8_t*)norm5.k.ptr(), 512, 0, 0, 0, sizeof(float));
    set_buf(norm5_h, (uint8_t*)norm5.h.ptr(), 512, 0, 0, 0, sizeof(float));
    // ConvLayer6
    buffer_t conv6_w={0}, norm6_k={0}, norm6_h={0};
    set_buf(conv6_w, (uint8_t*)conv6.w.ptr(), 3, 3, 512, 512, sizeof(int16_t));
    set_buf(norm6_k, (uint8_t*)norm6.k.ptr(), 512, 0, 0, 0, sizeof(float));
    set_buf(norm6_h, (uint8_t*)norm6.h.ptr(), 512, 0, 0, 0, sizeof(float));
    // Dense1
    buffer_t dense1_w={0}, norm7_k={0}, norm7_h={0};
    set_buf(dense1_w, (uint8_t*)dense1.w, 1024, 512*4*4, 0, 0, sizeof(float));
    set_buf(norm7_k, (uint8_t*)norm7.k.ptr(), 1024, 0, 0, 0, sizeof(float));
    set_buf(norm7_h, (uint8_t*)norm7.h.ptr(), 1024, 0, 0, 0, sizeof(float));
    // Dense2        
    buffer_t dense2_w={0}, norm8_k={0}, norm8_h={0};
    set_buf(dense2_w, (uint8_t*)dense2.w, 1024, 1024, 0, 0, sizeof(float));
    set_buf(norm8_k, (uint8_t*)norm8.k.ptr(), 1024, 0, 0, 0, sizeof(float));
    set_buf(norm8_h, (uint8_t*)norm8.h.ptr(), 1024, 0, 0, 0, sizeof(float));
    // Dense3
    buffer_t dense3_w={0};
    set_buf(dense3_w, (uint8_t*)dense3.w, 10, 1024, 0, 0, sizeof(float));

    for (int n=0;n<N;n+=batch_size) {
        int b_s=std::min(batch_size, N-n);
        float* data = X.data + n*3*32*32;
        short output[10][b_s];
        
        // set input, output buffer
        buffer_t input_buf={0}, output_buf={0};
        set_buf(input_buf, (uint8_t*)data, 32, 32, 3, b_s, sizeof(float));
        set_buf(output_buf, (uint8_t*)output, 10, b_s, 0, 0, sizeof(int16_t));

        t.start();
        
        compiled_network(
            &input_buf,
            &input_conv1_w, &input_conv1_k, &input_conv1_h,
            &conv2_w, &norm2_k, &norm2_h,
            &conv3_w, &norm3_k, &norm3_h,
            &conv4_w, &norm4_k, &norm4_h,
            &conv5_w, &norm5_k, &norm5_h,
            &conv6_w, &norm6_k, &norm6_h,
            &dense1_w, &norm7_k, &norm7_h,
            &dense2_w, &norm8_k, &norm8_k,
            &dense3_w,
            &output_buf
        );
        
        for (int j = 0;j < b_s; j++) {
            int prediction = 11;
            float maxval = -1e20;
            for (int i = 0; i < 10; ++i) {
                float k = params.float_data(25)[i];
                float h = params.float_data(26)[i];
                float val = output[i][j] * k + h;
                printf("%d ", output[i][j]);
                if (val > maxval) {
                    prediction = i;
                    maxval = val;
                }
            }

            int label = Y.data[n+j];
            n_errors += prediction!=label;
            // uncomment to print all prediction
            printf ("  Pred/Label:\t%2u/%2d\t[%s]\n", prediction, label,((prediction==label)?" OK ":"FAIL"));
        }
        
        t.stop();

        // for(int i=0;i<32;i++) {
        //     for(int j=0;j<32;j++)
        //         printf("%d ", output(j,i,0,9));
        //     printf("\n");
        // }
    }
    
    printf(" error rate = %d / %d\n", n_errors, N);
    printf(" total time = %.4lfs\n", t.totalTime);
    printf(" time per image = %.6lfs\n", t.totalTime / N);
}
