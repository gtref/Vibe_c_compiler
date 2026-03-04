/**
 * This test file verifies the ANSI color code definitions in vibe_color.h.
 * In version 1.5.11, it tests that color codes are correctly defined and produce expected output.
 * This code is AI-generated.
 */
#include <vibe_color.h>
#include <vibe_test.h>
#include <vibe_io.h>
#include <stdio.h>
#include <string.h>

/**
 * test_color_constants_defined - Verifies that all color constants are defined.
 * Internal Logic: Checks that color macros expand to non-empty strings.
 */
void test_color_constants_defined() {
    vibe_print("Testing color constants are defined...\n");

    // Verify color codes are non-NULL strings
    VIBE_ASSERT(VIBE_RED != NULL);
    VIBE_ASSERT(VIBE_GREEN != NULL);
    VIBE_ASSERT(VIBE_YELLOW != NULL);
    VIBE_ASSERT(VIBE_BLUE != NULL);
    VIBE_ASSERT(VIBE_RESET != NULL);

    // Verify they have content
    VIBE_ASSERT(strlen(VIBE_RED) > 0);
    VIBE_ASSERT(strlen(VIBE_GREEN) > 0);
    VIBE_ASSERT(strlen(VIBE_YELLOW) > 0);
    VIBE_ASSERT(strlen(VIBE_BLUE) > 0);
    VIBE_ASSERT(strlen(VIBE_RESET) > 0);
}

/**
 * test_color_codes_format - Verifies that color codes have correct ANSI escape format.
 * Internal Logic: Checks that codes start with ESC sequence.
 */
void test_color_codes_format() {
    vibe_print("Testing color codes have correct format...\n");

    // All ANSI escape codes should start with \x1b[
    VIBE_ASSERT(strncmp(VIBE_RED, "\x1b[", 2) == 0);
    VIBE_ASSERT(strncmp(VIBE_GREEN, "\x1b[", 2) == 0);
    VIBE_ASSERT(strncmp(VIBE_YELLOW, "\x1b[", 2) == 0);
    VIBE_ASSERT(strncmp(VIBE_BLUE, "\x1b[", 2) == 0);
    VIBE_ASSERT(strncmp(VIBE_RESET, "\x1b[", 2) == 0);
}

/**
 * test_color_codes_unique - Verifies that color codes are distinct.
 * Internal Logic: Ensures each color has a unique escape sequence.
 */
void test_color_codes_unique() {
    vibe_print("Testing color codes are unique...\n");

    // Each color should be different
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
 * test_color_output - Tests actual colored output formatting.
 * Internal Logic: Demonstrates that color codes can be used in output.
 */
void test_color_output() {
    vibe_print("Testing color output (visual verification)...\n");

    // Create colored strings
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%sRed Text%s", VIBE_RED, VIBE_RESET); // nosec
    VIBE_ASSERT(strlen(buffer) > 0);

    snprintf(buffer, sizeof(buffer), "%sGreen Text%s", VIBE_GREEN, VIBE_RESET); // nosec
    VIBE_ASSERT(strlen(buffer) > 0);

    snprintf(buffer, sizeof(buffer), "%sYellow Text%s", VIBE_YELLOW, VIBE_RESET); // nosec
    VIBE_ASSERT(strlen(buffer) > 0);

    snprintf(buffer, sizeof(buffer), "%sBlue Text%s", VIBE_BLUE, VIBE_RESET); // nosec
    VIBE_ASSERT(strlen(buffer) > 0);

    // Verify reset code works after colors
    snprintf(buffer, sizeof(buffer), "%sColor%sNormal", VIBE_RED, VIBE_RESET); // nosec
    VIBE_ASSERT(strlen(buffer) > 0);
}

/**
 * test_color_concatenation - Tests concatenating multiple colors.
 * Internal Logic: Verifies that color codes can be combined in strings.
 */
void test_color_concatenation() {
    vibe_print("Testing color concatenation...\n");

    char buffer[200];
    snprintf(buffer, sizeof(buffer), // nosec
             "%sError:%s %sFile not found%s",
             VIBE_RED, VIBE_RESET, VIBE_YELLOW, VIBE_RESET);

    // Verify the buffer contains all the color codes
    VIBE_ASSERT(strstr(buffer, VIBE_RED) != NULL);
    VIBE_ASSERT(strstr(buffer, VIBE_YELLOW) != NULL);
    VIBE_ASSERT(strstr(buffer, "Error:") != NULL);
    VIBE_ASSERT(strstr(buffer, "File not found") != NULL);
}

/**
 * test_color_escape_sequence_values - Tests specific escape sequence values.
 * Internal Logic: Verifies the exact ANSI codes match expected values.
 */
void test_color_escape_sequence_values() {
    vibe_print("Testing specific escape sequence values...\n");

    // Verify exact codes (based on standard ANSI colors)
    VIBE_ASSERT(strcmp(VIBE_RED, "\x1b[31m") == 0);
    VIBE_ASSERT(strcmp(VIBE_GREEN, "\x1b[32m") == 0);
    VIBE_ASSERT(strcmp(VIBE_YELLOW, "\x1b[33m") == 0);
    VIBE_ASSERT(strcmp(VIBE_BLUE, "\x1b[34m") == 0);
    VIBE_ASSERT(strcmp(VIBE_RESET, "\x1b[0m") == 0);
}

/**
 * test_color_usage_patterns - Tests common usage patterns with color codes.
 * Internal Logic: Simulates realistic logging and formatting scenarios.
 */
void test_color_usage_patterns() {
    vibe_print("Testing common usage patterns...\n");

    // Pattern 1: Error message
    char error_msg[100];
    snprintf(error_msg, sizeof(error_msg), "%s[ERROR]%s Operation failed", // nosec
             VIBE_RED, VIBE_RESET);
    VIBE_ASSERT(strlen(error_msg) > 0);

    // Pattern 2: Success message
    char success_msg[100];
    snprintf(success_msg, sizeof(success_msg), "%s[SUCCESS]%s Build completed", // nosec
             VIBE_GREEN, VIBE_RESET);
    VIBE_ASSERT(strlen(success_msg) > 0);

    // Pattern 3: Warning message
    char warning_msg[100];
    snprintf(warning_msg, sizeof(warning_msg), "%s[WARNING]%s Deprecated function", // nosec
             VIBE_YELLOW, VIBE_RESET);
    VIBE_ASSERT(strlen(warning_msg) > 0);

    // Pattern 4: Info message
    char info_msg[100];
    snprintf(info_msg, sizeof(info_msg), "%s[INFO]%s Starting process", // nosec
             VIBE_BLUE, VIBE_RESET);
    VIBE_ASSERT(strlen(info_msg) > 0);
}

int main() {
    vibe_print("=== Vibe Color Tests ===\n");

    test_color_constants_defined();
    test_color_codes_format();
    test_color_codes_unique();
    test_color_output();
    test_color_concatenation();
    test_color_escape_sequence_values();
    test_color_usage_patterns();

    VIBE_TEST_SUMMARY();
    return 0;
}