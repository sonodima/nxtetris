#ifndef NXTETRIS_SRC_GAME_GAME_H_
#define NXTETRIS_SRC_GAME_GAME_H_

#include <sys/times.h>

#include "../engine/types/rect.h"
#include "../engine/media/graphics.h"
#include "../engine/controls.h"
#include "../engine/core/matrix.h"

#include "tetromino.h"
#include "board.h"

/**
 * Number of pieces available for each shape.
 * Related to the X-Tetris rules.
 */
#define PER_PIECE_COUNT 1

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
  Rect bounds;
  GameState state;
  unsigned int pieces_count[TETROMINOES_COUNT];
  unsigned int score;
  unsigned int disable_input;

  Tetromino placing_piece;
  unsigned int placing_piece_x;

  Board* board;
} Game;

/**
 * Creates an instance of the game.
 * @param graphics Pointer to the graphics manager.
 * @param bounds Rectangle in which the game is drawn.
 * @return Pointer to the created game.
 */
Game* make_game(Graphics* graphics, Rect bounds);

/**
 * Destroys an instance of the game.
 * @param game Pointer to the game.
 */
void free_game(Game* game);

/**
 * Draws a rectangle outside the game bounds, the score and the current piece's count.
 * @param game Pointer to the game.
 */
void draw_game_bounds(Game* game);

/**
 * Sets the placing_piece to a random tetromino.
 * @param game Pointer to the game.
 */
void initialize_placing_piece(Game* game);

/**
 * Game loop routine. All drawing should be made here.
 * @param game Pointer to the game.
 */
void tick_game(Game* game);

/**
 * Internal routine that drops a piece in the board and handles board update.
 * @param game Pointer to the game.
 */
void drop_piece(Game* game);

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

/**
 * Checks if there are available pieces to drop in the game.
 * @param game Pointer to the game.
 * @return 1 if there is at least one piece left, otherwise 0.
 */
unsigned int has_pieces_left(Game* game);

/**
 * Checks if a given piece is still available in the game.
 * @param game Pointer to the game.
 * @param piece Target piece type.
 * @return 1 if there is at least one piece with this shape left, otherwise 0.
 */
unsigned int is_piece_available(Game* game, unsigned int piece);

/**
 * Handles removed lines count -> gained points conversion.
 * @param count Number of lines removed.
 * @return Gained points for this action.
 */
unsigned int removed_lines_to_points(unsigned int count);

#endif //NXTETRIS_SRC_GAME_GAME_H_
