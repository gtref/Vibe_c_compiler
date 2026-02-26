/**
 * File I/O utilities for Vibe C.
 * Provides easy-to-use functions for reading entire files into memory buffers.
 * This code is AI-generated.
 */
#ifndef VIBE_FILE_H
#define VIBE_FILE_H

#include <stdio.h>
#include <stdlib.h>

/* Reads the entire content of a file and returns it as a null-terminated string */
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
