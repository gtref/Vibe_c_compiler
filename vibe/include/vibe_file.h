/**
 * This header provides simple file I/O utilities for the Vibe C library with enhanced security.
 * In version 1.5.11, it features hardened file reading with size limits and OOM mitigation.
 * This code is AI-generated.
 */
#ifndef VIBE_FILE_H
#define VIBE_FILE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Internal Logic: Define a maximum file size (10MB) to prevent OOM/DoS attacks.
#define VIBE_FILE_MAX_SIZE (10 * 1024 * 1024)

/**
 * vibe_read_file - Reads the entire contents of a file into a heap-allocated buffer.
 * Internal Logic: Determines file size, enforces limits, and reads content into a null-terminated buffer.
 * Returns a null-terminated string or NULL on error or if size exceeds VIBE_FILE_MAX_SIZE.
 */
static inline char* vibe_read_file(const char* filename) {
    // Internal Logic: Sanitize input and attempt to open the file in binary mode.
    if (!filename) return NULL;
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;

    // Internal Logic: Navigate to the end of the file to determine the total length.
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }

    long len = ftell(f);
    if (len < 0) {
        fclose(f);
        return NULL;
    }

    // Sentinel: Enforce hard size limit to prevent resource exhaustion/DoS.
    if (len > VIBE_FILE_MAX_SIZE) {
        fclose(f);
        return NULL;
    }

    if (fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return NULL;
    }

    // Internal Logic: Check for potential integer overflow before allocation.
    if (len >= SIZE_MAX) {
        fclose(f);
        return NULL;
    }

    // Internal Logic: Allocate a buffer to hold the file content plus one byte for the null terminator.
    char* data = (char*)malloc((size_t)len + 1);
    if (!data) {
        fclose(f);
        return NULL;
    }

    // Internal Logic: Read the file content into the allocated buffer and verify read count.
    size_t read_bytes = fread(data, 1, (size_t)len, f);
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
