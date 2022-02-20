#include "colorizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum {
    COL_RED = 0,
    COL_GREEN,
    COL_YELLOW,
    COL_BLUE,
    COL_MAGENTA,
    COL_CYAN,
    COL_BOLD,
    COL_NORMAL
};

const char * const _c[] = { 
    "\033[31m",
    "\033[32m",
    "\033[33m",
    "\033[34m",
    "\033[35m",
    "\033[36m",
    "\033[1m",
    "\033[0m"
};

// s6-rc message types
enum {
    MSG_NONE,
    MSG_INFO
};

// s6-rc service state change types
enum {
    SVC_NONE,
    SVC_STARTING,
    SVC_STOPPING,
    SVC_STARTED,
    SVC_STOPPED
};

// strings for s6-rc state change types
const char * const svc_states[] = {
    "???",
    "starting",
    "stopping",
    "started",
    "stopped"
};

void init_colorizer_state(colorizer_state_t * const cs) {
    cs->svc_name = NULL;
    reset_colorizer_state(cs);
}

void reset_colorizer_state(colorizer_state_t * const cs) {
    cs->is_s6rc = FALSE;
    cs->msg_type = MSG_NONE;
    cs->is_svc = FALSE;
    if(cs->svc_name != NULL) {
        free(cs->svc_name);
        cs->svc_name = NULL;
    }
    cs->svc_action = SVC_NONE;
}

void colorizer_parse_line(const char * const line, colorizer_state_t * const cs) {
    // Create a copy of line for strtok()
    char *line_copy = (char*) malloc(strlen(line)+1);
    memcpy(line_copy, line, strlen(line)+1);

    char *token = NULL;
    int token_idx = 0;

    // Tokenize the line
    token = strtok(line_copy, " :");
    do {
        if(token_idx == 0) {
            if(strcmp("s6-rc", token) == 0)
                cs->is_s6rc = TRUE;
            else break; // No point in parsing more

        } else if(cs->is_s6rc == TRUE) {

            if(token_idx == 1) {
                if(strcmp("info", token) == 0)
                    cs->msg_type = MSG_INFO;

            } else if(token_idx == 2) {
                if(cs->msg_type == MSG_INFO) {
                    if(strcmp("service", token) == 0)
                        cs->is_svc = TRUE;
                }

            } else if(token_idx == 3) {
                cs->svc_name = (char*) malloc(strlen(token)+1);
                memcpy(cs->svc_name, token, strlen(token)+1);

            } else if(token_idx == 4) {
                if(strcmp("stopping\n", token) == 0)
                    cs->svc_action = SVC_STOPPING;
                else if(strcmp("starting\n", token) == 0)
                    cs->svc_action = SVC_STARTING;

            } else if(token_idx == 5) {
                if(strcmp("stopped\n", token) == 0)
                    cs->svc_action = SVC_STOPPED;
                else if(strcmp("started\n", token) == 0)
                    cs->svc_action = SVC_STARTED;
            }
        }

        token_idx++;

    } while((token = strtok(NULL, " :")) != NULL);

    // Destroy the line copy
    free(line_copy); line_copy = NULL;
}

void colorizer_colorize_line(const char * const line, const colorizer_state_t * const cs) {
    bool_t is_fully_parsed = TRUE;

    if(cs->is_s6rc == TRUE) {
        fprintf(stderr, "%s%ss6-rc%s: ", _c[COL_BOLD], _c[COL_YELLOW], _c[COL_NORMAL]);

        if(cs->msg_type == MSG_INFO) {
            fprintf(stderr, "info: ");

            if(cs->is_svc == TRUE) {
                fprintf(stderr, "service ");

                if(cs->svc_action == SVC_STARTING)
                    fprintf(stderr, "%s %s%s%s...", cs->svc_name, _c[COL_GREEN], svc_states[cs->svc_action], _c[COL_NORMAL]);
                else if(cs->svc_action == SVC_STARTED)
                        fprintf(stderr, "%s [  %s%s%s  ]", cs->svc_name, _c[COL_GREEN], svc_states[cs->svc_action], _c[COL_NORMAL]);
                else if(cs->svc_action == SVC_STOPPING)
                        fprintf(stderr, "%s %s%s%s...", cs->svc_name, _c[COL_RED], svc_states[cs->svc_action], _c[COL_NORMAL]);
                else if(cs->svc_action == SVC_STOPPED)
                        fprintf(stderr, "%s [  %s%s%s  ]", cs->svc_name, _c[COL_RED], svc_states[cs->svc_action], _c[COL_NORMAL]);
                else is_fully_parsed = FALSE;

            } else is_fully_parsed = FALSE;

        } else is_fully_parsed = FALSE;

        putc('\n', stderr);
    }

    // If not s6-rc or not sure the program reflected all relevant information
    if(cs->is_s6rc == FALSE || is_fully_parsed == FALSE)
        fprintf(stderr, "%s", line);

    // Flush the stream with every new line for realtime updates
    fflush(stderr);
}
