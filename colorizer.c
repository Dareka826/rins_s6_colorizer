#include "colorizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "colors.h"

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

int reset_colorizer_state(colorizer_state_t *cs) {
    cs->is_s6rc = FALSE;
    cs->msg_type = MSG_NONE;
    cs->is_svc = FALSE;
    if(cs->svc_name != NULL) {
        free(cs->svc_name);
        cs->svc_name = NULL;
    }
    cs->svc_action = SVC_NONE;

    return 0; // Nothing went wrong
}


// "s6-rc: info: service _____: starting"
// "s6-rc: info: service _____ successfully started"
// "s6-rc: info: service _____: stopping"
// "s6-rc: info: service _____ successfully stopped"
void colorizer_parse_line(const char * const line) {
    colorizer_state_t cstate;
    reset_colorizer_state(&cstate);

    char *line_copy = (char*) malloc(strlen(line) + 1);
    char *token = NULL;

    // Tokenize the line
    strtok(line_copy, " :");
    int token_idx = 0;
    do {


        token_idx++;

    } while((token = strtok(line_copy, " :")) != NULL);

    free(line_copy); line_copy = NULL;
}
