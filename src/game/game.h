#ifndef NXTETRIS_SRC_GAME_GAME_H_
#define NXTETRIS_SRC_GAME_GAME_H_

#include <sys/times.h>

#include "engine/types/rect.h"
#include "engine/media/graphics.h"
#include "engine/controls.h"
#include "engine/core/matrix.h"

#include "tetromino.h"
#include "board.h"
#include "pieces_pool.h"

/**
 * Execution state of the game.
 */
typedef enum {
	GAME_STATE_IDLE, /*!< The game just finished an action and is reverting to the GAME_STATE_PLACING state. */
	GAME_STATE_PLACING, /*!< The player is placing the temporary preview piece. */
	GAME_STATE_FINISHED /*!< The game ended because either one player placed a piece outside the board or the pieces pool ran out of pieces. */
} GameState;

typedef enum {
	GAME_EVENT_SET_X,
	GAME_EVENT_DROP,
	GAME_EVENT_ROT_CL,
	GAME_EVENT_ROT_CC,
	GAME_EVENT_SET_ROT,
	GAME_EVENT_CHP_UP,
	GAME_EVENT_CHP_DN,
	GAME_EVENT_SET_CHP
} GameEvent;

/**
 * A game handler, that holds its state and score.
 * Many game instances can be spawned and handled independently from one another.
 * Pieces sharing can be done through the pieces pool.
 */
typedef struct {
	PiecesPool* pieces_pool; /*!< Pointer to the object that manages the available pieces. */
	Rect bounds; /*!< Size and position of the game on screen. */
	Board* board; /*!< Pointer to the board matrix. */

	GameState state; /*!< Current state of the game. */
	unsigned int score; /*!< Total score of the game. */
	unsigned int last_removed_lines; /*!< The amount of lines removed in the last action. */
	unsigned int finished_for_overflow; /*!< 1 if the game ended because one player could not place a piece in the board. */
	unsigned int disable_input; /*!< Keeps the 'active' status of the game. */

	Tetromino placing_piece; /*!< Temporary preview tetromino displayed at the top of the board. */
	unsigned int placing_piece_x; /*!< Current x-axis offset for tetromino placement. */
} Game;

/**
 * Creates an instance of the game.
 * @param pieces_pool Pointer to the pool that contains the remaining pieces.
 * @param bounds Rectangle in which the game is drawn.
 * @return Pointer to the created game.
 */
Game* make_game(PiecesPool* pieces_pool, Rect bounds);

/**
 * Destroys an instance of the game.
 * @param game Pointer to the game.
 */
void free_game(Game* game);

/**
 * Clears the state of the game.
 * @param game Pointer to the game.
 */
void reset_game(Game* game);

/**
 * Game loop routine. All drawing should be made here.
 * @param game Pointer to the game.
 * @param graphics Pointer to the graphics manager.
 */
void tick_game(Game* game, Graphics* graphics);

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
 * Handles removed lines count -> gained points conversion.
 * @param count Number of lines removed.
 * @return Gained points for this action.
 */
unsigned int removed_lines_to_points(unsigned int count);

/**
 * Gets the next available tetromino in the game.
 * @param game Pointer to the game.
 * @return -1 if no piece is available, otherwise the next available tetromino.
 */
int get_next_available_tetromino(Game* game);

/**
 * Gets the previous available tetromino in the game.
 * @param game Pointer to the game.
 * @return -1 if no piece is available, otherwise the previous available tetromino.
 */
int get_previous_available_tetromino(Game* game);

#endif
