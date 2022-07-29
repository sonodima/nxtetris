#ifndef NXTETRIS_SRC_GAME_UI_MAIN_MENU_H_
#define NXTETRIS_SRC_GAME_UI_MAIN_MENU_H_

#include "../../engine/media/graphics.h"

#define GAME_MODES_COUNT 3

typedef enum {
  GAME_MODE_SP,
  GAME_MODE_MP,
  GAME_MODE_CPU
} GameMode;

typedef struct {
  GameMode game_mode;
  char* name;
} GameModeDescriptor;

static GameModeDescriptor game_modes[] = {
    { GAME_MODE_SP, "Single Player" },
    { GAME_MODE_MP, "Multi Player" },
    { GAME_MODE_CPU, "Against CPU" },
};

typedef struct {
  Graphics* graphics;
  GameMode selected_mode;
} MainMenu;

MainMenu* make_main_menu(Graphics* graphics);

void draw_main_menu(MainMenu* menu);

#endif //NXTETRIS_SRC_GAME_UI_MAIN_MENU_H_
