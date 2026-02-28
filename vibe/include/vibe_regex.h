/**
 * This header provides a simple wrapper for the POSIX regular expression library.
 * In version 1.5.8, it continues to provide easy pattern matching with automatic resource management.
 * This code is AI-generated.
 */
#ifndef VIBE_REGEX_H
#define VIBE_REGEX_H

#include <regex.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * vibe_regex_match - Check if a string matches a pattern
 * Internal Logic: Compiles the regex, executes it, and then frees the regex resources.
 * Returns true if the text matches the extended POSIX regex pattern.
 */
static inline bool vibe_regex_match(const char* pattern, const char* text) {
    if (!pattern || !text) return false;
    regex_t regex;
    int reti;
    bool result = false;

    // Internal Logic: Use REG_EXTENDED for modern regex support.
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) return false;

    reti = regexec(&regex, text, 0, NULL, 0);
    if (!reti) {
        result = true;
    }

    // Internal Logic: Always free the compiled regex memory to prevent leaks.
    regfree(&regex);
    return result;
}

#endif
