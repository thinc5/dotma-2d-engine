#ifndef DEBUG_H
#define DEBUG_H

/* This header file contains macros that allow for different build behaviour
   between debug and release builds. */

/**
 * If we are building a debug build we want DEBUG_PRINT to alias to printf,
 * else we want it to do nothing and get removed by preprocessor.
 */
#ifdef DEBUG

#define DEBUG_PRINT(...) fprintf(__VA_ARGS__)
#define INFO_LOG(...) fprintf(stdout, __VA_ARGS__)
#define ERROR_LOG(...) fprintf(stderr, __VA_ARGS__)
#else

#define DEBUG_PRINT(...)
#define INFO_LOG(...)
#define ERROR_LOG(...)

#endif

#endif
