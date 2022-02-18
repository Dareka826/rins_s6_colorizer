#ifndef _COLORS_H
#define _COLORS_H

enum {
    COLOR_RED = 0,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_BOLD,
    COLOR_NORMAL
};

const char * const colors[] = { 
    "\033[31m",
    "\033[32m",
    "\033[33m",
    "\033[34m",
    "\033[35m",
    "\033[36m",
    "\033[1m",
    "\033[0m"
};

#endif
