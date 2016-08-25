#ifndef TYPEDEFS_H
#define TYPEDEFS_H

//#define USE_FLOAT

#define GPU_SCHEDULE

//#define CPU_SCHEDULE

#ifdef USE_FLOAT

    typedef float itype;

#else

    typedef int16_t itype;

#endif

#endif
