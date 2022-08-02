#ifndef NXTETRIS_SRC_GAME_GAME_MODES_H_
#define NXTETRIS_SRC_GAME_GAME_MODES_H_

#include "engine/controls.h"
#include "game.h"
#include "cpu.h"

/**
 * Game-mode identifiers.
 */
typedef enum {
  GAME_MODE_SP,    /*!< Single player game-mode. */
  GAME_MODE_MP,    /*!< Two (alternating) players game-mode. */
  GAME_MODE_CPU    /*!< Player vs CPU game-mode. */
} GameMode;

/**
 * Holds the game-mode's information.
 */
typedef struct {
  GameMode game_mode;    /*!< The identifier of the game-mode. */
  char* name;            /*!< The displayable name of the game-mode. */
} GameModeDescriptor;

/**
 * Array of game-modes and their respective title.
 */
static GameModeDescriptor game_modes[] = {
    {GAME_MODE_SP,  "Single Player"},
    {GAME_MODE_MP,  "Multi Player"},
    {GAME_MODE_CPU, "Against CPU"},
};

/**
 * Single player game-mode tick handler.
 * Handles user input and game drawing.
 * @param game Pointer to the game.
 * @param controls Pointer to the controls handler.
 * @param graphics Pointer to the graphics manager.
 */
void handle_game_mode_sp(Game* game, Controls* controls, Graphics* graphics);

/**
 * Multi player (a vs b) game-mode tick handler.
 * Handles users input, game drawing, player switch and lines inversion.
 * @param game_a Pointer to the first game.
 * @param game_b Pointer to the second game.
 * @param controls Pointer to the controls handler.
 * @param graphics Pointer to the graphics manager.
 * @param active_player Pointer to the active player id.
 */
void handle_game_mode_mp(Game* game_a, Game* game_b, Controls* controls, Graphics* graphics,
                         unsigned int* active_player);

/**
 * Multi player (player vs cpu) game-mode tick handler.
 * Handles user input, game drawing, cpu actions and lines inversion.
 * @param game_a Pointer to the first game.
 * @param game_b Pointer to the second game.
 * @param controls Pointer to the controls handler.
 * @param graphics Pointer to the graphics manager.
 * @param cpu Pointer to the cpu.
 * @param active_player Pointer to the active player id.
 */
void handle_game_mode_cpu(Game* game_a, Game* game_b, Controls* controls, Graphics* graphics, CPU* cpu,
                          unsigned int* active_player);

#endif
