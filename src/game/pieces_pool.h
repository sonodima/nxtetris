#ifndef NXTETRIS_SRC_GAME_PIECES_POOL_H_
#define NXTETRIS_SRC_GAME_PIECES_POOL_H_

#include "tetromino.h"

/**
 * The PiecesPool keeps count of the available pieces left.
 * An instance of the PiecesPool can be shared with many games.
 */
typedef struct {
	unsigned int counts[TETROMINOES_COUNT]; /*!< The number of pieces left for each tetromino shape. */
	unsigned int count_per_piece; /*!< The number of pieces initially available for each shape. */
} PiecesPool;

/**
 * Creates the pieces manager, which keeps count of the used pieces.
 * @param count Number of pieces available for each shape.
 * @return Pointer to the created pieces pool.
 */
PiecesPool* make_pieces_pool(unsigned int count);

/**
 * Deallocates a piece pool.
 * @param pool Pointer to the piece pool.
 */
void free_pieces_pool(PiecesPool* pool);

/**
 * Resets all the available pieces in a piece pool with the given value.
 * This function is also called in the make_pieces_pool function.
 * @param pool Pointer to the pieces pool.
 * @param count Number of pieces available for each shape.
 */
void reset_pieces_pool(PiecesPool* pool, unsigned int count);

/**
 * Checks if there are available pieces to drop in the pool.
 * @param pool Pointer to the pieces pool.
 * @return 1 if there is at least one piece left, otherwise 0.
 */
unsigned int has_pieces_left(PiecesPool* pool);

/**
 * Gets the number of instances left for a piece.
 * @param pool Pointer to the pieces pool.
 * @param piece Target piece type.
 * @return Number of instances left.
 */
unsigned int get_piece_count(PiecesPool* pool, unsigned int piece);

/**
 * Checks if a given piece is still available in the pool.
 * @param pool Pointer to the pieces pool.
 * @param piece Target piece type.
 * @return 1 if there is at least one piece with this shape left, otherwise 0.
 */
unsigned int is_piece_available(PiecesPool* pool, unsigned int piece);

#endif
