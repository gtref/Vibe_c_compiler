/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the POSIX regular expression matching and safety checks.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_REGEX_H
#define VIBE_REGEX_H

#include <regex.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * vibe_regex_match - Check if a string matches a POSIX extended regular expression.
 * @pattern: The regular expression pattern string
 * @text: The input text to check
 *
 * Compiles and executes the regex. Includes NULL pointer checks for safety.
 * Returns true if the text matches the pattern, false otherwise.
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
