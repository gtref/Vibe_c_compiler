/**
 * This header provides common math constants and helper macros for the Vibe C library.
 * In version 1.5.11, it continues to provide high-precision PI and E constants, as well as min/max macros.
 * This code is AI-generated.
 */
#ifndef VIBE_MATH_H
#define VIBE_MATH_H
#include <math.h>

// Internal Logic: Define core mathematical constants with double precision (approx. 20 decimal places).
#define VIBE_PI 3.14159265358979323846
#define VIBE_E  2.71828182845904523536

// Internal Logic: Provide simple min/max macros. Note: arguments are evaluated multiple times.
#define vibe_max(a,b) ((a) > (b) ? (a) : (b))
#define vibe_min(a,b) ((a) < (b) ? (a) : (b))

#endif
