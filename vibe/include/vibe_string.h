/**
 * This header provides secure string comparison utilities for the Vibe C library.
 * In version 1.5.11, it maintains hardened constant-time comparison to prevent timing attacks and handle NULL inputs.
 * This code is AI-generated.
 */
#ifndef VIBE_STRING_H
#define VIBE_STRING_H
#include <string.h>
#include <stdbool.h>

/**
 * vibe_str_eq - Simple string equality check
 * Returns true if strings are identical, false otherwise.
 * Internal Logic: Guard against NULL pointer dereferences by checking inputs before calling strcmp.
 */
static inline bool vibe_str_eq(const char* s1, const char* s2) {
    // BOLT: Pointer equality fast-path to avoid strcmp for identical strings.
    if (s1 == s2) return true;
    if (!s1 || !s2) return false;
    return strcmp(s1, s2) == 0;
}

/**
 * vibe_str_eq_constant_time - Constant-time string comparison to prevent timing attacks
 * Internal Logic: We use a single pass to compare characters until both strings reach null terminators.
 */
static inline bool vibe_str_eq_constant_time(const char* s1, const char* s2) {
    // Internal Logic: Return equality based on pointer comparison if either is NULL to avoid crashes.
    if (!s1 || !s2) return s1 == s2;

    int result = 0;
    bool done1 = false;
    bool done2 = false;
    size_t i = 0;

    while (true) {
        unsigned char c1 = done1 ? 0 : (unsigned char)s1[i];
        unsigned char c2 = done2 ? 0 : (unsigned char)s2[i];

        if (c1 == '\0') done1 = true;
        if (c2 == '\0') done2 = true;

        if (done1 && done2) break;

        result |= (c1 ^ c2);
        i++;
    }

    return result == 0;
}

/**
 * vibe_str_copy - Securely copy a string into a buffer of known size
 * Internal Logic: Copy characters until the end of src or size-1 limit is reached, ensuring null-termination.
 */
static inline void vibe_str_copy(char* dest, const char* src, size_t size) {
    // Internal Logic: Guard against NULL pointers and zero sizes.
    if (!dest || size == 0) return;
    if (!src) {
        dest[0] = '\0';
        return;
    }

    // BOLT: Use memchr and memcpy for high-performance string copying.
    const char* end = (const char*)memchr(src, '\0', size - 1);
    size_t copy_len = end ? (size_t)(end - src) : size - 1;
    memcpy(dest, src, copy_len);
    dest[copy_len] = '\0';
}

#endif
