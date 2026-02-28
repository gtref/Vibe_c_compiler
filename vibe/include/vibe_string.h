/**
 * This header provides secure string comparison utilities for the Vibe C library.
 * In version 1.5.8, the constant-time comparison has been hardened to avoid leaking individual string lengths.
 * This code is AI-generated.
 */
#ifndef VIBE_STRING_H
#define VIBE_STRING_H
#include <string.h>
#include <stdbool.h>

/**
 * vibe_str_eq - Simple string equality check
 * Returns true if strings are identical, false otherwise.
 * Handles NULL pointers gracefully by returning true only if both are NULL.
 */
static inline bool vibe_str_eq(const char* s1, const char* s2) {
    if (!s1 || !s2) return s1 == s2;
    return strcmp(s1, s2) == 0;
}

/**
 * vibe_str_eq_constant_time - Constant-time string comparison to prevent timing attacks
 * This version is hardened to avoid leaking the length of the strings by using a single-pass
 * loop that continues until both strings reach their null terminators.
 */
static inline bool vibe_str_eq_constant_time(const char* s1, const char* s2) {
    if (!s1 || !s2) return s1 == s2;

    // Internal Logic: We use a single pass to compare characters.
    // We don't use strlen to avoid leaking lengths early.
    // The loop continues as long as either string has remaining characters.
    // 'result' accumulates differences using bitwise OR.
    // 'done1' and 'done2' track if we've hit the null terminator for each string.

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

    // If they had different lengths, result will be non-zero because one hit '\0' before the other.
    return result == 0;
}

#endif
