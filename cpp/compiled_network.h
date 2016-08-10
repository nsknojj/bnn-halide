#ifndef HALIDE__compiled_network_h
#define HALIDE__compiled_network_h
#ifndef HALIDE_ATTRIBUTE_ALIGN
  #ifdef _MSC_VER
    #define HALIDE_ATTRIBUTE_ALIGN(x) __declspec(align(x))
  #else
    #define HALIDE_ATTRIBUTE_ALIGN(x) __attribute__((aligned(x)))
  #endif
#endif
#ifndef BUFFER_T_DEFINED
#define BUFFER_T_DEFINED
#include <stdbool.h>
#include <stdint.h>
typedef struct buffer_t {
    uint64_t dev;
    uint8_t* host;
    int32_t extent[4];
    int32_t stride[4];
    int32_t min[4];
    int32_t elem_size;
    HALIDE_ATTRIBUTE_ALIGN(1) bool host_dirty;
    HALIDE_ATTRIBUTE_ALIGN(1) bool dev_dirty;
    HALIDE_ATTRIBUTE_ALIGN(1) uint8_t _padding[10 - sizeof(void *)];
} buffer_t;
#endif
struct halide_filter_metadata_t;
#ifndef HALIDE_FUNCTION_ATTRS
#define HALIDE_FUNCTION_ATTRS
#endif
#ifdef __cplusplus
extern "C" {
#endif
int compiled_network(buffer_t *_p0_buffer, buffer_t *_p1_buffer, buffer_t *_p2_buffer, buffer_t *_p3_buffer, buffer_t *_p4_buffer, buffer_t *_p5_buffer, buffer_t *_p6_buffer, buffer_t *_p7_buffer, buffer_t *_p8_buffer, buffer_t *_p9_buffer, buffer_t *_p10_buffer, buffer_t *_p11_buffer, buffer_t *_p12_buffer, buffer_t *_p13_buffer, buffer_t *_p14_buffer, buffer_t *_p15_buffer, buffer_t *_p16_buffer, buffer_t *_p17_buffer, buffer_t *_p18_buffer, buffer_t *_p19_buffer, buffer_t *_p20_buffer, buffer_t *_p21_buffer, buffer_t *_p22_buffer, buffer_t *_p23_buffer, buffer_t *_p24_buffer, buffer_t *_p25_buffer, buffer_t *_p26_buffer, buffer_t *_p27_buffer, buffer_t *_p28_buffer, buffer_t *_p29_buffer, buffer_t *_p30_buffer, buffer_t *_p31_buffer, buffer_t *_p32_buffer, buffer_t *_p33_buffer, buffer_t *_p34_buffer, buffer_t *_p35_buffer, buffer_t *_p36_buffer, buffer_t *_p37_buffer, buffer_t *_p38_buffer, buffer_t *_p39_buffer, buffer_t *_p40_buffer, buffer_t *_p41_buffer, buffer_t *_p42_buffer, buffer_t *_p43_buffer, buffer_t *_p44_buffer, buffer_t *_p45_buffer, buffer_t *_p46_buffer, buffer_t *_p47_buffer, buffer_t *_p48_buffer, buffer_t *_p49_buffer, buffer_t *_p50_buffer, buffer_t *_p51_buffer, buffer_t *_p52_buffer, buffer_t *_p53_buffer, buffer_t *_p54_buffer, buffer_t *_p55_buffer, buffer_t *_p56_buffer, buffer_t *_p57_buffer, buffer_t *_p58_buffer, buffer_t *_p59_buffer, buffer_t *_p60_buffer, buffer_t *_p61_buffer, buffer_t *_p62_buffer, buffer_t *_p63_buffer, buffer_t *_p64_buffer, buffer_t *_p65_buffer, buffer_t *_p66_buffer, buffer_t *_p67_buffer, buffer_t *_p68_buffer, buffer_t *_p69_buffer, buffer_t *_p70_buffer, buffer_t *_p71_buffer, buffer_t *_p72_buffer, buffer_t *_p73_buffer, buffer_t *_p74_buffer, buffer_t *_p75_buffer, buffer_t *_p76_buffer, buffer_t *_p77_buffer, buffer_t *_p78_buffer, buffer_t *_p79_buffer, buffer_t *_p80_buffer, buffer_t *_p81_buffer, buffer_t *_p82_buffer, buffer_t *_p83_buffer, buffer_t *_p84_buffer, buffer_t *_p85_buffer, buffer_t *_p86_buffer, buffer_t *_p87_buffer, buffer_t *_p88_buffer, buffer_t *_p89_buffer, buffer_t *_p90_buffer, buffer_t *_p91_buffer, buffer_t *_p92_buffer, buffer_t *_p93_buffer, buffer_t *_p94_buffer, buffer_t *_p95_buffer, buffer_t *_p96_buffer, buffer_t *_p97_buffer, buffer_t *_p98_buffer, buffer_t *_p99_buffer, buffer_t *_p100_buffer, buffer_t *_p101_buffer, buffer_t *_p102_buffer, buffer_t *_p103_buffer, buffer_t *_p104_buffer, buffer_t *_p105_buffer, buffer_t *_p106_buffer, buffer_t *_p107_buffer, buffer_t *_p108_buffer, buffer_t *_p109_buffer, buffer_t *_p110_buffer, buffer_t *_p111_buffer, buffer_t *_p112_buffer, buffer_t *_p113_buffer, buffer_t *_p114_buffer, buffer_t *_p115_buffer, buffer_t *_p116_buffer, buffer_t *_p117_buffer, buffer_t *_p118_buffer, buffer_t *_p119_buffer, buffer_t *_p120_buffer, buffer_t *_p121_buffer, buffer_t *_p122_buffer, buffer_t *_p123_buffer, buffer_t *_p124_buffer, buffer_t *_p125_buffer, buffer_t *_p126_buffer, buffer_t *_p127_buffer, buffer_t *_p128_buffer, buffer_t *_p129_buffer, buffer_t *_p130_buffer, buffer_t *_p131_buffer, buffer_t *_p132_buffer, buffer_t *_p133_buffer, buffer_t *_p134_buffer, buffer_t *_p135_buffer, buffer_t *_p136_buffer, buffer_t *_p137_buffer, buffer_t *_p138_buffer, buffer_t *_p139_buffer, buffer_t *_p140_buffer, buffer_t *_p141_buffer, buffer_t *_p142_buffer, buffer_t *_p143_buffer, buffer_t *_p144_buffer, buffer_t *_p145_buffer, buffer_t *_p146_buffer, buffer_t *_p147_buffer, buffer_t *_p148_buffer, buffer_t *_p149_buffer, buffer_t *_p150_buffer, buffer_t *_p151_buffer, buffer_t *_p152_buffer, buffer_t *_p153_buffer, buffer_t *_p154_buffer, buffer_t *_p155_buffer, buffer_t *_p156_buffer, buffer_t *_p157_buffer, buffer_t *_p158_buffer, buffer_t *_p159_buffer, buffer_t *_p160_buffer, buffer_t *_p161_buffer, buffer_t *_p162_buffer, buffer_t *_p163_buffer, buffer_t *_p164_buffer, buffer_t *_p165_buffer, buffer_t *_p166_buffer, buffer_t *_p167_buffer, buffer_t *_p168_buffer, buffer_t *_p169_buffer, buffer_t *_p170_buffer, buffer_t *_p171_buffer, buffer_t *_p172_buffer, buffer_t *_p173_buffer, buffer_t *_p174_buffer, buffer_t *_p175_buffer, buffer_t *_p176_buffer, buffer_t *_p177_buffer, buffer_t *_p178_buffer, buffer_t *_p179_buffer, buffer_t *_p180_buffer, buffer_t *_p181_buffer, buffer_t *_p182_buffer, buffer_t *_p183_buffer, buffer_t *_p184_buffer, buffer_t *_p185_buffer, buffer_t *_p186_buffer, buffer_t *_p187_buffer, buffer_t *_p188_buffer, buffer_t *_p189_buffer, buffer_t *_p190_buffer, buffer_t *_p191_buffer, buffer_t *_p192_buffer, buffer_t *_p193_buffer, buffer_t *_p194_buffer, buffer_t *_p195_buffer, buffer_t *_p196_buffer, buffer_t *_p197_buffer, buffer_t *_p198_buffer, buffer_t *_p199_buffer, buffer_t *_p200_buffer, buffer_t *_p201_buffer, buffer_t *_p202_buffer, buffer_t *_p203_buffer, buffer_t *_p204_buffer, buffer_t *_p205_buffer, buffer_t *_p206_buffer, buffer_t *_p207_buffer, buffer_t *_p208_buffer, buffer_t *_p209_buffer, buffer_t *_p210_buffer, buffer_t *_p211_buffer, buffer_t *_p212_buffer, buffer_t *_p213_buffer, buffer_t *_p214_buffer, buffer_t *_p215_buffer, buffer_t *_p216_buffer, buffer_t *_p217_buffer, buffer_t *_p218_buffer, buffer_t *_p219_buffer, buffer_t *_p220_buffer, buffer_t *_p221_buffer, buffer_t *_p222_buffer, buffer_t *_p223_buffer, buffer_t *_p224_buffer, buffer_t *_p225_buffer, buffer_t *_p226_buffer, buffer_t *_p227_buffer, buffer_t *_p228_buffer, buffer_t *_p229_buffer, buffer_t *_p230_buffer, buffer_t *_p231_buffer, buffer_t *_p232_buffer, buffer_t *_p233_buffer, buffer_t *_p234_buffer, buffer_t *_p235_buffer, buffer_t *_p236_buffer, buffer_t *_p237_buffer, buffer_t *_p238_buffer, buffer_t *_p239_buffer, buffer_t *_p240_buffer, buffer_t *_p241_buffer, buffer_t *_p242_buffer, buffer_t *_p243_buffer, buffer_t *_p244_buffer, buffer_t *_p245_buffer, buffer_t *_p246_buffer, buffer_t *_p247_buffer, buffer_t *_p248_buffer, buffer_t *_p249_buffer, buffer_t *_p250_buffer, buffer_t *_p251_buffer, buffer_t *_p252_buffer, buffer_t *_p253_buffer, buffer_t *_p254_buffer, buffer_t *_p255_buffer, buffer_t *_p256_buffer, buffer_t *_p257_buffer, buffer_t *_p258_buffer, buffer_t *_p259_buffer, buffer_t *_out__180_buffer) HALIDE_FUNCTION_ATTRS;
int compiled_network_argv(void **args) HALIDE_FUNCTION_ATTRS;
extern const struct halide_filter_metadata_t compiled_network_metadata;
#ifdef __cplusplus
}  // extern "C"
#endif
#endif
