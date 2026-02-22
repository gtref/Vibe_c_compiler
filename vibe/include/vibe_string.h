/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the safe string operations and constant-time comparison utilities.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_STRING_H
#define VIBE_STRING_H
#include <string.h>
#include <stdbool.h>
/**
 * vibe_str_eq - Compares two strings for equality.
 * @s1: First string
 * @s2: Second string
 *
 * Includes NULL pointer checks. Returns true if strings are equal, false otherwise.
 */
static inline bool vibe_str_eq(const char* s1, const char* s2) {
    if (!s1 || !s2) return s1 == s2;
    return strcmp(s1, s2) == 0;
}

/**
 * vibe_str_eq_constant_time - Constant-time string comparison to prevent timing attacks.
 * @s1: First string
 * @s2: Second string
 *
 * Compares strings in a way that the execution time does not depend on the
 * number of matching characters, mitigating side-channel information leaks.
 * Returns true if strings are equal, false otherwise.
 */
static inline bool vibe_str_eq_constant_time(const char* s1, const char* s2) {
    if (!s1 || !s2) return s1 == s2;
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    int result = (len1 != len2);
    size_t compare_len = len1 < len2 ? len1 : len2;
    for (size_t i = 0; i < compare_len; i++) {
        result |= s1[i] ^ s2[i];
    }
    return result == 0;
}

#endif
