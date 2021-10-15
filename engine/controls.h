#ifndef engine_controls_h
#define engine_controls_h

#include <stdio.h>

#include "types/point.h"

typedef struct {
    int enabled;
    int pressed_key;
    Point mouse_position;
    int mouse_state;
} Controls;

Controls* make_controls(void);

void free_controls(Controls* controls);

void update_controls(Controls* controls);

#endif /* engine_controls_h */
