#ifndef NXTETRIS_ENGINE_TYPES_COLOR_H_
#define NXTETRIS_ENGINE_TYPES_COLOR_H_

#include "../enums/alpha.h"

typedef struct {
    unsigned int foreground;
    unsigned int background;
    Alpha alpha;
} Color;

#endif /* engine_types_color_h */
