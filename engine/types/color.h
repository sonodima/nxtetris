#ifndef engine_types_color_h
#define engine_types_color_h

#include "../enums/alpha.h"
#include "../enums/color_module.h"

typedef struct {
    ColorModule foreground;
    ColorModule background;
    Alpha alpha;
} Color;

#endif /* engine_types_color_h */
