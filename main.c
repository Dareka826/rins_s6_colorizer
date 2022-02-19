#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "colors.h"

int min(int a, int b) { return a < b ? a : b; }

// Simple booleans
enum {
    FALSE = 0,
    TRUE = 1
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

// current line state
struct {
    uint8_t is_s6rc;
    uint8_t msg_type;
    uint8_t is_svc;
    char *svc_name;
    uint8_t svc_action;
} state;

void reset_state() {
    state.is_s6rc = FALSE;
    state.msg_type = MSG_NONE;
    state.is_svc = FALSE;
    if(state.svc_name != NULL) free(state.svc_name);
    state.svc_name = NULL;
    state.svc_action = SVC_NONE;
}


int main() {
    char *ptr = NULL; // Line string
    char *ptr_copy = NULL; // Copy of line string
    char *token = NULL;
    int token_idx = 0;
    size_t len = 0;
    size_t read_len = 0; // Length of read-in line
    uint8_t fully_parsed = FALSE; // Are we confident that we're not losing
                                  // information after only displaying the
                                  // parsed result?

    // While there are lines to read
    while((read_len = getline(&ptr, &len, stdin)) != -1) {
        reset_state(); // Reset line parsing state
        // Create a copy of the string as it will be destroyed by strtok
        ptr_copy = (char*) malloc(strlen(ptr)+1);
        memcpy(ptr_copy, ptr, strlen(ptr)+1);

        // Parse tokens separated by ' ' and ':'
        token = strtok(ptr, " :");
        token_idx = 0;
        do {
            if(token_idx == 0) { 
                if(strcmp("s6-rc", token) == 0)
                    state.is_s6rc = TRUE;
                else break;

            } else if(state.is_s6rc == TRUE) {

                if(token_idx == 1) {
                    if(strcmp("info", token) == 0)
                        state.msg_type = MSG_INFO;

                } else if(token_idx == 2) {
                    if(state.msg_type == MSG_INFO) {
                        if(strcmp("service", token) == 0)
                            state.is_svc = TRUE;
                    }

                } else if(token_idx == 3) {
                    state.svc_name = (char*) malloc(strlen(token)+1);
                    memcpy(state.svc_name, token, strlen(token)+1);

                } else if(token_idx == 4) {
                    if(strcmp("stopping\n", token) == 0)
                        state.svc_action = SVC_STOPPING;
                    else if(strcmp("starting\n", token) == 0)
                        state.svc_action = SVC_STARTING;

                } else if(token_idx == 5) {
                    if(strcmp("stopped\n", token) == 0)
                        state.svc_action = SVC_STOPPED;
                    else if(strcmp("started\n", token) == 0)
                        state.svc_action = SVC_STARTED;
                }
            } // endif(is_s6rc == TRUE)

            token_idx++;
        } while((token = strtok(NULL, " :")));

        fully_parsed = FALSE;

        // Print colored state based on parsed information
        if(state.is_s6rc == TRUE) {
            fully_parsed = TRUE;
            printf("%s%ss6-rc%s: ", colors[COLOR_BOLD], colors[COLOR_YELLOW],
                    colors[COLOR_NORMAL]);

            if(state.msg_type == MSG_INFO) {
                printf("info: ");

                if(state.is_svc == TRUE) {
                    printf("service ");

                    if(state.svc_action == SVC_STARTING)
                        printf("%s %s%s%s...", state.svc_name, colors[COLOR_GREEN], svc_states[state.svc_action], colors[COLOR_NORMAL]);
                    else if(state.svc_action == SVC_STARTED)
                        printf("%s [  %s%s%s  ]", state.svc_name, colors[COLOR_GREEN], svc_states[state.svc_action], colors[COLOR_NORMAL]);
                    else if(state.svc_action == SVC_STOPPING)
                        printf("%s %s%s%s...", state.svc_name, colors[COLOR_RED], svc_states[state.svc_action], colors[COLOR_NORMAL]);
                    else if(state.svc_action == SVC_STOPPED)
                        printf("%s [  %s%s%s  ]", state.svc_name, colors[COLOR_RED], svc_states[state.svc_action], colors[COLOR_NORMAL]);
                    else fully_parsed = FALSE;

                } else fully_parsed = FALSE;

            } else fully_parsed = FALSE;

            putchar('\n');
        }

        if(fully_parsed == FALSE || state.is_s6rc == FALSE)
            printf("%s", ptr_copy);

        // Flush the stream with every new line for realtime updates
        fflush(stdout);

        // Destroy strings
        free(ptr_copy); ptr_copy = NULL;
        free(ptr); ptr = NULL;
    }

    return 0;
}
