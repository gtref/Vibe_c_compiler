/**
 * This header implements high-performance and secure JSON serialization for the Vibe C library.
 * In version 1.5.9, it features recursive depth tracking, secure character escaping, and hardened printing macros.
 * This code is AI-generated.
 */
#ifndef VIBE_JSON_H
#define VIBE_JSON_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "vibe_io.h"

// Sentinel: Default maximum depth for recursive JSON printing to mitigate stack overflow DoS.
#ifndef VIBE_JSON_MAX_DEPTH
#define VIBE_JSON_MAX_DEPTH 128
#endif

typedef enum {
    VIBE_JSON_NULL,
    VIBE_JSON_BOOL,
    VIBE_JSON_NUMBER,
    VIBE_JSON_STRING,
    VIBE_JSON_ARRAY,
    VIBE_JSON_OBJECT
} vibe_json_type_t;

typedef struct vibe_json_value {
    vibe_json_type_t type;
    union {
        bool boolean;
        double number;
        char* string;
        struct {
            struct vibe_json_value** elements;
            size_t count;
        } array;
        struct {
            char** keys;
            struct vibe_json_value** values;
            size_t count;
        } object;
    } value;
} vibe_json_value_t;

/**
 * vibe_json_new_string - Creates a new JSON string value
 */
static inline vibe_json_value_t* vibe_json_new_string(const char* s) {
    // Internal Logic: Sanitize input and allocate memory for the JSON value structure and its string content.
    if (!s) return NULL;
    vibe_json_value_t* v = (vibe_json_value_t*)malloc(sizeof(vibe_json_value_t));
    if (!v) return NULL;
    v->type = VIBE_JSON_STRING;
    v->value.string = strdup(s);
    if (!v->value.string) {
        free(v);
        return NULL;
    }
    return v;
}

/**
 * vibe_json_free - Recursively frees a JSON value structure
 */
static inline void vibe_json_free(vibe_json_value_t* v) {
    // Internal Logic: Recursively traverse the JSON tree to free all allocated memory for strings, arrays, and objects.
    if (!v) return;
    if (v->type == VIBE_JSON_STRING) {
        free(v->value.string);
    } else if (v->type == VIBE_JSON_ARRAY) {
        for (size_t i = 0; i < v->value.array.count; i++) {
            vibe_json_free(v->value.array.elements[i]);
        }
        free(v->value.array.elements);
    } else if (v->type == VIBE_JSON_OBJECT) {
        for (size_t i = 0; i < v->value.object.count; i++) {
            free(v->value.object.keys[i]);
            vibe_json_free(v->value.object.values[i]);
        }
        free(v->value.object.keys);
        free(v->value.object.values);
    }
    free(v);
}

/**
 * _vibe_json_print_escaped - Internal helper for secure string escaping
 */
static inline void _vibe_json_print_escaped(const char* s) {
    // Internal Logic: Implement JSON-compliant escaping for quotes, backslashes, and control characters (U+0000 to U+001F).
    if (!s) { fputs("null", stdout); return; }
    putchar('\"');
    const char* start = s;
    // BOLT: Use strcspn to find chunks of characters that don't need escaping for high-performance scanning.
    while (*start) {
        size_t len = strcspn(start, "\"\\\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F");
        if (len > 0) {
            fwrite(start, 1, len, stdout);
            start += len;
        }
        if (*start) {
            unsigned char c = (unsigned char)*start;
            // BOLT: Replace slow switch/formatter with fast if-else and manual hex conversion for control characters.
            if (c == '\"') fputs("\\\"", stdout);
            else if (c == '\\') fputs("\\\\", stdout);
            else if (c == '\b') fputs("\\b", stdout);
            else if (c == '\f') fputs("\\f", stdout);
            else if (c == '\n') fputs("\\n", stdout);
            else if (c == '\r') fputs("\\r", stdout);
            else if (c == '\t') fputs("\\t", stdout);
            else {
                static const char hex[] = "0123456789abcdef";
                char buf[6] = {'\\', 'u', '0', '0', hex[c >> 4], hex[c & 0x0f]};
                fwrite(buf, 1, 6, stdout);
            }
            start++;
        }
    }
    putchar('\"');
}

/**
 * _vibe_json_print_recursive - Internal helper for depth-tracked printing
 */
static inline void _vibe_json_print_recursive(vibe_json_value_t* v, int depth) {
    // Sentinel: Depth tracking to prevent stack overflow DoS by enforcing VIBE_JSON_MAX_DEPTH.
    if (!v || depth > VIBE_JSON_MAX_DEPTH) {
        fputs("null", stdout);
        return;
    }

    switch(v->type) {
        case VIBE_JSON_NULL: fputs("null", stdout); break;
        case VIBE_JSON_BOOL: fputs(v->value.boolean ? "true" : "false", stdout); break;
        case VIBE_JSON_NUMBER: {
            double n = v->value.number;
            // BOLT: Handle NaN and Infinity by outputting null to ensure standard-compliant JSON output.
            if (isnan(n) || isinf(n)) {
                fputs("null", stdout);
            }
            // BOLT: Fast path for integers to avoid slow floating-point formatter overhead.
            else if (n >= -9e18 && n <= 9e18 && n == (long long)n) {
                vibe_print("%lld", (long long)n);
            } else {
                vibe_print("%g", n);
            }
            break;
        }
        case VIBE_JSON_STRING: _vibe_json_print_escaped(v->value.string); break;
        case VIBE_JSON_ARRAY:
            // BOLT: Use putchar for single characters to minimize I/O overhead.
            putchar('[');
            // BOLT: Optimized loop that removes internal conditional branches to improve CPU prediction.
            if (v->value.array.count > 0) {
                for (size_t i = 0; i < v->value.array.count - 1; i++) {
                    _vibe_json_print_recursive(v->value.array.elements[i], depth + 1);
                    putchar(',');
                }
                _vibe_json_print_recursive(v->value.array.elements[v->value.array.count - 1], depth + 1);
            }
            putchar(']');
            break;
        case VIBE_JSON_OBJECT:
            putchar('{');
            // BOLT: Optimized loop with tail processing to eliminate conditional comma printing.
            if (v->value.object.count > 0) {
                for (size_t i = 0; i < v->value.object.count - 1; i++) {
                    _vibe_json_print_escaped(v->value.object.keys[i]);
                    putchar(':');
                    _vibe_json_print_recursive(v->value.object.values[i], depth + 1);
                    putchar(',');
                }
                _vibe_json_print_escaped(v->value.object.keys[v->value.object.count - 1]);
                putchar(':');
                _vibe_json_print_recursive(v->value.object.values[v->value.object.count - 1], depth + 1);
            }
            putchar('}');
            break;
        default: fputs("???", stdout);
    }
}

/**
 * vibe_json_print - Prints a JSON value to stdout with security depth tracking
 */
static inline void vibe_json_print(vibe_json_value_t* v) {
    // Internal Logic: Entry point for JSON printing, initializing depth tracking at zero.
    _vibe_json_print_recursive(v, 0);
}

#endif
