/**
 * Core I/O macros for Vibe C, providing simple and hardened printing functions.
 * It enforces compile-time string literal checks to prevent format string injection vulnerabilities.
 * This code is AI-generated.
 */
#ifndef VIBE_IO_H
#define VIBE_IO_H

#include <stdio.h>

/* SENTINEL: Hardened printing macros that enforce string literal format strings to prevent injection */
#define vibe_print(fmt, ...) printf("" fmt, ##__VA_ARGS__) // nosec
#define vibe_error(fmt, ...) fprintf(stderr, "" fmt, ##__VA_ARGS__) // nosec

#endif
