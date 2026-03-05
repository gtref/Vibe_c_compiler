/**
 * This benchmark measures the performance of vibe_str_copy.
 */
#include "../vibe/include/vibe_string.h"
#include "../vibe/include/vibe_bench.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void vibe_str_copy_original(char* dest, const char* src, size_t size) {
    if (!dest || size == 0) return;
    if (!src) {
        dest[0] = '\0';
        return;
    }

    size_t i = 0;
    while (i < size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
}

void vibe_str_copy_memcpy(char* dest, const char* src, size_t size) {
    if (!dest || size == 0) return;
    if (!src) {
        dest[0] = '\0';
        return;
    }

    const char* end = (const char*)memchr(src, '\0', size - 1);
    size_t copy_len = end ? (size_t)(end - src) : size - 1;
    memcpy(dest, src, copy_len);
    dest[copy_len] = '\0';
}

int main() {
    size_t lengths[] = {16, 128, 1024, 1024 * 1024};
    const char* labels[] = {"16B", "128B", "1KB", "1MB"};
    int num_lengths = 4;

    printf("--- String Copy Benchmark ---\n");

    for (int i = 0; i < num_lengths; i++) {
        size_t len = lengths[i];
        char* src = (char*)malloc(len + 1);
        char* dest = (char*)malloc(len + 1);
        memset(src, 'A', len);
        src[len] = '\0';

        printf("Length: %s\n", labels[i]);

        char label1[64];
        snprintf(label1, sizeof(label1), "original (%s)", labels[i]);
        VIBE_BENCHMARK(label1, {
            for(int j=0; j<100; j++) vibe_str_copy_original(dest, src, len + 1);
        });

        char label2[64];
        snprintf(label2, sizeof(label2), "memcpy (%s)", labels[i]);
        VIBE_BENCHMARK(label2, {
            for(int j=0; j<100; j++) vibe_str_copy_memcpy(dest, src, len + 1);
        });

        free(src);
        free(dest);
    }

    return 0;
}
