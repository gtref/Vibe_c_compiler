/**
 * This benchmark compares the performance of the optimized vibe_secure_memzero against a simple byte-wise loop.
 * In version 1.5.11, it continues to demonstrate the efficiency of word-sized writes for large buffers.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_mem.h"
#include "../vibe/include/vibe_bench.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * Overwrite the memory region pointed to by p with zeros in a manner that resists compiler optimization.
 *
 * Performs a byte-wise volatile write loop to ensure the zeroing is not optimized away; intended as a
 * simple baseline implementation for benchmarking secure memzero approaches.
 *
 * @param p Pointer to the start of the memory region to clear. If NULL, the function does nothing.
 * @param len Number of bytes to overwrite with zeros.
 */
void baseline_secure_memzero(void* p, size_t len) {
    if (!p) return;
    volatile unsigned char* ptr = (volatile unsigned char*)p;
    while (len--) {
        *ptr++ = 0;
    }
}

/**
 * Run a performance benchmark comparing a baseline byte-wise secure memory zeroing
 * implementation with the optimized `vibe_secure_memzero`.
 *
 * Allocates ~100 MB of test buffer, uses an intentionally unaligned pointer to exercise
 * alignment handling, prints a benchmark header, runs both benchmarks, and frees the buffer.
 *
 * @returns 0 on success, 1 if memory allocation fails.
 */
int main() {
    // Internal Logic: Allocate 100MB of data and use an unaligned pointer to test the library's alignment handling.
    size_t len = 100 * 1024 * 1024; // 100MB
    void* data = malloc(len + 8);
    if (!data) return 1;

    // Test with unaligned pointer
    void* p = (void*)((uintptr_t)data + 1);

    printf("--- Secure Memzero Performance Benchmark ---\n"); // nosec

    VIBE_BENCHMARK("Baseline Secure Memzero", {
        baseline_secure_memzero(p, len);
    });

    VIBE_BENCHMARK("Optimized vibe_secure_memzero", {
        vibe_secure_memzero(p, len);
    });

    free(data);
    return 0;
}
