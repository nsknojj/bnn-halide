#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_LEVEL 2

#ifdef DEBUG_LEVEL

  #define DB(lvl, x) if (lvl <= DEBUG_LEVEL) {x;}
  #define DB_PRINT(lvl, ...) \
    if (lvl <= DEBUG_LEVEL) \
      printf (__VA_ARGS__)

#else
  
  #define DB(lvl, x)   
  #define DB_PRINT(lvl, ...)

#endif

#endif
