/**
 * Simple command-line argument parsing utilities for Vibe C.
 * Provides helper functions to check for flags and retrieve associated values from argv.
 * This code is AI-generated.
 */
#ifndef VIBE_ARG_H
#define VIBE_ARG_H

#include <string.h>
#include <stdbool.h>

/**
 * Checks if a specific flag is present in the arguments.
 */
static inline bool vibe_arg_has(int argc, char** argv, const char* flag) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], flag) == 0) return true;
    }
    return false;
}

/**
 * Gets the value following a specific flag.
 * Returns NULL if the flag is not found or is the last argument.
 */
static inline char* vibe_arg_get(int argc, char** argv, const char* flag) {
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], flag) == 0) return argv[i+1];
    }
    return NULL;
}

#endif
