/**
 * Benchmark for vibe_simple_hash performance.
 * This code is AI-generated.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../vibe/include/vibe_crypt.h"

#define ITERATIONS 1000000
#define STR_LEN 1024

int main() {
    char* str = (char*)malloc(STR_LEN + 1);
    for (int i = 0; i < STR_LEN; i++) {
        str[i] = (char)('A' + (i % 26));
    }
    str[STR_LEN] = '\0';

    clock_t start = clock();
    uint64_t total_hash = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        total_hash += vibe_simple_hash(str);
        // Change one char to prevent some compiler optimizations if any
        str[0] = (char)('A' + (i % 26));
    }
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for %d iterations on %d byte string: %f seconds\n", ITERATIONS, STR_LEN, time_taken);
    printf("Hash result (to prevent dead code elimination): %llu\n", (unsigned long long)total_hash);

    free(str);
    return 0;
}
