#ifndef NXTETRIS_SRC_GAME_GAME_MODES_H_
#define NXTETRIS_SRC_GAME_GAME_MODES_H_

#include "engine/controls.h"
#include "game.h"

typedef enum {
  GAME_MODE_SP,
  GAME_MODE_MP,
  GAME_MODE_CPU
} GameMode;

typedef struct {
  GameMode game_mode;
  char *name;
} GameModeDescriptor;

static GameModeDescriptor game_modes[] = {
    {GAME_MODE_SP, "Single Player"},
    {GAME_MODE_MP, "Multi Player"},
    {GAME_MODE_CPU, "Against CPU"},
};

/**
 * Internal function used to share user input routines with multiple game modes.
 * @param game Pointer to the game.
 * @param controls Pointer to the controls handler.
 * @param is_running Pointer to a is_running state.
 * @return 1 if the user dropped dropped a piece this tick, otherwise 0.
 */
unsigned int handle_shared_game_input(Game *game, Controls *controls, unsigned int *is_running);

/**
 * Single player game mode tick handler.
 * @param game Pointer to the game.
 * @param controls Pointer to the controls handler.
 * @param data Pointer to the game data.
 */
void handle_game_mode_sp(Game *game, Controls *controls, GameDataSP *data);

/**
 * Multi player (a vs b) game mode tick handler.
 * @param game_a Pointer to the first game.
 * @param game_b Pointer to the second game.
 * @param controls Pointer to the controls handler.
 * @param data Pointer to the game data.
 */
void handle_game_mode_mp(Game *game_a, Game *game_b, Controls *controls, GameDataMP *data);

/**
 * Multi player (player vs cpu) game mode tick handler.
 * @param game_a Pointer to the first game.
 * @param game_b Pointer to the second game.
 * @param controls Pointer to the controls handler.
 * @param data Pointer to the game data.
 */
void handle_game_mode_cpu(Game *game_a, Game *game_b, Controls *controls, GameDataMP *data);

#endif
