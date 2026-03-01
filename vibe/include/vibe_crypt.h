/**
 * This header provides cryptographic primitives, including an optimized XOR cipher and DJB2 hashing.
 * In version 1.5.9, it includes a SWAR-optimized DJB2 hashing implementation for high throughput.
 * This code is AI-generated.
 */
#ifndef VIBE_CRYPT_H
#define VIBE_CRYPT_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/**
 * vibe_xor_cipher - Simple XOR encryption/decryption
 * @data: Data to encrypt/decrypt (in-place)
 * @len: Length of data
 * @key: Key to use
 * @key_len: Length of key
 * Internal Logic: Uses word-sized XOR operations for common key sizes to maximize throughput.
 * Falls back to an optimized byte-wise loop for arbitrary key lengths.
 */
static inline void vibe_xor_cipher(uint8_t* data, size_t len, const uint8_t* key, size_t key_len) {
    if (!data || !key || key_len == 0) return;

    // BOLT: Specialization for 1-byte keys to use word-sized XOR operations.
    if (key_len == 1) {
        uint8_t k = key[0];
        uint64_t k8 = 0x0101010101010101ULL * k;
        size_t i = 0;
        for (; i + 8 <= len; i += 8) {
            uint64_t d;
            memcpy(&d, &data[i], 8);
            d ^= k8;
            memcpy(&data[i], &d, 8);
        }
        for (; i < len; i++) {
            data[i] ^= k;
        }
        return;
    }

    // BOLT: Specialization for 2-byte keys to use word-sized XOR operations.
    if (key_len == 2) {
        uint16_t k2;
        memcpy(&k2, key, 2);
        uint64_t k8 = 0x0001000100010001ULL * k2;
        size_t i = 0;
        for (; i + 8 <= len; i += 8) {
            uint64_t d;
            memcpy(&d, &data[i], 8);
            d ^= k8;
            memcpy(&data[i], &d, 8);
        }
        for (; i < len; i++) {
            data[i] ^= key[i & 1];
        }
        return;
    }

    // BOLT: Specialization for 4-byte keys to use word-sized XOR operations.
    if (key_len == 4) {
        uint32_t k4;
        memcpy(&k4, key, 4);
        uint64_t k8 = ((uint64_t)k4 << 32) | (uint64_t)k4;
        size_t i = 0;
        for (; i + 8 <= len; i += 8) {
            uint64_t d;
            memcpy(&d, &data[i], 8);
            d ^= k8;
            memcpy(&data[i], &d, 8);
        }
        for (; i < len; i++) {
            data[i] ^= key[i & 3];
        }
        return;
    }

    // BOLT: Specialization for 16-byte keys to allow word-sized XOR operations.
    if (key_len == 16) {
        uint64_t k_low, k_high;
        memcpy(&k_low, key, 8);
        memcpy(&k_high, key + 8, 8);
        size_t i = 0;
        for (; i + 16 <= len; i += 16) {
            uint64_t d_low, d_high;
            memcpy(&d_low, &data[i], 8);
            memcpy(&d_high, &data[i + 8], 8);
            d_low ^= k_low;
            d_high ^= k_high;
            memcpy(&data[i], &d_low, 8);
            memcpy(&data[i + 8], &d_high, 8);
        }
        for (; i < len; i++) {
            data[i] ^= key[i & 15];
        }
        return;
    }

    // BOLT: Specialization for 8-byte keys to allow word-sized XOR operations.
    if (key_len == 8) {
        uint64_t k8;
        memcpy(&k8, key, 8);
        size_t i = 0;
        for (; i + 8 <= len; i += 8) {
            uint64_t d;
            memcpy(&d, &data[i], 8);
            d ^= k8;
            memcpy(&data[i], &d, 8);
        }
        for (; i < len; i++) {
            data[i] ^= key[i & 7];
        }
        return;
    }

    // Internal Logic: Generic byte-wise XOR loop with incremental key index.
    size_t k = 0;
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key[k++];
        if (k == key_len) k = 0;
    }
}

/**
 * vibe_simple_hash - A very simple non-cryptographic hash (DJB2)
 * Internal Logic: Classic DJB2 hash algorithm using bit shifts and additions.
 * BOLT: Optimized using SWAR (SIMD Within A Register) to process 8 bytes at a time.
 */
static inline uint64_t vibe_simple_hash(const char* str) {
    if (!str) return 0;
    uint64_t hash = 5381;

    // BOLT: Process initial unaligned bytes.
    while (((uintptr_t)str & 7) != 0) {
        unsigned char c = (unsigned char)*str++;
        if (c == '\0') return hash;
        hash = ((hash << 5) + hash) + c;
    }

    const uint64_t* p8 = (const uint64_t*)str;
    while (true) {
        uint64_t v = *p8;
        // BOLT: High-speed zero-byte detection using SWAR.
        if ((v - 0x0101010101010101ULL) & ~v & 0x8080808080808080ULL) {
            break;
        }

        // BOLT: Access the loaded word through an unsigned char pointer for portable,
        // endian-neutral byte extraction which maintains performance.
        const unsigned char* b = (const unsigned char*)p8;
        hash = ((hash << 5) + hash) + b[0];
        hash = ((hash << 5) + hash) + b[1];
        hash = ((hash << 5) + hash) + b[2];
        hash = ((hash << 5) + hash) + b[3];
        hash = ((hash << 5) + hash) + b[4];
        hash = ((hash << 5) + hash) + b[5];
        hash = ((hash << 5) + hash) + b[6];
        hash = ((hash << 5) + hash) + b[7];
        p8++;
    }

    // BOLT: Process remaining bytes.
    str = (const char*)p8;
    while (*str) {
        hash = ((hash << 5) + hash) + (unsigned char)*str++;
    }
    return hash;
}

#endif
