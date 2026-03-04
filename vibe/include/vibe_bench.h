/**
 * This header provides a simple benchmarking utility for measuring the execution time of code blocks.
 * In version 1.5.11, the benchmarking macro has been updated for better timing accuracy using the monotonic clock.
 * This code is AI-generated.
 */
#ifndef VIBE_BENCH_H
#define VIBE_BENCH_H
#include "vibe_time.h"
#include "vibe_io.h"
#include <stdio.h>

/**
 * VIBE_BENCHMARK - Measures and prints the execution time of a code block.
 * Internal Logic: Records start time using vibe_get_time, executes the block, records end time, and prints the duration.
 */
#define VIBE_BENCHMARK(name, block) do { \
    double start = vibe_get_time(); \
    block; \
    double end = vibe_get_time(); \
    vibe_print("Benchmark '%s': %f seconds\n", name, end - start); \
} while(0)

#endif
