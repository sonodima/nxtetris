#include "tetrominoes.h"

void draw_tetromino(Graphics* graphics, unsigned int shape, unsigned int rotation,
                    Point point, Color color) {
    Rect rect;
    unsigned int i, j;
    
    if (shape < TETROMINOES_COUNT && rotation < TETROMINOES_ROTATIONS) {
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                if (tetromino_shapes[shape][rotation]  & (0x8000 >> (i * 4 + j))) {
                    rect.x = point.x + j * 2;
                    rect.y = point.y + i;
                    rect.width = 2;
                    rect.height = 1;
                    draw_rect(graphics, rect, color);
                }
            }
        }
    } else {
        draw_text(graphics, "UNKN", point, color, left, 1, 1);
    }
}

int test_tetromino_collision(unsigned int shape, unsigned int rotation, Point point,
                             Point tested_point) {
    unsigned int i, j, w;
    Point temp_point;

    if (shape < TETROMINOES_COUNT && rotation < TETROMINOES_ROTATIONS) {
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                if (tetromino_shapes[shape][rotation]  & (0x8000 >> (i * 4 + j))) {
                    for (w = 0; w < 2; ++w) {
                        temp_point.x = point.x + j * 2 + w;
                        temp_point.y = point.y + i;
                        
                        if (point_equals(tested_point, temp_point)) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}
