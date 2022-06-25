#ifndef NXTETRIS_SRC_GAME_BOARD_H_
#define NXTETRIS_SRC_GAME_BOARD_H_

#include "../engine/media/graphics.h"
#include "../engine/types/point.h"
#include "../engine/core/matrix.h"

#include "tetromino.h"

/**
 * Draws the entire game board, relative to its bounds.
 * @param game Pointer to the game.
 */
void draw_board(Graphics* graphics, Matrix* board, Point offset);

/**
 * Adds a tetromino to a board matrix.
 * Color is obtained from the foreground.
 * @param board Pointer to the board matrix.
 * @param tetromino Target tetromino.
 * @param position Position of the tetromino in 2d space.
 */
void add_tetromino_to_board(Matrix *board, Tetromino tetromino, Point position);

/**
 *
 * @param board
 * @param tetromino
 * @param x_offset
 * @return
 */
int intersect_tetromino_with_board(Matrix *board, Tetromino tetromino, int x_offset);

/**
 * @param board Pointer to the board matrix.
 * @return The number of lines removed.
 */
int attempt_board_line_removal(Matrix* board);

#endif //NXTETRIS_SRC_GAME_BOARD_H_
