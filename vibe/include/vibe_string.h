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
 * Determine whether two C strings contain the same sequence of characters.
 *
 * @param s1 First null-terminated string to compare; may be NULL.
 * @param s2 Second null-terminated string to compare; may be NULL.
 * @returns `true` if both pointers are NULL or both strings are identical, `false` otherwise.
 */
static inline bool vibe_str_eq(const char* s1, const char* s2) {
    if (!s1 || !s2) return s1 == s2;
    return strcmp(s1, s2) == 0;
}

/**
 * Compare two strings for equality in a way that resists timing attacks.
 *
 * Compares the contents of `s1` and `s2` in constant time with respect to their length and reports whether they are identical. If either pointer is `NULL`, equality is true only when both are `NULL`.
 *
 * @param s1 First null-terminated string, or `NULL`.
 * @param s2 Second null-terminated string, or `NULL`.
 * @returns `true` if the strings are identical, `false` otherwise.
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
 * Copy up to size-1 characters from `src` into `dest` and ensure `dest` is NUL-terminated.
 *
 * If `dest` is NULL or `size` is zero, the function does nothing. If `src` is NULL,
 * `dest[0]` is set to `'\0'`. At most `size-1` bytes are copied and a terminating
 * NUL is always written when `size` is greater than zero.
 *
 * @param dest Destination buffer where the string will be written.
 * @param src Source string to copy from; may be NULL.
 * @param size Size of the destination buffer in bytes.
 */
static inline void vibe_str_copy(char* dest, const char* src, size_t size) {
    // Internal Logic: Guard against NULL pointers and zero sizes.
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

#endif
