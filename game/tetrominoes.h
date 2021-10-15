#ifndef game_tetrominoes_h
#define game_tetrominoes_h

#include <stdio.h>

#include "../engine/graphics.h"

#define TETROMINOES_COUNT 7
#define TETROMINOES_ROTATIONS 4

/**
 * Bitmask encoding idea from: https://stackoverflow.com/a/38596291
 */
static int tetromino_shapes[TETROMINOES_COUNT][TETROMINOES_ROTATIONS] = {
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

void draw_tetromino(Graphics* graphics, unsigned int shape, unsigned int rotation,
                    Point point, Color color);

#endif /* game_tetrominoes_h */
