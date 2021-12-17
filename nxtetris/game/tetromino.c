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
                    
                    fill_rect(graphics, rect, tetromino.color);
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

Rect get_tetromino_bounds(Tetromino tetromino) {
    Rect bounds;
    unsigned int i, j;
    
    /*
     Setup the bounds to the correct initial values
     so that further checks are easier.
     */
    bounds.x = 3;
    bounds.y = 3;
    bounds.width = 0;
    bounds.height = 0;
    
    if (tetromino.shape < TETROMINOES_COUNT && tetromino.rotation < TETROMINOES_ROTATIONS) {
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                if (tetrominoes[tetromino.shape][tetromino.rotation] & (0x8000 >> (i * 4 + j))) {
                    /*
                     Get the piece position, obtaining the
                     x and y coordinates closer to 0.
                     */
                    if (j < bounds.x) {
                        bounds.x = j;
                    }
                    if (i < bounds.y) {
                        bounds.y = i;
                    }
                    
                    
                    
                    /*
                     Calculate height, getting the delta from the
                     start coordinate and the furthest axis point.
                     Size is >1, so the value is incremented by 1.
                     */
                    if (j - bounds.x + 1 > bounds.width) {
                        bounds.width = j - bounds.x + 1;
                    }
                    if (i - bounds.y + 1 > bounds.height) {
                        bounds.height = i - bounds.y + 1;
                    }
                }
            }
        }
    }
    
    return bounds;
}

void rotate_tetromino(Tetromino* tetromino, int direction) {
    tetromino->rotation += direction;

    if (tetromino->rotation >= TETROMINOES_ROTATIONS) {
        tetromino->rotation = 0;
    }
    
    if (tetromino->rotation < 0) {
        tetromino->rotation = TETROMINOES_ROTATIONS - 1;
    }
    
    tetromino->bounds = get_tetromino_bounds(*tetromino);

    
    /*
    if (angle > 0) {
        if (tetromino->rotation < TETROMINOES_ROTATIONS - 1) {
            tetromino->rotation += angle;
        } else {
            tetromino->rotation = 0;
        }
    } else if (angle < 0) {
        if (tetromino->rotation > 0) {
            tetromino->rotation -= angle;
        } else {
            tetromino->rotation = TETROMINOES_ROTATIONS - 1;
        }
    }
    */
}
