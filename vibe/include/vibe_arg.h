/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the command-line argument parsing utilities.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_ARG_H
#define VIBE_ARG_H

#include <string.h>
#include <stdbool.h>

/**
 * vibe_arg_has - Checks if a specific flag is present in the command-line arguments.
 * @argc: Number of arguments
 * @argv: Array of argument strings
 * @flag: The flag to search for (e.g., "--verbose")
 *
 * Returns true if the flag is found, false otherwise.
 */
static inline bool vibe_arg_has(int argc, char** argv, const char* flag) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], flag) == 0) return true;
    }
    return false;
}

/**
 * vibe_arg_get - Gets the value associated with a specific command-line flag.
 * @argc: Number of arguments
 * @argv: Array of argument strings
 * @flag: The flag to search for
 *
 * Scans for the flag and returns the immediately following argument string.
 * Returns NULL if the flag is not found or is the last argument in the array.
 */
static inline char* vibe_arg_get(int argc, char** argv, const char* flag) {
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], flag) == 0) return argv[i+1];
    }
    return NULL;
}

#endif
