/**
 * This benchmark tool measures the performance gain of the optimized XOR cipher for 2-byte and 16-byte keys.
 * In version 1.5.11, it continues to compare specialized word-sized paths (SWAR) against baseline loops.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_crypt.h"
#include "../vibe/include/vibe_bench.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * baseline_xor_cipher - Simple byte-wise XOR implementation for benchmarking comparison.
 * Internal Logic: Generic byte-by-byte XOR loop with modulo key indexing.
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
    // Internal Logic: Allocate 100MB of data for stable throughput measurement.
    size_t len = 100 * 1024 * 1024; // 100MB for test
    uint8_t* data = (uint8_t*)malloc(len);
    if (!data) return 1;

    uint8_t key2[2] = {0xAA, 0xBB};
    uint8_t key16[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};

    printf("--- Bolt XOR Extended Performance Benchmark ---\n"); // nosec

    // Internal Logic: Run benchmarks for 2-byte and 16-byte key sizes.
    memset(data, 0, len);
    VIBE_BENCHMARK("2-byte key (Baseline)", {
        baseline_xor_cipher(data, len, key2, 2);
    });
    memset(data, 0, len);
    VIBE_BENCHMARK("2-byte key (Current)", {
        vibe_xor_cipher(data, len, key2, 2);
    });

    memset(data, 0, len);
    VIBE_BENCHMARK("16-byte key (Baseline)", {
        baseline_xor_cipher(data, len, key16, 16);
    });
    memset(data, 0, len);
    VIBE_BENCHMARK("16-byte key (Current)", {
        vibe_xor_cipher(data, len, key16, 16);
    });

    free(data);
    return 0;
}
