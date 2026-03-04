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
 * Apply a repeating-key XOR to a data buffer in place.
 *
 * Performs an in-place XOR of each byte in `data` with bytes from `key`, cycling through
 * `key` when its end is reached. If `data` is NULL, `key` is NULL, or `key_len` is zero,
 * the function returns without modifying the buffer.
 *
 * @param data Buffer to be XORed in place.
 * @param len Number of bytes in `data`.
 * @param key Key bytes used for XOR operations.
 * @param key_len Number of bytes in `key`.
 */
void baseline_xor_cipher(uint8_t* data, size_t len, const uint8_t* key, size_t key_len) {
    if (!data || !key || key_len == 0) return;
    size_t k = 0;
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key[k++];
        if (k == key_len) k = 0;
    }
}

/**
 * Run performance benchmarks for XOR cipher implementations and verify correctness.
 *
 * Benchmarks baseline and optimized XOR cipher implementations using a 50 MB buffer
 * for 1-byte and 8-byte keys, then performs a correctness check by applying an
 * 8-byte-key XOR twice and confirming the buffer is restored.
 *
 * @returns 0 on success; 1 on failure (memory allocation failure or correctness check failure).
 */
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
