#ifndef NXTETRIS_ENGINE_CONTROLS_H_
#define NXTETRIS_ENGINE_CONTROLS_H_

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

#endif /* NXTETRIS_ENGINE_CONTROLS_H_ */
