#ifndef NXTETRIS_GAME_TETROMINO_H_
#define NXTETRIS_GAME_TETROMINO_H_

#include <stdio.h>

#include "../engine/types/rect.h"
#include "../engine/types/point.h"
#include "../engine/media/graphics.h"
#include "enums/placement_state.h"

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
    Point point;
    Color color;
    Rect bounds;
    PlacementState placement;
} Tetromino;

int is_tetromino_valid(Tetromino tetromino);

int get_tetromino_value_at(Tetromino tetromino, unsigned int x, unsigned int y);

void draw_tetromino(Graphics* graphics, Tetromino tetromino);

/**
 * Checks for collisions between two tetrominoes, with a y-axis offset that can be used for collision prediction.
 * @param first first tetromino. (must be placed higher than the second)
 * @param second secnnd tetromino.
 * @return 1 if the two tetrinubies cikkude at the given offset.
 */
int test_tetromino_collision(Tetromino first, Tetromino second, int offset);

/**
 * Test collisions with the walls.
 * todo: implement
 */
// Point test_tetromino_walls_limit(Rect bounds, Tetromino tetromino);

Rect get_tetromino_bounds(Tetromino tetromino);

void rotate_tetromino(Tetromino* tetromino, int direction);

#endif /* NXTETRIS_GAME_TETROMINO_H_ */
