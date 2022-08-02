#ifndef NXTETRIS_SRC_GAME_UI_END_SCREEN_H
#define NXTETRIS_SRC_GAME_UI_END_SCREEN_H

#include "engine/media/graphics.h"

/**
 * Draws a simple popup that should be displayed at game end.
 * @param graphics Pointer to the graphics manager.
 * @param sub_message Secondary label.
 */
void draw_end_screen(Graphics* graphics, const char* sub_message);

#endif
