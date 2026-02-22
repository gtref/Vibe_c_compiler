/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the benchmarking utilities for measuring execution time.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_BENCH_H
#define VIBE_BENCH_H
#include "vibe_time.h"
#include <stdio.h>
/**
 * VIBE_BENCHMARK - Measures the execution time of a code block.
 * @name: Descriptive name for the benchmark
 * @block: The code block to measure
 *
 * This macro uses vibe_get_time() to calculate the duration of the provided block
 * and prints the result to standard output.
 */
#define VIBE_BENCHMARK(name, block) do { \
    double start = vibe_get_time(); \
    block; \
    double end = vibe_get_time(); \
    printf("Benchmark '%s': %f seconds\n", name, end - start); /* nosec */ \
} while(0)
#endif
