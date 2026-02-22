/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the utility functions for file I/O operations.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_FILE_H
#define VIBE_FILE_H
#include <stdio.h>
#include <stdlib.h>
/**
 * vibe_read_file - Reads the entire contents of a file into a buffer.
 * @filename: The path to the file to read
 *
 * This function opens the file, determines its size, allocates a buffer,
 * and reads the content. The returned buffer is null-terminated.
 * The caller is responsible for freeing the returned memory.
 * Returns NULL on failure (e.g., file not found, memory allocation failed).
 */
static inline char* vibe_read_file(const char* filename) {
    if (!filename) return NULL;
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    if (len < 0) {
        fclose(f);
        return NULL;
    }
    fseek(f, 0, SEEK_SET);
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
