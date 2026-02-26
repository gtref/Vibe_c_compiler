/**
 * Benchmarking utilities for Vibe C.
 * Provides a simple macro to measure and display the execution time of any code block.
 * This code is AI-generated.
 */
#ifndef VIBE_BENCH_H
#define VIBE_BENCH_H

#include "vibe_time.h"
#include <stdio.h>

/* BOLT: Simple micro-benchmarking macro to measure and print execution time */
#define VIBE_BENCHMARK(name, block) do { \
    double start = vibe_get_time(); \
    block; \
    double end = vibe_get_time(); \
    printf("Benchmark '%s': %f seconds\n", name, end - start); /* nosec */ \
} while(0)

#endif
