/**
 * This test file verifies the newly implemented security hardening features, including vibe_str_copy and vibe_snprintf.
 * In version 1.5.11, it ensures that these primitives provide guaranteed null-termination and format string safety.
 * This code is AI-generated.
 */
#include "../vibe/include/vibe_string.h"
#include "../vibe/include/vibe_io.h"
#include "../vibe/include/vibe_test.h"
#include <string.h>

/**
 * Run sentinel hardening tests for vibe_str_copy and vibe_snprintf.
 *
 * Executes a suite of runtime assertions that verify:
 * - vibe_str_copy copies strings correctly, truncates to fit buffers, and always null-terminates.
 * - vibe_str_copy treats a NULL source as an empty string.
 * - vibe_snprintf produces correct formatted output, truncates when necessary, and null-terminates.
 * The function prints a test header and finalizes the test run via VIBE_TEST_SUMMARY().
 *
 * @returns Process exit status (0 on success, non-zero if any test failed).
 */
int main() {
    vibe_print("--- Vibe Sentinel Hardening Tests ---\n");

    // Internal Logic: Test vibe_str_copy basic functionality for identical string replication.
    char buf1[10];
    vibe_str_copy(buf1, "Hello", sizeof(buf1));
    VIBE_ASSERT_STR_EQ(buf1, "Hello");

    // Internal Logic: Verify vibe_str_copy truncation behavior and mandatory null-termination for oversized inputs.
    char buf2[5];
    vibe_str_copy(buf2, "LongString", sizeof(buf2));
    VIBE_ASSERT_STR_EQ(buf2, "Long"); // Should be "Long" + \0
    VIBE_ASSERT_EQ(buf2[4], '\0');

    // Internal Logic: Ensure vibe_str_copy handles NULL source pointers gracefully by creating an empty string.
    char buf3[10] = "Existing";
    vibe_str_copy(buf3, NULL, sizeof(buf3));
    VIBE_ASSERT_STR_EQ(buf3, "");

    // Internal Logic: Test vibe_snprintf for correct formatted output and integer conversion.
    char buf4[20];
    vibe_snprintf(buf4, sizeof(buf4), "Value: %d", 42);
    VIBE_ASSERT_STR_EQ(buf4, "Value: 42");

    // Internal Logic: Verify vibe_snprintf truncation and null-termination when the output exceeds buffer capacity.
    char buf5[5];
    vibe_snprintf(buf5, sizeof(buf5), "123456789");
    VIBE_ASSERT_STR_EQ(buf5, "1234");
    VIBE_ASSERT_EQ(buf5[4], '\0');

    VIBE_TEST_SUMMARY();
}
