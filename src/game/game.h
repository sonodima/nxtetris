#ifndef NXTETRIS_SRC_GAME_GAME_H_
#define NXTETRIS_SRC_GAME_GAME_H_

#include <sys/times.h>

#include "../engine/types/rect.h"
#include "../engine/media/graphics.h"
#include "../engine/controls.h"
#include "../engine/core/matrix.h"

#include "tetromino.h"
#include "board.h"

#define PER_PIECE_COUNT 20;

typedef enum {
  GAME_STATE_IDLE,
  GAME_STATE_PLACING,
  GAME_STATE_FINISHED
} GameState;

typedef enum {
  GAME_EVENT_SET_X,
  GAME_EVENT_DROP,
  GAME_EVENT_ROT_CL,
  GAME_EVENT_ROT_CC,
  GAME_EVENT_CHP_UP,
  GAME_EVENT_CHP_DN
} GameEvent;

typedef struct {
  Graphics* graphics;
  Controls* controls;
  Rect bounds;
  clock_t gravity_clock;
  GameState state;
  unsigned int score;

  Tetromino placing_piece;
  unsigned int placing_piece_x;

  Board board;
} Game;

Game* make_game(Graphics* graphics, Controls* controls, Rect bounds);

void free_game(Game* game);

/**
 * Game loop routine. All drawing should be made here.
 * @param game Pointer to the game.
 */
void tick_game(Game* game);

/**
 * Event dispatcher. All manual-input game events should get processed here.
 * @param game Pointer to the game.
 * @param event Event type.
 * @param data Optional event data.
 */
void process_game_event(Game* game, GameEvent event, void* data);

/**
 * Helper function that converts game-relative coords to absolute CLI coords.
 * @param game Pointer to the game.
 * @param point Point to convert.
 * @return Converted point.
 */
Point game_rel_to_abs(Game* game, Point point);

#endif //NXTETRIS_SRC_GAME_GAME_H_
