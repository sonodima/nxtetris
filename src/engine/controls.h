#ifndef NXTETRIS_SRC_ENGINE_CONTROLS_H_
#define NXTETRIS_SRC_ENGINE_CONTROLS_H_

#include "types/point.h"

typedef struct {
  int enabled;
  int pressed_key;
  Point mouse_position;
  int mouse_state;
} Controls;

/**
 * Allocates the con
 * @return
 */
Controls* make_controls(void);

void free_controls(Controls* controls);

void update_controls(Controls* controls);

#endif
