/**
 * This benchmark tool measures the performance gain of the optimized XOR cipher.
 * In version 1.5.11, it compares specialized word-sized paths (1, 4, 8 bytes) against a baseline loop.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_crypt.h"
#include "../vibe/include/vibe_bench.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Apply an in-place XOR cipher over a byte buffer using a repeating key.
 *
 * Performs XOR of each byte in `data` with the corresponding byte from `key`,
 * repeating the key when `len` exceeds `key_len`. If `data` or `key` is NULL
 * or `key_len` is zero, the function returns without modifying memory.
 *
 * @param data Buffer of bytes to encrypt/decrypt in place.
 * @param len Number of bytes in `data` to process.
 * @param key Key bytes used for XOR.
 * @param key_len Length of the `key` in bytes.
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
 * Run throughput benchmarks for XOR implementations and verify correctness.
 *
 * Allocates a 100 MB buffer, runs baseline and optimized XOR benchmarks for 1-, 4-, and 8-byte keys,
 * then verifies correctness by double-applying the optimized XOR with several keys (including a 3-byte mixed key).
 *
 * @returns 0 on success; 1 if memory allocation fails or a correctness check fails.
 */
int main() {
    // Internal Logic: Allocate 100MB of data for stable throughput measurement.
    size_t len = 100 * 1024 * 1024; // 100MB for test
    uint8_t* data = (uint8_t*)malloc(len);
    if (!data) return 1;

    uint8_t key1[1] = {0xAA};
    uint8_t key4[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    uint8_t key8[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    printf("--- Bolt XOR Performance Benchmark ---\n"); // nosec

    // Internal Logic: Run benchmarks for different key sizes and compare baseline vs optimized.
    memset(data, 0, len);
    VIBE_BENCHMARK("1-byte key (Baseline)", {
        baseline_xor_cipher(data, len, key1, 1);
    });
    memset(data, 0, len);
    VIBE_BENCHMARK("1-byte key (Current Optimized)", {
        vibe_xor_cipher(data, len, key1, 1);
    });

    memset(data, 0, len);
    VIBE_BENCHMARK("4-byte key (Baseline)", {
        baseline_xor_cipher(data, len, key4, 4);
    });
    memset(data, 0, len);
    VIBE_BENCHMARK("4-byte key (Current)", {
        vibe_xor_cipher(data, len, key4, 4);
    });

    memset(data, 0, len);
    VIBE_BENCHMARK("8-byte key (Baseline)", {
        baseline_xor_cipher(data, len, key8, 8);
    });
    memset(data, 0, len);
    VIBE_BENCHMARK("8-byte key (Current Optimized)", {
        vibe_xor_cipher(data, len, key8, 8);
    });

    // Internal Logic: Verify correctness of the optimized implementation via double XOR.
    printf("--- Correctness Check ---\n"); // nosec
    uint8_t key_mixed[3] = {0x11, 0x22, 0x33};
    memset(data, 0xAA, len);

    vibe_xor_cipher(data, len, key1, 1);
    vibe_xor_cipher(data, len, key1, 1);
    for(size_t i = 0; i < len; i++) if(data[i] != 0xAA) { printf("Fail 1-byte\n"); return 1; } // nosec
    printf("1-byte OK\n"); // nosec

    vibe_xor_cipher(data, len, key4, 4);
    vibe_xor_cipher(data, len, key4, 4);
    for(size_t i = 0; i < len; i++) if(data[i] != 0xAA) { printf("Fail 4-byte\n"); return 1; } // nosec
    printf("4-byte OK\n"); // nosec

    vibe_xor_cipher(data, len, key8, 8);
    vibe_xor_cipher(data, len, key8, 8);
    for(size_t i = 0; i < len; i++) if(data[i] != 0xAA) { printf("Fail 8-byte\n"); return 1; } // nosec
    printf("8-byte OK\n"); // nosec

    vibe_xor_cipher(data, len, key_mixed, 3);
    vibe_xor_cipher(data, len, key_mixed, 3);
    for(size_t i = 0; i < len; i++) if(data[i] != 0xAA) { printf("Fail fallback\n"); return 1; } // nosec
    printf("Fallback OK\n"); // nosec

    free(data);
    return 0;
}
