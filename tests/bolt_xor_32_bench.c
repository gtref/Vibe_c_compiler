#include "../vibe/include/vibe_crypt.h"
#include "../vibe/include/vibe_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void baseline_xor_cipher(uint8_t* data, size_t len, const uint8_t* key, size_t key_len) {
    if (!data || !key || key_len == 0) return;
    size_t k = 0;
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key[k++];
        if (k == key_len) k = 0;
    }
}

int main() {
    size_t len = 50 * 1024 * 1024; // 50MB
    uint8_t* data = malloc(len);
    if (!data) return 1;

    uint8_t key32[32];
    for(int i=0; i<32; i++) key32[i] = (uint8_t)i;

    printf("Benchmarking XOR cipher with 32-byte key (50MB data)...\n");

    memset(data, 0, len);
    double start = vibe_get_time();
    baseline_xor_cipher(data, len, key32, 32);
    double end = vibe_get_time();
    printf("Baseline (generic loop): %.6f seconds\n", end - start);

    memset(data, 0, len);
    start = vibe_get_time();
    vibe_xor_cipher(data, len, key32, 32);
    double end_opt = vibe_get_time();
    printf("Optimized (uses 32-byte specialization): %.6f seconds\n", end_opt - start);

    free(data);
    return 0;
}
