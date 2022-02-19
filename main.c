#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "colorizer.h"

// "s6-rc" uses only stderr for its messages, so stdout should pass through
// without wasting time trying to colorize it

// if executed as "s6-rc-color", start s6-rc with the passed in flags and
// additionally -v2 for state change messages to be colorized
// otherwise act as colorizer in a pipeline

int main() {
    char *line = NULL; // Line string
    size_t len = 0;
    size_t read_len = 0; // Length of read-in line

    colorizer_state_t cs;
    init_colorizer_state(&cs);

    // While there are lines to read
    while((read_len = getline(&line, &len, stdin)) != -1) {
        reset_colorizer_state(&cs);

        colorizer_parse_line(line, &cs);
        colorizer_colorize_line(line, &cs);

        free(line); line = NULL;
    }
    if(line != NULL) free(line);
    reset_colorizer_state(&cs);

    return 0;
}
