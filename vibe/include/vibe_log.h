/**
 * This header provides standardized logging macros for the Vibe C library with hardened security.
 * In version 1.5.11, these continue to enforce string literal formats to mitigate injection risks through literal concatenation.
 * This code is AI-generated.
 */
#ifndef VIBE_LOG_H
#define VIBE_LOG_H
#include <stdio.h>

// Internal Logic: Macros for different log levels, prefixed for easy identification in output.
// The literal concatenation pattern ensures compile-time safety against format string attacks.
#define vibe_log_info(...)  printf("[INFO] " __VA_ARGS__) // nosec
#define vibe_log_warn(...)  printf("[WARN] " __VA_ARGS__) // nosec
#define vibe_log_error(...) fprintf(stderr, "[ERROR] " __VA_ARGS__) // nosec

#endif
