/**
 * This benchmark measures the performance of the XOR cipher with a 32-byte key.
 * In version 1.5.11, it verifies the throughput of the 256-bit SWAR specialization.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_crypt.h"
#include "../vibe/include/vibe_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * baseline_xor_cipher - Simple byte-wise XOR implementation for benchmarking comparison.
 * Internal Logic: Iterates through data, XORing each byte with the key using modulo indexing.
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
    // Internal Logic: Allocate 50MB of memory for the performance test.
    size_t len = 50 * 1024 * 1024; // 50MB
    uint8_t* data = (uint8_t*)malloc(len);
    if (!data) return 1;

    uint8_t key32[32];
    for(int i=0; i<32; i++) key32[i] = (uint8_t)i;

    printf("Benchmarking XOR cipher with 32-byte key (50MB data)...\n"); // nosec

    // Internal Logic: Benchmark the baseline implementation.
    memset(data, 0, len);
    double start = vibe_get_time();
    baseline_xor_cipher(data, len, key32, 32);
    double end = vibe_get_time();
    printf("Baseline (generic loop): %.6f seconds\n", end - start); // nosec

    // Internal Logic: Benchmark the optimized implementation using 32-byte specialization.
    memset(data, 0, len);
    start = vibe_get_time();
    vibe_xor_cipher(data, len, key32, 32);
    double end_opt = vibe_get_time();
    printf("Optimized (uses 32-byte specialization): %.6f seconds\n", end_opt - start); // nosec

    free(data);
    return 0;
}
