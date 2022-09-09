#ifndef NXTETRIS_SRC_GAME_BOARD_H_
#define NXTETRIS_SRC_GAME_BOARD_H_

#include "engine/media/graphics.h"
#include "engine/types/point.h"
#include "engine/core/matrix.h"

#include "tetromino.h"

/**
 * A dynamic matrix of integers that contains the various displayable blocks.
 */
typedef Matrix Board;

/**
 * Draws the entire game board, relative to its bounds.
 * @param graphics Pointer to the graphics manager.
 * @param board Pointer to the board.
 * @param offset Board draw offset.
 */
void draw_board(Graphics* graphics, Board* board, Point offset);

/**
 * Adds a tetromino to a board matrix.
 * Color is obtained from the foreground.
 * @param board Pointer to the board.
 * @param tetromino Target tetromino.
 * @param position Position of the tetromino in 2d space.
 */
void add_tetromino_to_board(Board* board, Tetromino tetromino, Point position);

/**
 * Gets the first available insertion point in the board.
 * This method may become slow if used on big matrices; Use it carefully!
 * @param board Pointer to the board.
 * @param tetromino Target tetromino.
 * @param point Starting point for the intersection.
 * @return Intersected point.
 */
Point intersect_tetromino_with_board(Board* board, Tetromino tetromino, Point point);

/**
 * Removes all the filled lines in the board.
 * This should be called after piece insertion.
 * @param board Pointer to the board.
 * @return The number of lines removed.
 */
unsigned int attempt_board_line_removal(Board* board);

/**
 * Counts the number of filled lines in the board.
 * @param board Pointer to the board.
 * @return The number of filled lines.
 */
unsigned int get_filled_lines_count(Board* board);

/**
 * Inverts the values in the given range of rows.
 * Empty cells will become white, coloured cells will become empty.
 * @param board Pointer to the board.
 * @param from Starting row. (from 0)
 * @param to Ending row. (from 0)
 */
void invert_board_lines(Board* board, unsigned int from, unsigned int to);

#endif
