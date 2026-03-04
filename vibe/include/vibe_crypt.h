/**
 * This header provides cryptographic primitives, including an optimized XOR cipher and DJB2 hashing.
 * In version 1.5.11, it features optimized DJB2 hashing and maintains performance-critical 128-bit XOR processing.
 * This code is AI-generated.
 */
#ifndef VIBE_CRYPT_H
#define VIBE_CRYPT_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/**
 * vibe_xor_cipher - Simple XOR encryption/decryption
 * @data: Data to encrypt/decrypt (in-place)
 * @len: Length of data
 * @key: Key to use
 * @key_len: Length of key
 * Internal Logic: Uses word-sized XOR operations (SWAR) for common key sizes to maximize throughput.
 * Falls back to an optimized byte-wise loop for arbitrary key lengths.
 */
static inline void vibe_xor_cipher(uint8_t* data, size_t len, const uint8_t* key, size_t key_len) {
    // Internal Logic: Validate inputs to prevent NULL dereferences or division by zero in generic path.
    if (!data || !key || key_len == 0) return;

    // BOLT: Specialization for 1-byte keys to use 64-bit word-sized XOR operations.
    if (key_len == 1) {
        uint8_t k = key[0];
        // BOLT: Replicate byte using constant multiplication for efficient mask generation.
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

    // BOLT: Specialization for 2-byte keys to use 64-bit word-sized XOR operations.
    if (key_len == 2) {
        uint16_t k2;
        memcpy(&k2, key, 2);
        // BOLT: Replicate 2-byte key across 64 bits using constant multiplication.
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

    // BOLT: Specialization for 4-byte keys to use 64-bit word-sized XOR operations.
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

    // BOLT: Specialization for 16-byte keys to allow 128-bit processing using two 64-bit words.
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

    // BOLT: Specialization for 32-byte keys to allow 256-bit processing using four 64-bit words.
    if (key_len == 32) {
        uint64_t k[4];
        memcpy(&k[0], key, 8);
        memcpy(&k[1], key + 8, 8);
        memcpy(&k[2], key + 16, 8);
        memcpy(&k[3], key + 24, 8);
        size_t i = 0;
        for (; i + 32 <= len; i += 32) {
            uint64_t d[4];
            memcpy(&d[0], &data[i], 8);
            memcpy(&d[1], &data[i + 8], 8);
            memcpy(&d[2], &data[i + 16], 8);
            memcpy(&d[3], &data[i + 24], 8);
            d[0] ^= k[0]; d[1] ^= k[1]; d[2] ^= k[2]; d[3] ^= k[3];
            memcpy(&data[i], &d[0], 8);
            memcpy(&data[i + 8], &d[1], 8);
            memcpy(&data[i + 16], &d[2], 8);
            memcpy(&data[i + 24], &d[3], 8);
        }
        for (; i < len; i++) {
            data[i] ^= key[i & 31];
        }
        return;
    }

    // BOLT: Specialization for 8-byte keys to allow 64-bit word-sized XOR operations.
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

    // Internal Logic: Generic byte-wise XOR loop with incremental key index to avoid modulo operator.
    size_t k = 0;
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key[k++];
        if (k == key_len) k = 0;
    }
}

/**
 * vibe_simple_hash - A very simple non-cryptographic hash (DJB2)
 * Internal Logic: Optimized DJB2 hash algorithm using SWAR (SIMD Within A Register) for high-speed string hashing.
 */
static inline uint64_t vibe_simple_hash(const char* str) {
    // Internal Logic: Check for NULL input to prevent segmentation faults during hashing.
    if (!str) return 0;
    uint64_t hash = 5381;

    // BOLT: Align access to 8-byte boundary for optimal SWAR performance.
    while (((uintptr_t)str & 7) != 0) {
        unsigned char c = (unsigned char)*str++;
        if (c == 0) return hash;
        hash = ((hash << 5) + hash) + c;
    }

    // BOLT: Process 8 bytes at a time using 64-bit loads and zero-byte detection.
    while (1) {
        uint64_t v;
        memcpy(&v, str, 8);

        // BOLT: High-speed zero-byte detection trick (non-branching).
        if ((v - 0x0101010101010101ULL) & ~v & 0x8080808080808080ULL) break;

        // BOLT: Unrolled DJB2 updates for the 8-byte block.
        // We use a portable way to access bytes from the loaded word to maintain endian neutrality.
        const unsigned char* p = (const unsigned char*)&v;
        hash = ((hash << 5) + hash) + p[0];
        hash = ((hash << 5) + hash) + p[1];
        hash = ((hash << 5) + hash) + p[2];
        hash = ((hash << 5) + hash) + p[3];
        hash = ((hash << 5) + hash) + p[4];
        hash = ((hash << 5) + hash) + p[5];
        hash = ((hash << 5) + hash) + p[6];
        hash = ((hash << 5) + hash) + p[7];

        str += 8;
    }

    // Internal Logic: Process remaining bytes individually until null terminator.
    unsigned char c;
    while ((c = (unsigned char)*str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

#endif
