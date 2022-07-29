#ifndef NXTETRIS_SRC_GAME_GAME_MODES_H_
#define NXTETRIS_SRC_GAME_GAME_MODES_H_

#include "../engine/controls.h"
#include "game.h"

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

void handle_game_mode_sp(Game* game, Controls* controls, GameDataSP* data);

void handle_game_mode_mp(Game* game_a, Game* game_b, Controls* controls, GameDataMP* data);

void handle_game_mode_cpu(Game* game_a, Game* game_b, Controls* controls, GameDataMP* data);

#endif //NXTETRIS_SRC_GAME_GAME_MODES_H_
