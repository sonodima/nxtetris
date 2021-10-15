#ifndef engine_media_graphics_h
#define engine_media_graphics_h

#include <stdio.h>

#include "../types/size.h"
#include "../types/point.h"
#include "../types/rect.h"
#include "../types/color.h"
#include "../enums/vertical_alignment.h"

typedef struct {
    Size size;
} Graphics;

Graphics* make_graphics(void);

void free_graphics(Graphics* graphics);

void begin_frame(Graphics* graphics);

void present_frame(void);

Size draw_text(Graphics* graphics, const char* text, Point point, Color color,
               VerticalAlignment alignment, int bold, int underline);

void draw_rect(Graphics* graphics, Rect rect, Color color);

Size get_window_size(void);

char* get_drawable_character(unsigned int alpha);

#endif /* engine_media_graphics_h */
