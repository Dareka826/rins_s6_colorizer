#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include "utils.h"
#include "colorizer.h"

// "s6-rc" uses only stderr for its messages, so stdout should pass through
// without wasting time trying to colorize it

// if executed as "s6-rc-color", start s6-rc with the passed in flags and
// additionally -v2 for state change messages to be colorized
// otherwise act as colorizer in a pipeline

/*
* s6-rc-color: stdin  <-------------- stdin: s6-rc
*              stdout <-------------- stdout
*              stderr <--(colorize)-- stderr
*/

extern char **environ;

#define PIPE_WRITE 1
#define PIPE_READ  0

int main(int argc, char *argv[]) {
    FILE *data_source = stdin;
    bool_t has_child = FALSE;
    pid_t pid;
    int pipe_fd[2];

    if(strcmp("s6-rc-color", basename(argv[0])) == 0) {
        has_child = TRUE;
        pipe(pipe_fd);

        pid = fork();

        // Check if child
        if(pid == 0) {
            close(pipe_fd[PIPE_READ]); // Close pipe out
            dup2(pipe_fd[PIPE_WRITE], STDERR_FILENO); // Point stderr to pipe in

#ifdef EXEC_DEBUG
            argv[0] = "./test.sh";
#else
            argv[0] = "s6-rc";
#endif
            execvp(argv[0], argv);

        } else {
            // Parent
            close(pipe_fd[PIPE_WRITE]);
            close(STDIN_FILENO);  // Make sure only the child has stdin
            close(STDOUT_FILENO); // and stdout
            data_source = fdopen(pipe_fd[PIPE_READ], "r");
        }
    }

    char *line = NULL; // Line string
    size_t len = 0;
    size_t read_len = 0; // Length of read-in line

    colorizer_state_t cs;
    init_colorizer_state(&cs);

    // While there are lines to read
    while((read_len = getline(&line, &len, data_source)) != -1) {
        reset_colorizer_state(&cs);

        colorizer_parse_line(line, &cs);
        colorizer_colorize_line(line, &cs);

        free(line); line = NULL;
    }
    if(line != NULL) free(line);
    reset_colorizer_state(&cs);

    if(has_child == TRUE) {
        fclose(data_source);
    }

    return 0;
}
