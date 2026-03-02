/**
 * This benchmark compares the performance of the SWAR-optimized DJB2 hash against a large number of iterations.
 * In version 1.5.9, it verifies the performance gains achieved by processing 8 bytes at a time.
 * This code is AI-generated.
 */
#include "vibe_crypt.h"
#include "vibe_time.h"
#include <stdio.h>
#include <string.h>

int main() {
    char data[1024];
    memset(data, 'A', 1023);
    data[1023] = '\0';

    printf("Benchmarking vibe_simple_hash on 1024-byte string...\n");

    double start = vibe_get_time();
    uint64_t total_hash = 0;
    int iterations = 1000000;

    for (int i = 0; i < iterations; i++) {
        total_hash += vibe_simple_hash(data);
    }

    double end = vibe_get_time();
    double elapsed = end - start;

    printf("Iterations: %d\n", iterations);
    printf("Elapsed:    %.4f seconds\n", elapsed);
    printf("Hashes/sec: %.2f\n", (double)iterations / elapsed);
    printf("Result:     %llu (checksum)\n", (unsigned long long)total_hash);

    return 0;
}
