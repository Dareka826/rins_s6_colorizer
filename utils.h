#ifndef _UTILS_H
#define _UTILS_H

// Simple booleans
enum {
    FALSE = 0,
    TRUE = 1
};

// Comparison functions instead of macros to avoid accidentally executing
// functions multiple times
int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

#endif
