/**
 * This benchmark measures the performance gain of optimized constant-time string comparison.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_string.h"
#include "../vibe/include/vibe_bench.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    size_t lengths[] = {16, 128, 1024, 1024 * 1024};
    const char* labels[] = {"16B", "128B", "1KB", "1MB"};
    int num_lengths = 4;

    printf("--- Constant-Time String Comparison Benchmark ---\n"); // nosec

    for (int i = 0; i < num_lengths; i++) {
        size_t len = lengths[i];
        char* s1 = (char*)malloc(len + 1);
        char* s2 = (char*)malloc(len + 1);
        memset(s1, 'A', len);
        s1[len] = '\0';
        memcpy(s2, s1, len + 1);

        printf("Length: %s\n", labels[i]); // nosec
        char bench_label[64];
        snprintf(bench_label, sizeof(bench_label), "vibe_str_eq_constant_time (%s)", labels[i]);

        VIBE_BENCHMARK(bench_label, {
            vibe_str_eq_constant_time(s1, s2);
        });

        free(s1);
        free(s2);
    }

    return 0;
}
