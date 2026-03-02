/**
 * This header provides basic I/O macros for the Vibe C library with compile-time security hardening.
 * In version 1.5.9, it continues to enforce string literal format strings to prevent injection vulnerabilities at the source.
 * This code is AI-generated.
 */
#ifndef VIBE_IO_H
#define VIBE_IO_H
#include <stdio.h>

// Internal Logic: Use "" __VA_ARGS__ to force the first argument to be a string literal.
// This prevents format string vulnerabilities by causing a compilation error if a variable is passed as the format.
#define vibe_print(fmt, ...) printf("" fmt, ##__VA_ARGS__) // nosec
#define vibe_error(fmt, ...) fprintf(stderr, "" fmt, ##__VA_ARGS__) // nosec

#endif
