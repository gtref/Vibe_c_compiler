/**
 * This benchmark compares the optimized SWAR vibe_simple_hash against a baseline.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_crypt.h"
#include "../vibe/include/vibe_bench.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * baseline_simple_hash - Standard DJB2 implementation.
 */
uint64_t baseline_simple_hash(const char* str) {
    if (!str) return 0;
    uint64_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

int main() {
    size_t len = 50 * 1024 * 1024; // 50MB
    char* data = malloc(len + 16);
    if (!data) return 1;

    // Test with various alignments
    for (int align = 0; align < 8; align++) {
        char* str = data + align;
        memset(str, 'A', len);
        str[len] = '\0';

        printf("--- Hash Alignment %d Benchmark ---\n", align);

        // Correctness check
        uint64_t h1 = baseline_simple_hash(str);
        uint64_t h2 = vibe_simple_hash(str);
        if (h1 != h2) {
            printf("FAILED Alignment %d: h1=%llu, h2=%llu\n", align, (unsigned long long)h1, (unsigned long long)h2);
            free(data);
            return 1;
        }

        VIBE_BENCHMARK("Baseline Hash", {
            volatile uint64_t h = baseline_simple_hash(str);
            (void)h;
        });

        VIBE_BENCHMARK("Optimized vibe_simple_hash", {
            volatile uint64_t h = vibe_simple_hash(str);
            (void)h;
        });
    }

    // Small string test
    const char* small = "Hello, Vibe!";
    if (baseline_simple_hash(small) != vibe_simple_hash(small)) {
        printf("FAILED small string\n");
        free(data);
        return 1;
    }
    printf("Small string OK\n");

    free(data);
    return 0;
}
