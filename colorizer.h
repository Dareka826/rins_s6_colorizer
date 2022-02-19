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

void init_colorizer_state(colorizer_state_t * const cs);
// Reset the information about a line
void reset_colorizer_state(colorizer_state_t * const cs);
// Parse line for colorizer_colorize_line()
void colorizer_parse_line(const char * const line, colorizer_state_t * const cs);
// Print the colorized line
void colorizer_colorize_line(const char * const line, const colorizer_state_t * const cs);

#endif
