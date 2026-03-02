/**
 * This header provides simple terminal User Interface (UI) helpers for the Vibe C library.
 * In version 1.5.9, it uses Vibe color constants to format and print colorized section headers to stdout.
 * This code is AI-generated.
 */
#ifndef VIBE_UI_H
#define VIBE_UI_H
#include "vibe_color.h"
#include <stdio.h>

/**
 * vibe_ui_header - Prints a colorized section header to the terminal.
 * Internal Logic: Wraps the provided title in blue ANSI escape codes for enhanced terminal visibility.
 */
static inline void vibe_ui_header(const char* title) {
    // Internal Logic: Robustly handle NULL titles and output colorized header markers to the standard output.
    if (!title) return;
    printf("%s=== %s ===%s\n", VIBE_BLUE, title, VIBE_RESET); // nosec
}

#endif
