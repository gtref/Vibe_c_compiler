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
 * baseline_secure_memzero - Simple byte-wise secure memzero for comparison.
 * Internal Logic: Standard byte-by-byte volatile write loop to prevent optimization.
 */
void baseline_secure_memzero(void* p, size_t len) {
    if (!p) return;
    volatile unsigned char* ptr = (volatile unsigned char*)p;
    while (len--) {
        *ptr++ = 0;
    }
}

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
