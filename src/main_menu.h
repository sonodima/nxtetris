#ifndef NXTETRIS_SRC_GAME_UI_MAIN_MENU_H_
#define NXTETRIS_SRC_GAME_UI_MAIN_MENU_H_

#include "engine/media/graphics.h"
#include "game/game_modes.h"

#define GAME_MODES_COUNT 3

typedef struct {
  Graphics* graphics;
  GameMode selected_mode;
} MainMenu;

MainMenu* make_main_menu(Graphics* graphics);

void free_main_menu(MainMenu* menu);

void draw_main_menu(MainMenu* menu);

#endif
