/**
 * Comprehensive tests for vibe_arg.h command-line argument parsing utilities.
 * In version 1.5.11, this test verifies flag detection and value retrieval functionality.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_arg.h"
#include "../vibe/include/vibe_test.h"
#include <stdio.h>

/**
 * test_arg_has_basic - Tests basic flag presence detection.
 * Internal Logic: Verifies that vibe_arg_has correctly identifies flags in argv.
 */
void test_arg_has_basic() {
    char* argv[] = {"program", "--verbose", "--output", "file.txt", NULL};
    int argc = 4;

    VIBE_ASSERT(vibe_arg_has(argc, argv, "--verbose") == true);
    VIBE_ASSERT(vibe_arg_has(argc, argv, "--output") == true);
    VIBE_ASSERT(vibe_arg_has(argc, argv, "file.txt") == true);
    VIBE_ASSERT(vibe_arg_has(argc, argv, "--missing") == false);
}

/**
 * test_arg_has_null_safety - Tests NULL pointer safety for vibe_arg_has.
 * Internal Logic: Ensures function returns false when given NULL inputs.
 */
void test_arg_has_null_safety() {
    char* argv[] = {"program", "--flag", NULL};
    int argc = 2;

    VIBE_ASSERT(vibe_arg_has(argc, NULL, "--flag") == false);
    VIBE_ASSERT(vibe_arg_has(argc, argv, NULL) == false);
    VIBE_ASSERT(vibe_arg_has(argc, NULL, NULL) == false);
}

/**
 * test_arg_has_empty_args - Tests behavior with minimal arguments.
 * Internal Logic: Verifies handling of argc=1 (only program name).
 */
void test_arg_has_empty_args() {
    char* argv[] = {"program", NULL};
    int argc = 1;

    VIBE_ASSERT(vibe_arg_has(argc, argv, "--anything") == false);
}

/**
 * test_arg_get_basic - Tests basic value retrieval for flags.
 * Internal Logic: Verifies that vibe_arg_get returns the value following a flag.
 */
void test_arg_get_basic() {
    char* argv[] = {"program", "--name", "test", "--count", "42", NULL};
    int argc = 5;

    char* name = vibe_arg_get(argc, argv, "--name");
    VIBE_ASSERT(name != NULL);
    VIBE_ASSERT(strcmp(name, "test") == 0);

    char* count = vibe_arg_get(argc, argv, "--count");
    VIBE_ASSERT(count != NULL);
    VIBE_ASSERT(strcmp(count, "42") == 0);
}

/**
 * test_arg_get_missing_flag - Tests retrieval of non-existent flags.
 * Internal Logic: Ensures NULL is returned when flag is not found.
 */
void test_arg_get_missing_flag() {
    char* argv[] = {"program", "--existing", "value", NULL};
    int argc = 3;

    char* result = vibe_arg_get(argc, argv, "--missing");
    VIBE_ASSERT(result == NULL);
}

/**
 * test_arg_get_last_flag - Tests behavior when flag is the last argument.
 * Internal Logic: Should return NULL since there's no value after the flag.
 */
void test_arg_get_last_flag() {
    char* argv[] = {"program", "--flag", NULL};
    int argc = 2;

    char* result = vibe_arg_get(argc, argv, "--flag");
    VIBE_ASSERT(result == NULL);
}

/**
 * test_arg_get_null_safety - Tests NULL pointer safety for vibe_arg_get.
 * Internal Logic: Ensures function returns NULL when given NULL inputs.
 */
void test_arg_get_null_safety() {
    char* argv[] = {"program", "--flag", "value", NULL};
    int argc = 3;

    VIBE_ASSERT(vibe_arg_get(argc, NULL, "--flag") == NULL);
    VIBE_ASSERT(vibe_arg_get(argc, argv, NULL) == NULL);
    VIBE_ASSERT(vibe_arg_get(argc, NULL, NULL) == NULL);
}

/**
 * test_arg_get_multiple_flags - Tests handling of multiple flags with values.
 * Internal Logic: Verifies correct value retrieval when multiple flags are present.
 */
void test_arg_get_multiple_flags() {
    char* argv[] = {
        "program",
        "--input", "in.txt",
        "--output", "out.txt",
        "--verbose",
        "--level", "3",
        NULL
    };
    int argc = 8;

    VIBE_ASSERT(strcmp(vibe_arg_get(argc, argv, "--input"), "in.txt") == 0);
    VIBE_ASSERT(strcmp(vibe_arg_get(argc, argv, "--output"), "out.txt") == 0);
    VIBE_ASSERT(strcmp(vibe_arg_get(argc, argv, "--verbose"), "--level") == 0);
    VIBE_ASSERT(strcmp(vibe_arg_get(argc, argv, "--level"), "3") == 0);
}

/**
 * test_arg_has_first_position - Tests flag detection in first argument position.
 * Internal Logic: Ensures argv[1] is correctly checked.
 */
void test_arg_has_first_position() {
    char* argv[] = {"program", "--first", "--second", NULL};
    int argc = 3;

    VIBE_ASSERT(vibe_arg_has(argc, argv, "--first") == true);
    VIBE_ASSERT(vibe_arg_has(argc, argv, "program") == false);
}

/**
 * test_arg_edge_cases - Tests various edge cases for argument parsing.
 * Internal Logic: Covers special characters, spaces in values, and exact matches.
 */
void test_arg_edge_cases() {
    char* argv[] = {"prog", "-v", "--file=test.txt", "--", "regular_arg", NULL};
    int argc = 5;

    VIBE_ASSERT(vibe_arg_has(argc, argv, "-v") == true);
    VIBE_ASSERT(vibe_arg_has(argc, argv, "--file=test.txt") == true);
    VIBE_ASSERT(vibe_arg_has(argc, argv, "--file") == false); // Exact match required
    VIBE_ASSERT(vibe_arg_has(argc, argv, "--") == true);
    VIBE_ASSERT(vibe_arg_has(argc, argv, "regular_arg") == true);
}

/**
 * test_arg_get_with_equals - Tests that flags with embedded values are handled.
 * Internal Logic: Verifies behavior when using --flag=value syntax.
 */
void test_arg_get_with_equals() {
    char* argv[] = {"program", "--config=file.conf", "--next", "value", NULL};
    int argc = 4;

    // vibe_arg_get looks for exact match and returns next arg
    char* config = vibe_arg_get(argc, argv, "--config=file.conf");
    VIBE_ASSERT(config != NULL);
    VIBE_ASSERT(strcmp(config, "--next") == 0);

    // Looking for just --config won't match
    VIBE_ASSERT(vibe_arg_get(argc, argv, "--config") == NULL);
}

int main() {
    printf("Testing vibe_arg.h functionality...\n"); /* nosec */

    test_arg_has_basic();
    test_arg_has_null_safety();
    test_arg_has_empty_args();
    test_arg_get_basic();
    test_arg_get_missing_flag();
    test_arg_get_last_flag();
    test_arg_get_null_safety();
    test_arg_get_multiple_flags();
    test_arg_has_first_position();
    test_arg_edge_cases();
    test_arg_get_with_equals();

    VIBE_TEST_SUMMARY();
    return 0;
}