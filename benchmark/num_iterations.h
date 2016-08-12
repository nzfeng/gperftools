#ifndef __NUM_ITERATIONS_H__
#define __NUM_ITERATIONS_H__

// Defines the number of iterations the benchmark should run.

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NATIVE_FACTOR
#define NATIVE_FACTOR 1
#endif

const size_t BASE_ITERATIONS = 32768;
const size_t ITERATIONS = BASE_ITERATIONS * NATIVE_FACTOR;
const size_t REPEATS = 13;

#ifdef __cplusplus
}
#endif

#endif
