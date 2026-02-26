/**
 * Simple UI and Terminal utilities for Vibe C.
 * Provides helper functions for formatted console output and basic interface elements.
 * This code is AI-generated.
 */
#ifndef VIBE_UI_H
#define VIBE_UI_H

#include "vibe_color.h"
#include <stdio.h>

/* Prints a stylized header to the terminal */
static inline void vibe_ui_header(const char* title) {
    printf("%s=== %s ===%s\n", VIBE_BLUE, title, VIBE_RESET); // nosec
}
#endif
