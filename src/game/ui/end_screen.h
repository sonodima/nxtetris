#ifndef NXTETRIS_SRC_GAME_UI_END_SCREEN_H
#define NXTETRIS_SRC_GAME_UI_END_SCREEN_H

#include "engine/media/graphics.h"
#include "game/game.h"
#include "game/game_modes.h"

/**
 * Draws a simple popup that should be displayed at game end.
 * @param graphics Pointer to the graphics manager.
 * @param sub_message Secondary label, drawn in the end screen popup.
 */
void draw_end_screen(Graphics* graphics, const char* sub_message);

/**
 * Writes the end message to a string buffer.
 * The buffer should be at least 30 bytes long.
 * @param game_mode The current game mode.
 * @param game_a Pointer to the first game.
 * @param game_b Pointer to the second game.
 * @param buffer Output buffer.
 */
void get_end_message(GameMode game_mode, Game* game_a, Game* game_b, char* buffer);

#endif
