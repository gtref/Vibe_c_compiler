#ifndef VIBE_STRING_H
#define VIBE_STRING_H
#include <string.h>
#include <stdbool.h>
static inline bool vibe_str_eq(const char* s1, const char* s2) {
    if (!s1 || !s2) return s1 == s2;
    return strcmp(s1, s2) == 0;
}

/**
 * vibe_str_eq_constant_time - Constant-time string comparison to prevent timing attacks
 */
static inline bool vibe_str_eq_constant_time(const char* s1, const char* s2) {
    if (!s1 || !s2) return s1 == s2;
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;
    unsigned char result = 0;
    size_t i = 0;

    // Single-pass comparison to avoid length-leaking strlen calls.
    // For strings of equal length, this loop always runs for the full length plus the null terminator.
    while (1) {
        unsigned char c1 = p1[i];
        unsigned char c2 = p2[i];
        result |= (c1 ^ c2);
        if (c1 == '\0' || c2 == '\0') break;
        i++;
    }

    return result == 0 && p1[i] == '\0' && p2[i] == '\0';
}

#endif
