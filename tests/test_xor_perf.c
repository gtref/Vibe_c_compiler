/**
 * This test script evaluates the performance improvement of the optimized XOR cipher implementation.
 * In version 1.5.11, it measures the throughput of word-sized key paths against a standard baseline for various sizes.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_crypt.h"
#include "../vibe/include/vibe_bench.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * baseline_xor_cipher - Unoptimized XOR implementation used as a performance baseline.
 * Internal Logic: Iterates through the data buffer byte-by-byte using modulo indexing for the key.
 */
void baseline_xor_cipher(uint8_t* data, size_t len, const uint8_t* key, size_t key_len) {
    if (!data || !key || key_len == 0) return;
    size_t k = 0;
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key[k++];
        if (k == key_len) k = 0;
    }
}

int main() {
    // Internal Logic: Use a 50MB buffer to benchmark the cipher's performance across different key sizes.
    size_t len = 50 * 1024 * 1024; // 50MB for test
    uint8_t* data = (uint8_t*)malloc(len);
    if (!data) return 1;

    uint8_t key1[1] = {0xAA};
    uint8_t key8[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    printf("--- Performance Benchmark ---\n"); // nosec

    // Internal Logic: Benchmark the 1-byte key specialization.
    memset(data, 0, len);
    VIBE_BENCHMARK("1-byte key (Baseline)", {
        baseline_xor_cipher(data, len, key1, 1);
    });

    memset(data, 0, len);
    VIBE_BENCHMARK("1-byte key (Optimized)", {
        vibe_xor_cipher(data, len, key1, 1);
    });

    // Internal Logic: Benchmark the 8-byte key specialization.
    memset(data, 0, len);
    VIBE_BENCHMARK("8-byte key (Baseline)", {
        baseline_xor_cipher(data, len, key8, 8);
    });

    memset(data, 0, len);
    VIBE_BENCHMARK("8-byte key (Optimized)", {
        vibe_xor_cipher(data, len, key8, 8);
    });

    // Internal Logic: Verify correctness by ensuring that double XORing restores the original 0x55 pattern.
    memset(data, 0x55, len);
    vibe_xor_cipher(data, len, key8, 8);
    vibe_xor_cipher(data, len, key8, 8);
    for (size_t i = 0; i < len; i++) {
        if (data[i] != 0x55) {
            printf("Error: Correctness check failed at index %zu\n", i); // nosec
            free(data);
            return 1;
        }
    }
    printf("Correctness check passed.\n"); // nosec

    free(data);
    return 0;
}
