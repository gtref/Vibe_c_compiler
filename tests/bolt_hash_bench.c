/**
 * This benchmark measures the performance of the vibe_simple_hash function.
 * In version 1.5.11, it compares the optimized SWAR-based DJB2 hash against a byte-wise baseline.
 * This code is AI-generated.
 */
#include <stdio.h>
#include <string.h>
#include "vibe_crypt.h"
#include "vibe_time.h"

/**
 * baseline_hash - Non-optimized version for baseline comparison.
 * Internal Logic: Standard byte-by-byte DJB2 implementation.
 */
__attribute__((noinline))
uint64_t baseline_hash(const char* str) {
    if (!str) return 0;
    uint64_t hash = 5381;
    unsigned char c;
    while ((c = (unsigned char)*str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

int main() {
    // Internal Logic: Prepare a 1024-byte string for benchmarking.
    char buffer[1024];
    memset(buffer, 'A', sizeof(buffer));
    buffer[1023] = '\0';

    int iterations = 1000000;
    volatile uint64_t sink = 0;

    // Warm up the CPU caches for baseline.
    for(int i=0; i<100000; i++) sink = baseline_hash(buffer);

    printf("Benchmarking baseline_hash with %d iterations on 1024-byte string...\n", iterations); // nosec
    double start = vibe_get_time();
    for (int i = 0; i < iterations; i++) {
        sink = baseline_hash(buffer);
    }
    double end = vibe_get_time();
    double baseline_time = end - start;
    printf("Baseline time: %.6f seconds\n", baseline_time); // nosec

    sink = 0;
    // Warm up the CPU caches for optimized version.
    for(int i=0; i<100000; i++) sink = vibe_simple_hash(buffer);

    printf("Benchmarking optimized vibe_simple_hash with %d iterations on 1024-byte string...\n", iterations); // nosec
    start = vibe_get_time();
    for (int i = 0; i < iterations; i++) {
        sink = vibe_simple_hash(buffer);
    }
    end = vibe_get_time();
    double optimized_time = end - start;
    printf("Optimized time: %.6f seconds\n", optimized_time); // nosec

    // Internal Logic: Output speedup and percentage improvement metrics.
    printf("Speedup: %.2fx\n", baseline_time / optimized_time); // nosec
    printf("Improvement: %.2f%%\n", (baseline_time - optimized_time) / baseline_time * 100.0); // nosec

    (void)sink;
    return 0;
}
