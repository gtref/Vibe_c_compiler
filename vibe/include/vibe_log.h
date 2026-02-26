/**
 * Hardened logging macros for Vibe C.
 * Enforces compile-time format string literal checks to prevent format string vulnerabilities in log messages.
 * This code is AI-generated.
 */
#ifndef VIBE_LOG_H
#define VIBE_LOG_H

#include <stdio.h>

/* SENTINEL: Hardened logging macros with level prefix and format string enforcement */
#define vibe_log_info(...)  printf("[INFO] " __VA_ARGS__) // nosec
#define vibe_log_warn(...)  printf("[WARN] " __VA_ARGS__) // nosec
#define vibe_log_error(...) fprintf(stderr, "[ERROR] " __VA_ARGS__) // nosec

#endif
