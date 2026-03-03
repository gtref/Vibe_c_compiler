/**
 * This test verifies the security hardening of the file I/O utilities.
 * It checks for file size limits, NULL pointer safety, and robust error handling.
 * This code is AI-generated.
 */
#define VIBE_FILE_MAX_SIZE 1024
#include <vibe_file.h>
#include <vibe_io.h>
#include <vibe_test.h>
#include <stdlib.h>
#include <unistd.h>

void test_oversized_file() {
    vibe_print("Testing oversized file rejection (Limit: 1024 bytes)...\n");
    const char* filename = "oversized_test.bin";
    FILE* f = fopen(filename, "wb");
    if (!f) return;

    // Create a file of 1025 bytes
    char* buf = (char*)calloc(1, 1025);
    fwrite(buf, 1, 1025, f);
    fclose(f);
    free(buf);

    char* data = vibe_read_file(filename);
    VIBE_ASSERT(data == NULL);

    unlink(filename);
}

void test_null_file() {
    vibe_print("Testing NULL filename safety...\n");
    VIBE_ASSERT(vibe_read_file(NULL) == NULL);
}

void test_valid_file_read() {
    vibe_print("Testing valid file reading...\n");
    const char* filename = "valid_test.txt";
    const char* content = "Vibe Secure I/O Test";
    FILE* f = fopen(filename, "w");
    if (!f) return;
    fputs(content, f);
    fclose(f);

    char* data = vibe_read_file(filename);
    VIBE_ASSERT(data != NULL);
    if (data) {
        VIBE_ASSERT_STR_EQ(data, content);
        free(data);
    }

    unlink(filename);
}

int main() {
    test_oversized_file();
    test_null_file();
    test_valid_file_read();
    VIBE_TEST_SUMMARY();
}
