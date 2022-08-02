#ifndef NXTETRIS_SRC_GAME_UI_MAIN_MENU_H_
#define NXTETRIS_SRC_GAME_UI_MAIN_MENU_H_

#include "engine/media/graphics.h"
#include "game/game_modes.h"

/**
 * Holds the state of the main menu.
 */
typedef struct {
	GameMode selected_mode; /*!< The currently selected game-mode in the main menu. */
} MainMenu;

/**
 * Creates an instance of the main menu.
 * @return Pointer to the created main menu.
 */
MainMenu* make_main_menu(void);

/**
 * Destroys an instance of the main menu.
 * @param menu Pointer to the main menu.
 */
void free_main_menu(MainMenu* menu);

/**
 * Draws the main menu to screen this frame.
 * @param graphics Pointer to the graphics manager.
 * @param menu Pointer to the main menu.
 */
void draw_main_menu(Graphics* graphics, MainMenu* menu);

#endif
