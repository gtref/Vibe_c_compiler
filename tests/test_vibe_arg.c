/**
 * This test file verifies the command-line argument parsing utilities in vibe_arg.h.
 * In version 1.5.11, it tests flag detection, value retrieval, and NULL pointer safety.
 * This code is AI-generated.
 */
#include <vibe_arg.h>
#include <vibe_test.h>
#include <vibe_io.h>
#include <string.h>

/**
 * test_arg_has_basic - Tests basic flag detection functionality.
 * Internal Logic: Verifies that vibe_arg_has correctly identifies present and absent flags.
 */
void test_arg_has_basic() {
    vibe_print("Testing vibe_arg_has basic functionality...\n");

    char* argv[] = {"program", "--flag", "--another", "value"};
    int argc = 4;

    // Test flag that exists
    VIBE_ASSERT(vibe_arg_has(argc, argv, "--flag") == true);
    VIBE_ASSERT(vibe_arg_has(argc, argv, "--another") == true);

    // Test flag that doesn't exist
    VIBE_ASSERT(vibe_arg_has(argc, argv, "--missing") == false);
    VIBE_ASSERT(vibe_arg_has(argc, argv, "--fla") == false);

    // Test value is not treated as flag
    VIBE_ASSERT(vibe_arg_has(argc, argv, "value") == true);
}

/**
 * test_arg_has_null_safety - Tests NULL pointer handling in vibe_arg_has.
 * Internal Logic: Ensures the function safely returns false for NULL inputs.
 */
void test_arg_has_null_safety() {
    vibe_print("Testing vibe_arg_has NULL safety...\n");

    char* argv[] = {"program", "--flag"};
    int argc = 2;

    // Test NULL argv
    VIBE_ASSERT(vibe_arg_has(argc, NULL, "--flag") == false);

    // Test NULL flag
    VIBE_ASSERT(vibe_arg_has(argc, argv, NULL) == false);

    // Test both NULL
    VIBE_ASSERT(vibe_arg_has(argc, NULL, NULL) == false);
}

/**
 * test_arg_has_edge_cases - Tests edge cases for flag detection.
 * Internal Logic: Verifies behavior with empty arguments and exact matches.
 */
void test_arg_has_edge_cases() {
    vibe_print("Testing vibe_arg_has edge cases...\n");

    // Test with only program name
    char* argv1[] = {"program"};
    VIBE_ASSERT(vibe_arg_has(1, argv1, "--flag") == false);

    // Test with empty string flag
    char* argv2[] = {"program", ""};
    VIBE_ASSERT(vibe_arg_has(2, argv2, "") == true);

    // Test case sensitivity
    char* argv3[] = {"program", "--Flag"};
    VIBE_ASSERT(vibe_arg_has(2, argv3, "--flag") == false);
    VIBE_ASSERT(vibe_arg_has(2, argv3, "--Flag") == true);
}

/**
 * test_arg_get_basic - Tests basic value retrieval functionality.
 * Internal Logic: Verifies that vibe_arg_get returns the value following a flag.
 */
void test_arg_get_basic() {
    vibe_print("Testing vibe_arg_get basic functionality...\n");

    char* argv[] = {"program", "--name", "testapp", "--port", "8080"};
    int argc = 5;

    // Test getting values
    char* name = vibe_arg_get(argc, argv, "--name");
    VIBE_ASSERT(name != NULL);
    VIBE_ASSERT(strcmp(name, "testapp") == 0);

    char* port = vibe_arg_get(argc, argv, "--port");
    VIBE_ASSERT(port != NULL);
    VIBE_ASSERT(strcmp(port, "8080") == 0);

    // Test missing flag
    VIBE_ASSERT(vibe_arg_get(argc, argv, "--missing") == NULL);
}

/**
 * test_arg_get_null_safety - Tests NULL pointer handling in vibe_arg_get.
 * Internal Logic: Ensures the function safely returns NULL for invalid inputs.
 */
void test_arg_get_null_safety() {
    vibe_print("Testing vibe_arg_get NULL safety...\n");

    char* argv[] = {"program", "--flag", "value"};
    int argc = 3;

    // Test NULL argv
    VIBE_ASSERT(vibe_arg_get(argc, NULL, "--flag") == NULL);

    // Test NULL flag
    VIBE_ASSERT(vibe_arg_get(argc, argv, NULL) == NULL);

    // Test both NULL
    VIBE_ASSERT(vibe_arg_get(argc, NULL, NULL) == NULL);
}

/**
 * test_arg_get_edge_cases - Tests edge cases for value retrieval.
 * Internal Logic: Verifies behavior when flag is last argument or values are empty.
 */
void test_arg_get_edge_cases() {
    vibe_print("Testing vibe_arg_get edge cases...\n");

    // Test flag as last argument (no value)
    char* argv1[] = {"program", "--flag"};
    VIBE_ASSERT(vibe_arg_get(2, argv1, "--flag") == NULL);

    // Test getting empty string value
    char* argv2[] = {"program", "--flag", ""};
    char* val = vibe_arg_get(3, argv2, "--flag");
    VIBE_ASSERT(val != NULL);
    VIBE_ASSERT(strcmp(val, "") == 0);

    // Test multiple flags with same name (should return first match)
    char* argv3[] = {"program", "--flag", "first", "--flag", "second"};
    char* result = vibe_arg_get(5, argv3, "--flag");
    VIBE_ASSERT(result != NULL);
    VIBE_ASSERT(strcmp(result, "first") == 0);
}

/**
 * test_arg_get_value_types - Tests retrieval of different value types.
 * Internal Logic: Verifies that various argument formats are correctly retrieved.
 */
void test_arg_get_value_types() {
    vibe_print("Testing vibe_arg_get with different value types...\n");

    char* argv[] = {
        "program",
        "--string", "hello world",
        "--number", "42",
        "--path", "/usr/local/bin",
        "--negative", "-123"
    };
    int argc = 9;

    // Test string with space (note: shell would split this)
    char* str = vibe_arg_get(argc, argv, "--string");
    VIBE_ASSERT(str != NULL);
    VIBE_ASSERT(strcmp(str, "hello world") == 0);

    // Test numeric string
    char* num = vibe_arg_get(argc, argv, "--number");
    VIBE_ASSERT(num != NULL);
    VIBE_ASSERT(strcmp(num, "42") == 0);

    // Test path
    char* path = vibe_arg_get(argc, argv, "--path");
    VIBE_ASSERT(path != NULL);
    VIBE_ASSERT(strcmp(path, "/usr/local/bin") == 0);

    // Test negative number
    char* neg = vibe_arg_get(argc, argv, "--negative");
    VIBE_ASSERT(neg != NULL);
    VIBE_ASSERT(strcmp(neg, "-123") == 0);
}

/**
 * test_arg_combined_usage - Tests using both functions together.
 * Internal Logic: Simulates realistic command-line parsing scenarios.
 */
void test_arg_combined_usage() {
    vibe_print("Testing combined usage of vibe_arg_has and vibe_arg_get...\n");

    char* argv[] = {"program", "--verbose", "--output", "result.txt", "--count", "10"};
    int argc = 6;

    // Check if verbose flag is present
    if (vibe_arg_has(argc, argv, "--verbose")) {
        VIBE_ASSERT(true);
    } else {
        VIBE_ASSERT(false);
    }

    // Get output file if flag is present
    if (vibe_arg_has(argc, argv, "--output")) {
        char* output = vibe_arg_get(argc, argv, "--output");
        VIBE_ASSERT(output != NULL);
        VIBE_ASSERT(strcmp(output, "result.txt") == 0);
    }

    // Get count value
    char* count = vibe_arg_get(argc, argv, "--count");
    VIBE_ASSERT(count != NULL);
    VIBE_ASSERT(strcmp(count, "10") == 0);

    // Check for missing optional flag
    VIBE_ASSERT(vibe_arg_has(argc, argv, "--debug") == false);
}

int main() {
    vibe_print("=== Vibe Argument Parsing Tests ===\n");

    test_arg_has_basic();
    test_arg_has_null_safety();
    test_arg_has_edge_cases();
    test_arg_get_basic();
    test_arg_get_null_safety();
    test_arg_get_edge_cases();
    test_arg_get_value_types();
    test_arg_combined_usage();

    VIBE_TEST_SUMMARY();
    return 0;
}