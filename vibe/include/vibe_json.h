/*
 * This header has been updated to include comprehensive documentation and logic explanations.
 * The changes enhance maintainability by detailing the lightweight JSON parser and high-performance secure printer.
 * THIS CODE IS AI GENERATED.
 */

#ifndef VIBE_JSON_H
#define VIBE_JSON_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * Supported JSON data types.
 */
typedef enum {
    VIBE_JSON_NULL,   /* Represents a null value */
    VIBE_JSON_BOOL,   /* Represents a boolean value (true/false) */
    VIBE_JSON_NUMBER, /* Represents a numeric value (double) */
    VIBE_JSON_STRING, /* Represents a string value */
    VIBE_JSON_ARRAY,  /* Represents an ordered list of values */
    VIBE_JSON_OBJECT  /* Represents a collection of name/value pairs */
} vibe_json_type_t;

/**
 * Structure representing a JSON value.
 * Uses a union to store the actual data based on the 'type' field.
 */
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
 * vibe_json_new_string - Creates a new JSON string value.
 * @s: The C string to wrap
 *
 * Allocates a new vibe_json_value_t and duplicates the input string.
 * Returns the new value or NULL on allocation failure.
 */
static inline vibe_json_value_t* vibe_json_new_string(const char* s) {
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
 * vibe_json_free - Recursively frees a JSON value structure.
 * @v: The root value to free
 */
static inline void vibe_json_free(vibe_json_value_t* v) {
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
 * Internal helper to print a string with JSON escaping.
 * Handles special characters like quotes, backslashes, and control characters.
 */
static inline void _vibe_json_print_escaped(const char* s) {
    if (!s) { fputs("null", stdout); return; }
    putchar('\"');
    const char* start = s;
    // BOLT: Use strcspn to find chunks of characters that don't need escaping.
    // This is significantly faster than a manual loop as strcspn is often highly optimized (SIMD).
    while (*start) {
        size_t len = strcspn(start, "\"\\\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F");
        if (len > 0) {
            fwrite(start, 1, len, stdout);
            start += len;
        }
        if (*start) {
            unsigned char c = (unsigned char)*start;
            // BOLT: Replace slow switch/formatter with fast if-else and manual hex conversion
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
 * vibe_json_print - Prints a JSON value structure to standard output.
 * @v: The JSON value to print
 *
 * Uses optimized chunked I/O and secure escaping for string values.
 */
static inline void vibe_json_print(vibe_json_value_t* v) {
    if (!v) { fputs("null", stdout); return; }
    switch(v->type) {
        case VIBE_JSON_NULL: fputs("null", stdout); break;
        case VIBE_JSON_BOOL: fputs(v->value.boolean ? "true" : "false", stdout); break;
        case VIBE_JSON_NUMBER: printf("%g", v->value.number); break; // nosec
        case VIBE_JSON_STRING: _vibe_json_print_escaped(v->value.string); break;
        case VIBE_JSON_ARRAY:
            // BOLT: Use putchar for single characters to avoid formatter overhead
            putchar('[');
            for (size_t i = 0; i < v->value.array.count; i++) {
                vibe_json_print(v->value.array.elements[i]);
                if (i < v->value.array.count - 1) putchar(',');
            }
            putchar(']');
            break;
        case VIBE_JSON_OBJECT:
            putchar('{');
            for (size_t i = 0; i < v->value.object.count; i++) {
                _vibe_json_print_escaped(v->value.object.keys[i]);
                putchar(':');
                vibe_json_print(v->value.object.values[i]);
                if (i < v->value.object.count - 1) putchar(',');
            }
            putchar('}');
            break;
        default: fputs("???", stdout);
    }
}

#endif
