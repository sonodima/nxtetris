#ifndef NXTETRIS_SRC_GAME_TETROMINO_H_
#define NXTETRIS_SRC_GAME_TETROMINO_H_

#include "../engine/types/rect.h"
#include "../engine/types/color.h"
#include "../engine/types/point.h"
#include "../engine/media/graphics.h"
#include "../engine/core/matrix.h"

#define TETROMINOES_COUNT 7
#define TETROMINOES_ROTATIONS 4

typedef struct {
  unsigned int data;
  unsigned int width;
  unsigned int height;
} TetrominoState;

typedef struct {
  unsigned short shape;
  unsigned short rotation;
  Color color;
} Tetromino;

/*
 * Tetrominoes can get represented with a 16x16 matrix that only needs to contain [1;0].
 * We can store them in a single uint16_t.
 *
 * Starting from the least significant bit (horizontal flip), we can consider a piece as:
 *    xxxx  -> [FIRST_ROW]  [4..1]
 *    xxxx  -> [SECOND_ROW] [4..1]
 *    xxxx  -> [THIRD_ROW]  [4..1]
 *    xxxx  -> [FOURTH_ROW] [4..1]
 */
static TetrominoState tetrominoes[TETROMINOES_COUNT][TETROMINOES_ROTATIONS] = {
    {
        /* I */
        { 0x000F, 4, 1 }, { 0x1111, 1, 4 }, { 0x000F, 4, 1 }, { 0x1111, 1, 4 },
    },
    {
        /* J */
        { 0x0071, 3, 2 }, { 0x0113, 2, 3 },{ 0x0047, 3, 2 },{ 0x0322, 2, 3 },
    },
    {
        /* L */
        { 0x0074, 3, 2 }, { 0x0311, 2, 3 },{ 0x0017, 3, 2 },{ 0x0223, 2, 3 },
    },
    {
        /* O */
        { 0x0033, 2, 2 }, { 0x0033, 2, 2 },{ 0x0033, 2, 2 },{ 0x0033, 2, 2 },
    },
    {
        /* S */
        { 0x0036, 3, 2 }, { 0x0231, 2, 3 },{ 0x0036, 3, 2 },{ 0x0231, 2, 3 },
    },
    {
        /* T */
        { 0x0072, 3, 2 }, { 0x0131, 2, 3 },{ 0x0027, 3, 2 },{ 0x0232, 2, 3 },
    },
    {
        /* Z */
        { 0x0063, 3, 2 }, { 0x0132, 2, 3 },{ 0x0063, 3, 2 },{ 0x0132, 2, 3 },
    },
};

TetrominoState get_tetromino_state(Tetromino tetromino);

unsigned int get_tetromino_value_at(Tetromino tetromino, unsigned int x, unsigned int y);

unsigned int is_tetromino_valid(Tetromino tetromino);

void draw_tetromino(Graphics* graphics, Tetromino tetromino, Point position);

/**
 * Obtains the 2-dimensional size of the given tetromino, obtaining
 * it from its state.
 * @param tetromino
 * @return
 */
Size get_tetromino_size(Tetromino tetromino);

#endif //NXTETRIS_SRC_GAME_TETROMINO_H_
