#ifndef _COLORIZER_H
#define _COLORIZER_H

#include "utils.h"

typedef struct {
    bool_t is_s6rc; // Is the current line sent by s6-rc
    uint8_t msg_type;
    bool_t is_svc;
    char *svc_name;
    uint8_t svc_action;

} colorizer_state_t ;

int reset_colorizer_state(colorizer_state_t *cs);

#endif
