/**
 * POSIX regular expression wrapper for Vibe C.
 * Provides a simplified interface for pattern matching with built-in NULL pointer safety.
 * This code is AI-generated.
 */
#ifndef VIBE_REGEX_H
#define VIBE_REGEX_H

#include <regex.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * vibe_regex_match - Check if a string matches a pattern with POSIX extended regex
 */
static inline bool vibe_regex_match(const char* pattern, const char* text) {
    if (!pattern || !text) return false;
    regex_t regex;
    int reti;
    bool result = false;

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) return false;

    reti = regexec(&regex, text, 0, NULL, 0);
    if (!reti) {
        result = true;
    }

    regfree(&regex);
    return result;
}

#endif
