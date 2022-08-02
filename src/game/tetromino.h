#ifndef NXTETRIS_SRC_GAME_TETROMINO_H_
#define NXTETRIS_SRC_GAME_TETROMINO_H_

#include "engine/types/size.h"
#include "engine/types/rect.h"
#include "engine/types/color.h"
#include "engine/types/point.h"
#include "engine/media/graphics.h"
#include "engine/core/matrix.h"

/**
 * Number of tetromino shapes.
 */
#define TETROMINOES_COUNT 7

/**
 * Number of tetromino shape rotations.
 */
#define TETROMINOES_ROTATIONS 4

/**
 * Pre-calculated state of a tetromino. (in a specific rotation)
 */
typedef struct {
  unsigned short data; /*!< State data. (parsed to bits) */
  Size size; /*!< Size of the tetromino state. */
} TetrominoState;

/**
 * Tetromino object that can be displayed and inserted in a board.
 */
typedef struct {
  unsigned short shape; /*!< Shape of the tetromino. */
  unsigned short rotation; /*!< Rotation of the tetromino. */
  Color color; /*!< Display color of the tetromino. */
} Tetromino;

/*
 * Tetrominoes can get represented with a 16x16 matrix that only needs to contain [1;0].
 * We can store them in a single uint16_t.
 *
 * Starting from the least significant bit (horizontal flip), we can consider a piece as:
 *    bbbb  -> [FIRST_ROW]  [4..1]
 *    bbbb  -> [SECOND_ROW] [4..1]
 *    bbbb  -> [THIRD_ROW]  [4..1]
 *    bbbb  -> [FOURTH_ROW] [4..1]
 */
static TetrominoState tetrominoes[TETROMINOES_COUNT][TETROMINOES_ROTATIONS] = {
    {
        /* I */
        {0x000F, {1, 4}}, {0x1111, {4, 1}}, {0x000F, {1, 4}}, {0x1111, {4, 1}},
    },
    {
        /* J */
        {0x0071, {2, 3}}, {0x0113, {3, 2}}, {0x0047, {2, 3}}, {0x0322, {3, 2}},
    },
    {
        /* L */
        {0x0074, {2, 3}}, {0x0311, {3, 2}}, {0x0017, {2, 3}}, {0x0223, {3, 2}},
    },
    {
        /* O */
        {0x0033, {2, 2}}, {0x0033, {2, 2}}, {0x0033, {2, 2}}, {0x0033, {2, 2}},
    },
    {
        /* S */
        {0x0036, {2, 3}}, {0x0231, {3, 2}}, {0x0036, {2, 3}}, {0x0231, {3, 2}},
    },
    {
        /* T */
        {0x0072, {2, 3}}, {0x0131, {3, 2}}, {0x0027, {2, 3}}, {0x0232, {3, 2}},
    },
    {
        /* Z */
        {0x0063, {2, 3}}, {0x0132, {3, 2}}, {0x0063, {2, 3}}, {0x0132, {3, 2}},
    },
};

/**
 * Obtains the state of a tetromino, containing its data and pre-computed size.
 * @param tetromino Target tetromino.
 * @return The state of the tetromino.
 */
TetrominoState get_tetromino_state(Tetromino tetromino);

/**
 * Obtains the value of a specified point in a tetromino.
 * @param tetromino Target tetromino.
 * @param x X-axis coordinate.
 * @param y Y-axis coordinate.
 * @return 1 if the tetromino is filled at the specified coordinates, otherwise 0.
 */
unsigned int get_tetromino_value_at(Tetromino tetromino, unsigned int x, unsigned int y);

/**
 * Checks if a tetromino is correctly defined.
 * @param tetromino Tetromino to check.
 * @return 1 if the tetromino is valid, otherwise 0.
 */
unsigned int is_tetromino_valid(Tetromino tetromino);

/**
 * Draws a tetromino to screen.
 * @param graphics Pointer to the graphics manager.
 * @param tetromino Tetromino to draw.
 * @param position Draw coordinates.
 */
void draw_tetromino(Graphics* graphics, Tetromino tetromino, Point position);

#endif
