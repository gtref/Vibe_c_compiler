/**
 * Comprehensive tests for vibe_color.h ANSI color code definitions.
 * In version 1.5.11, this test verifies color constants are properly defined.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_color.h"
#include "../vibe/include/vibe_test.h"
#include <stdio.h>
#include <string.h>

/**
 * test_color_constants_defined - Tests that all color constants are defined.
 * Internal Logic: Verifies that color macros are non-NULL strings.
 */
void test_color_constants_defined() {
    VIBE_ASSERT(VIBE_RED != NULL);
    VIBE_ASSERT(VIBE_GREEN != NULL);
    VIBE_ASSERT(VIBE_YELLOW != NULL);
    VIBE_ASSERT(VIBE_BLUE != NULL);
    VIBE_ASSERT(VIBE_RESET != NULL);
}

/**
 * test_color_ansi_format - Tests that colors use proper ANSI escape sequences.
 * Internal Logic: Verifies that color strings start with ESC sequence.
 */
void test_color_ansi_format() {
    // All ANSI color codes should start with "\x1b["
    VIBE_ASSERT(strncmp(VIBE_RED, "\x1b[", 2) == 0);
    VIBE_ASSERT(strncmp(VIBE_GREEN, "\x1b[", 2) == 0);
    VIBE_ASSERT(strncmp(VIBE_YELLOW, "\x1b[", 2) == 0);
    VIBE_ASSERT(strncmp(VIBE_BLUE, "\x1b[", 2) == 0);
    VIBE_ASSERT(strncmp(VIBE_RESET, "\x1b[", 2) == 0);
}

/**
 * test_color_unique_codes - Tests that different colors have different codes.
 * Internal Logic: Ensures each color constant is unique.
 */
void test_color_unique_codes() {
    VIBE_ASSERT(strcmp(VIBE_RED, VIBE_GREEN) != 0);
    VIBE_ASSERT(strcmp(VIBE_RED, VIBE_YELLOW) != 0);
    VIBE_ASSERT(strcmp(VIBE_RED, VIBE_BLUE) != 0);
    VIBE_ASSERT(strcmp(VIBE_RED, VIBE_RESET) != 0);

    VIBE_ASSERT(strcmp(VIBE_GREEN, VIBE_YELLOW) != 0);
    VIBE_ASSERT(strcmp(VIBE_GREEN, VIBE_BLUE) != 0);
    VIBE_ASSERT(strcmp(VIBE_GREEN, VIBE_RESET) != 0);

    VIBE_ASSERT(strcmp(VIBE_YELLOW, VIBE_BLUE) != 0);
    VIBE_ASSERT(strcmp(VIBE_YELLOW, VIBE_RESET) != 0);

    VIBE_ASSERT(strcmp(VIBE_BLUE, VIBE_RESET) != 0);
}

/**
 * test_color_string_usage - Tests using colors in string concatenation.
 * Internal Logic: Verifies colors can be used in print statements.
 */
void test_color_string_usage() {
    char buffer[256];

    // Test red color
    snprintf(buffer, sizeof(buffer), "%sRed Text%s", VIBE_RED, VIBE_RESET); /* nosec */
    VIBE_ASSERT(strlen(buffer) > 8); // "Red Text" + ANSI codes

    // Test green color
    snprintf(buffer, sizeof(buffer), "%sGreen Text%s", VIBE_GREEN, VIBE_RESET); /* nosec */
    VIBE_ASSERT(strlen(buffer) > 10);

    // Test yellow color
    snprintf(buffer, sizeof(buffer), "%sYellow Text%s", VIBE_YELLOW, VIBE_RESET); /* nosec */
    VIBE_ASSERT(strlen(buffer) > 11);

    // Test blue color
    snprintf(buffer, sizeof(buffer), "%sBlue Text%s", VIBE_BLUE, VIBE_RESET); /* nosec */
    VIBE_ASSERT(strlen(buffer) > 9);
}

/**
 * test_color_reset_code - Tests that VIBE_RESET is the standard reset sequence.
 * Internal Logic: Verifies VIBE_RESET matches expected ANSI reset code.
 */
void test_color_reset_code() {
    VIBE_ASSERT(strcmp(VIBE_RESET, "\x1b[0m") == 0);
}

/**
 * test_specific_color_codes - Tests that colors match expected ANSI codes.
 * Internal Logic: Verifies each color has the correct ANSI escape sequence.
 */
void test_specific_color_codes() {
    VIBE_ASSERT(strcmp(VIBE_RED, "\x1b[31m") == 0);
    VIBE_ASSERT(strcmp(VIBE_GREEN, "\x1b[32m") == 0);
    VIBE_ASSERT(strcmp(VIBE_YELLOW, "\x1b[33m") == 0);
    VIBE_ASSERT(strcmp(VIBE_BLUE, "\x1b[34m") == 0);
}

/**
 * test_color_concatenation - Tests concatenating multiple colors.
 * Internal Logic: Verifies colors can be combined in one string.
 */
void test_color_concatenation() {
    char buffer[512];

    snprintf(buffer, sizeof(buffer), /* nosec */
             "%sRed%s %sGreen%s %sYellow%s %sBlue%s",
             VIBE_RED, VIBE_RESET,
             VIBE_GREEN, VIBE_RESET,
             VIBE_YELLOW, VIBE_RESET,
             VIBE_BLUE, VIBE_RESET);

    VIBE_ASSERT(strlen(buffer) > 20); // Base text + all ANSI codes
    VIBE_ASSERT(strstr(buffer, "Red") != NULL);
    VIBE_ASSERT(strstr(buffer, "Green") != NULL);
    VIBE_ASSERT(strstr(buffer, "Yellow") != NULL);
    VIBE_ASSERT(strstr(buffer, "Blue") != NULL);
}

/**
 * test_color_visual_output - Prints colored text for visual verification.
 * Internal Logic: Outputs colored text to terminal for manual inspection.
 */
void test_color_visual_output() {
    printf("\n--- Visual Color Test (check terminal output) ---\n"); /* nosec */
    printf("%sThis text should be RED%s\n", VIBE_RED, VIBE_RESET); /* nosec */
    printf("%sThis text should be GREEN%s\n", VIBE_GREEN, VIBE_RESET); /* nosec */
    printf("%sThis text should be YELLOW%s\n", VIBE_YELLOW, VIBE_RESET); /* nosec */
    printf("%sThis text should be BLUE%s\n", VIBE_BLUE, VIBE_RESET); /* nosec */
    printf("This text should be normal (no color)\n"); /* nosec */
    printf("--- End Visual Test ---\n\n"); /* nosec */

    VIBE_ASSERT(true); // Visual test always passes
}

/**
 * test_color_length - Tests that color codes have non-zero length.
 * Internal Logic: Verifies all color strings are valid (non-empty).
 */
void test_color_length() {
    VIBE_ASSERT(strlen(VIBE_RED) > 0);
    VIBE_ASSERT(strlen(VIBE_GREEN) > 0);
    VIBE_ASSERT(strlen(VIBE_YELLOW) > 0);
    VIBE_ASSERT(strlen(VIBE_BLUE) > 0);
    VIBE_ASSERT(strlen(VIBE_RESET) > 0);
}

/**
 * test_color_no_newlines - Tests that color codes don't contain newlines.
 * Internal Logic: Ensures color strings are single-line escape sequences.
 */
void test_color_no_newlines() {
    VIBE_ASSERT(strchr(VIBE_RED, '\n') == NULL);
    VIBE_ASSERT(strchr(VIBE_GREEN, '\n') == NULL);
    VIBE_ASSERT(strchr(VIBE_YELLOW, '\n') == NULL);
    VIBE_ASSERT(strchr(VIBE_BLUE, '\n') == NULL);
    VIBE_ASSERT(strchr(VIBE_RESET, '\n') == NULL);
}

int main() {
    printf("Testing vibe_color.h functionality...\n"); /* nosec */

    test_color_constants_defined();
    test_color_ansi_format();
    test_color_unique_codes();
    test_color_string_usage();
    test_color_reset_code();
    test_specific_color_codes();
    test_color_concatenation();
    test_color_length();
    test_color_no_newlines();

    // Visual test - run last
    test_color_visual_output();

    VIBE_TEST_SUMMARY();
    return 0;
}