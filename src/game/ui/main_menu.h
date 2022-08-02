#ifndef NXTETRIS_SRC_GAME_UI_MAIN_MENU_H_
#define NXTETRIS_SRC_GAME_UI_MAIN_MENU_H_

#include "engine/media/graphics.h"
#include "game/game_modes.h"

#define GAME_MODES_COUNT 3

/**
 * Holds the current game-mode selection and a pointer to the graphics manager.
 */
typedef struct {
  Graphics* graphics;
  GameMode selected_mode;
} MainMenu;

/**
 * Creates an instance of the main menu.
 * @param graphics Pointer to the graphics manager.
 * @return Pointer to the created main menu.
 */
MainMenu* make_main_menu(Graphics* graphics);

/**
 * Destroys an instance of the main menu.
 * @param menu Pointer to the main menu.
 */
void free_main_menu(MainMenu* menu);

/**
 * Draws the main menu to screen this frame.
 * @param menu Pointer to the main menu.
 */
void draw_main_menu(MainMenu* menu);

#endif
