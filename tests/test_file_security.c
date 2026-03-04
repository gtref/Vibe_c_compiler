/**
 * This test verifies the security hardening of the vibe_read_file function.
 * In version 1.5.11, it checks that the function correctly handles NULL inputs, oversized files, and robust error paths.
 * vibe_read_file enforces a VIBE_FILE_MAX_SIZE (10MB) to prevent OOM/DoS.
 * This code is AI-generated.
 */
#include "vibe_file.h"
#include "vibe_test.h"
#include "vibe_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Run security-focused tests for the file-reading utility.
 *
 * Exercises error and edge cases to verify correct handling of:
 * - NULL filename input
 * - non-existent files
 * - normal small files (verifies correct content is returned)
 * - oversized files (verifies files larger than 10MB are rejected)
 *
 * The test creates and removes temporary files ("test_normal.txt" and
 * "test_oversized.txt") as needed.
 */
void test_file_security() {
    vibe_ui_header("File I/O Security Tests");

    // Test 1: NULL filename
    VIBE_ASSERT(vibe_read_file(NULL) == NULL);
    printf("  [+] Passed: NULL filename handled correctly.\n"); /* nosec */

    // Test 2: Non-existent file
    VIBE_ASSERT(vibe_read_file("non_existent_file_xyz.txt") == NULL);
    printf("  [+] Passed: Non-existent file handled correctly.\n"); /* nosec */

    // Test 3: Normal small file
    const char* filename = "test_normal.txt";
    FILE* f = fopen(filename, "w");
    if (f) {
        fprintf(f, "Hello Vibe!"); /* nosec */
        fclose(f);

        char* data = vibe_read_file(filename);
        VIBE_ASSERT(data != NULL);
        VIBE_ASSERT(strcmp(data, "Hello Vibe!") == 0);
        free(data);
        remove(filename);
        printf("  [+] Passed: Normal small file read correctly.\n"); /* nosec */
    }

    // Test 4: Oversized file (simulated by creating a file larger than 10MB)
    // We'll create a 11MB file to trigger the VIBE_FILE_MAX_SIZE limit.
    const char* large_filename = "test_oversized.txt";
    f = fopen(large_filename, "wb");
    if (f) {
        char buffer[1024];
        memset(buffer, 'A', 1024);
        for (int i = 0; i < 11 * 1024; i++) { // 11MB
            fwrite(buffer, 1, 1024, f);
        }
        fclose(f);

        char* data = vibe_read_file(large_filename);
        VIBE_ASSERT(data == NULL); // Should be rejected due to size limit
        remove(large_filename);
        printf("  [+] Passed: Oversized file (11MB) rejected as expected.\n"); /* nosec */
    } else {
        printf("  [!] Skip: Could not create large test file.\n"); /* nosec */
    }
}

int main() {
    test_file_security();
    VIBE_TEST_SUMMARY();
    return 0;
}
