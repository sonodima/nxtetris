#ifndef NXTETRIS_SRC_GAME_GAME_MODES_H_
#define NXTETRIS_SRC_GAME_GAME_MODES_H_

#include "engine/controls.h"
#include "game.h"
#include "cpu.h"

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
    {GAME_MODE_SP,  "Single Player"},
    {GAME_MODE_MP,  "Multi Player"},
    {GAME_MODE_CPU, "Against CPU"},
};

/**
 * Internal function used to share user input routines with multiple game modes.
 * @param game Pointer to the game.
 * @param controls Pointer to the controls handler.
 * @return 1 if the user dropped dropped a piece this tick, otherwise 0.
 */
unsigned int handle_shared_game_input(Game* game, Controls* controls);

/**
 * Single player game mode tick handler.
 * Handles user input and game drawing.
 * @param game Pointer to the game.
 * @param controls Pointer to the controls handler.
 */
void handle_game_mode_sp(Game* game, Controls* controls);

/**
 * Multi player (a vs b) game mode tick handler.
 * Handles users input, game drawing, player switch and lines inversion.
 * @param game_a Pointer to the first game.
 * @param game_b Pointer to the second game.
 * @param controls Pointer to the controls handler.
 * @param active_player Pointer to the active player id.
 */
void handle_game_mode_mp(Game* game_a, Game* game_b, Controls* controls, unsigned int* active_player);

/**
 * Multi player (player vs cpu) game mode tick handler.
 * Handles user input, game drawing, cpu actions and lines inversion.
 * @param game_a Pointer to the first game.
 * @param game_b Pointer to the second game.
 * @param controls Pointer to the controls handler.
 * @param cpu Pointer to the cpu.
 * @param active_player Pointer to the active player id.
 */
void handle_game_mode_cpu(Game* game_a, Game* game_b, Controls* controls, CPU* cpu, unsigned int* active_player);

#endif
