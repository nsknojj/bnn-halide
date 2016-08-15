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
int compiled_network(buffer_t *_p0_buffer, buffer_t *_p1_buffer, buffer_t *_p2_buffer, buffer_t *_p3_buffer, buffer_t *_p4_buffer, buffer_t *_p5_buffer, buffer_t *_p6_buffer, buffer_t *_p7_buffer, buffer_t *_p8_buffer, buffer_t *_p9_buffer, buffer_t *_p10_buffer, buffer_t *_p11_buffer, buffer_t *_p12_buffer, buffer_t *_p13_buffer, buffer_t *_p14_buffer, buffer_t *_p15_buffer, buffer_t *_p16_buffer, buffer_t *_p17_buffer, buffer_t *_p18_buffer, buffer_t *_p19_buffer, buffer_t *_p20_buffer, buffer_t *_p21_buffer, buffer_t *_p22_buffer, buffer_t *_p23_buffer, buffer_t *_p24_buffer, buffer_t *_p25_buffer, buffer_t *_out__18_buffer) HALIDE_FUNCTION_ATTRS;
int compiled_network_argv(void **args) HALIDE_FUNCTION_ATTRS;
extern const struct halide_filter_metadata_t compiled_network_metadata;
#ifdef __cplusplus
}  // extern "C"
#endif
#endif
