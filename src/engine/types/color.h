#ifndef NXTETRIS_SRC_ENGINE_TYPES_COLOR_H_
#define NXTETRIS_SRC_ENGINE_TYPES_COLOR_H_

#include "../enums/alpha.h"

typedef struct {
  unsigned int foreground;
  unsigned int background;
  Alpha alpha;
} Color;

#endif //NXTETRIS_SRC_ENGINE_TYPES_COLOR_H_