/**
 * This header provides memory management utilities for the Vibe C library, including secure wiping.
 * In version 1.5.11, it continues to feature an optimized vibe_secure_memzero for safe clearing of sensitive data.
 * This code is AI-generated.
 */
#ifndef VIBE_MEM_H
#define VIBE_MEM_H
#include <stdlib.h>
#include <stdint.h>

// Internal Logic: Simple wrappers for standard memory allocation functions.
#define vibe_alloc(sz) malloc(sz)
#define vibe_free(p) free(p)

/**
 * vibe_secure_memzero - Securely clear memory to avoid compiler optimization
 * @p: Pointer to memory to clear
 * @len: Length of memory to clear
 * Internal Logic: Uses a volatile pointer and word-sized writes to ensure the data is cleared
 * and that the operation is not optimized away by the compiler.
 */
static inline void vibe_secure_memzero(void* p, size_t len) {
    // Internal Logic: Perform a NULL check to ensure the function is robust against uninitialized pointers.
    if (!p) return;

    // BOLT: Optimize by using word-sized (64-bit) writes while maintaining security.
    volatile uint8_t* p1 = (volatile uint8_t*)p;

    // Internal Logic: Align to 8 bytes to avoid unaligned access penalties on certain architectures.
    while (len > 0 && ((uintptr_t)p1 & 7) != 0) {
        *p1++ = 0;
        len--;
    }

    // Internal Logic: Process memory in 64-bit chunks for maximum throughput.
    volatile uint64_t* p8 = (volatile uint64_t*)p1;
    while (len >= 8) {
        *p8++ = 0;
        len -= 8;
    }

    // Internal Logic: Handle remaining bytes individually.
    p1 = (volatile uint8_t*)p8;
    while (len--) {
        *p1++ = 0;
    }
}

#endif
