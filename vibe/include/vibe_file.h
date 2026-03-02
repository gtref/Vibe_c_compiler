/**
 * This header provides simple file I/O utilities for the Vibe C library.
 * In version 1.5.9, it includes a robust file reading function with automatic null-termination and enhanced error checks.
 * This code is AI-generated.
 */
#ifndef VIBE_FILE_H
#define VIBE_FILE_H
#include <stdio.h>
#include <stdlib.h>

/**
 * vibe_read_file - Reads the entire contents of a file into a heap-allocated buffer.
 * Internal Logic: Determines file size using fseek/ftell, allocates memory, and reads the content.
 * Returns a null-terminated string or NULL on error.
 */
static inline char* vibe_read_file(const char* filename) {
    // Internal Logic: Sanitize input and attempt to open the file in binary mode for portability.
    if (!filename) return NULL;
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;

    // Internal Logic: Navigate to the end of the file to determine the total length in bytes.
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    if (len < 0) {
        fclose(f);
        return NULL;
    }
    fseek(f, 0, SEEK_SET);

    // Internal Logic: Allocate a buffer to hold the file content plus one byte for the null terminator.
    char* data = (char*)malloc(len + 1);
    if (!data) {
        fclose(f);
        return NULL;
    }

    // Internal Logic: Read the file content into the allocated buffer and verify the number of bytes read.
    size_t read_bytes = fread(data, 1, len, f);
    if (read_bytes < (size_t)len) {
        free(data);
        fclose(f);
        return NULL;
    }

    // Internal Logic: Explicitly null-terminate the buffer and close the file handle.
    data[len] = '\0';
    fclose(f);
    return data;
}

#endif
