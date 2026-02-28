/**
 * This header provides simple file I/O utilities for the Vibe C library.
 * In version 1.5.8, it continues to feature a robust file reading function with automatic null-termination.
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
    if (!filename) return NULL;
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;

    // Internal Logic: Navigate to the end of the file to find its length.
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    if (len < 0) {
        fclose(f);
        return NULL;
    }
    fseek(f, 0, SEEK_SET);

    // Internal Logic: Allocate buffer for file content plus null terminator.
    char* data = (char*)malloc(len + 1);
    if (!data) {
        fclose(f);
        return NULL;
    }

    size_t read_bytes = fread(data, 1, len, f);
    if (read_bytes < (size_t)len) {
        free(data);
        fclose(f);
        return NULL;
    }

    data[len] = '\0';
    fclose(f);
    return data;
}

#endif
