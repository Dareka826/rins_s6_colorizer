#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

// Simple booleans
typedef uint8_t bool_t; // We only need 1 bit, so choose the smallest int type
enum {
    FALSE = 0,
    TRUE = 1
};

// Comparison functions instead of macros to avoid accidentally executing
// functions multiple times
int min(int a, int b);
int max(int a, int b);

#endif
