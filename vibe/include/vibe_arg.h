/**
 * This header provides simple command-line argument parsing utilities for the Vibe C library.
 * In version 1.5.9, it includes robust checks for flag presence and associated value retrieval from argv.
 * This code is AI-generated.
 */
#ifndef VIBE_ARG_H
#define VIBE_ARG_H

#include <string.h>
#include <stdbool.h>

/**
 * vibe_arg_has - Checks if a specific flag is present in the arguments.
 * Internal Logic: Iterates through the argv array and performs a standard string comparison for each element.
 */
static inline bool vibe_arg_has(int argc, char** argv, const char* flag) {
    // Internal Logic: Perform a NULL check to ensure the function is robust against empty argument lists.
    if (!argv || !flag) return false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], flag) == 0) return true;
    }
    return false;
}

/**
 * vibe_arg_get - Gets the value following a specific flag.
 * Internal Logic: Searches for the flag and returns the subsequent element in the argv array if it exists.
 * Returns NULL if the flag is not found or is the last argument.
 */
static inline char* vibe_arg_get(int argc, char** argv, const char* flag) {
    // Internal Logic: Sanitize input pointers before processing the argument list.
    if (!argv || !flag) return NULL;
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], flag) == 0) return argv[i+1];
    }
    return NULL;
}

#endif
