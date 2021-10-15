#include "tetromino.h"

#include "../engine/collisions.h"

void draw_tetromino(Graphics* graphics, Tetromino tetromino) {
    Rect rect;
    unsigned int i, j;
    
    if (tetromino.shape < TETROMINOES_COUNT && tetromino.rotation < TETROMINOES_ROTATIONS) {
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                if (tetrominoes[tetromino.shape][tetromino.rotation] & (0x8000 >> (i * 4 + j))) {
                    rect.x = tetromino.point.x + j;
                    rect.y = tetromino.point.y + i;
                    rect.width = 1;
                    rect.height = 1;
                    
                    draw_rect(graphics, rect, tetromino.color);
                }
            }
        }
    } else {
        draw_text(graphics, "UNKN", tetromino.point, tetromino.color, left, 1, 1);
    }
}

int test_tetromino_collision(Graphics* graphics, Tetromino tetromino, Point point) {
    Rect rect;
    unsigned int i, j;

    if (tetromino.shape < TETROMINOES_COUNT && tetromino.rotation < TETROMINOES_ROTATIONS) {
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                if (tetrominoes[tetromino.shape][tetromino.rotation] & (0x8000 >> (i * 4 + j))) {
                    rect.x = point.x + j;
                    rect.y = point.y + i;
                    rect.width = 1;
                    rect.height = 1;
                    
                    if (test_box_collision(rect, point)) {
                        return 1;
                    }
                }
            }
        }
    }
    
    return 0;
}
