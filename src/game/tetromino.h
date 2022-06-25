#ifndef NXTETRIS_SRC_GAME_TETROMINO_H_
#define NXTETRIS_SRC_GAME_TETROMINO_H_

#include "../engine/types/rect.h"
#include "../engine/types/color.h"
#include "../engine/types/point.h"
#include "../engine/media/graphics.h"
#include "../engine/core/matrix.h"

#define TETROMINOES_COUNT 7
#define TETROMINOES_ROTATIONS 4

/**
 * Bitmask encoding idea from: https://stackoverflow.com/a/38596291
 */
static int tetrominoes[TETROMINOES_COUNT][TETROMINOES_ROTATIONS] = {
    {
        /* T */
        0x4640, 0x0E40, 0x4C40, 0x4E00
    },
    {
        /* S */
        0x8C40, 0x6C00, 0x8C40, 0x6C00
    },
    {
        /* Z */
        0x4C80, 0xC600, 0x4C80, 0xC600
    },
    {
        /* I */
        0x4444, 0x0F00, 0x4444, 0x0F00
    },
    {
        /* J */
        0x44C0, 0x8E00, 0xC880, 0xE200
    },
    {
        /* L */
        0x88C0, 0xE800, 0xC440, 0x2E00
    },
    {
        /* O */
        0xCC00, 0xCC00, 0xCC00, 0xCC00
    },
};

typedef struct {
  unsigned int shape;
  unsigned int rotation;
  Color color;
} Tetromino;

int get_tetromino_value_at(Tetromino tetromino, unsigned int x, unsigned int y);

int is_tetromino_valid(Tetromino tetromino);

Rect get_tetromino_bounds(Tetromino tetromino);

void draw_tetromino(Graphics* graphics, Tetromino tetromino, Point position);

#endif //NXTETRIS_SRC_GAME_TETROMINO_H_
