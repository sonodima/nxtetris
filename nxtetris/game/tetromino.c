#include "tetromino.h"

#include "../engine/collisions.h"

#include <math.h>

int is_tetromino_valid(Tetromino tetromino) {
    return tetromino.shape < TETROMINOES_COUNT && tetromino.rotation < TETROMINOES_ROTATIONS;
}

int get_tetromino_value_at(Tetromino tetromino, unsigned int x, unsigned int y) {
    /*
     If the value we want to check is out of bounds, we can assume
     that in that point it certainly is not equal to 1.
     */
    if (x >= 4 || y >= 4) {
        return 0;
    }
    
    /*
     Obtain the value at the given coords, from the bitmask table.
     */
    return tetrominoes[tetromino.shape][tetromino.rotation] & (0x8000 >> (y * 4 + x));
}

void draw_tetromino(Graphics* graphics, Tetromino tetromino) {
    Rect rect;
    unsigned int i, j;
    
    /*
     Only draw the tetromino if its properties are valid.
     This check should never fail, but if it does, at least you can't blame me.
     */
    if (!is_tetromino_valid(tetromino)) {
        draw_text(graphics, "UNKN", tetromino.point, tetromino.color, VERTICAL_ALIGNMENT_LEFT, 1, 1);
        return;
    }
    
    /*
     Iteration can be limited by the bounding box, as we are sure that all the points
     outside of it are equal to 0.
     */
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            /*
             Check if the value for the current coordinates is equal to 1.
             In that case, we can draw it to the screen.
             */
            if (get_tetromino_value_at(tetromino, j, i)) {
                rect.x = tetromino.point.x + j;
                rect.y = tetromino.point.y + i;
                rect.width = 1;
                rect.height = 1;
                
                fill_rect(graphics, rect, tetromino.color);
            }
        }
    }
}

int test_tetromino_collision(Tetromino first, Tetromino second, int offset) {
    unsigned int i, j;
    int delta_x, delta_y;
    
    /*
     Only test collisions if both tetrominoes are valid.
     We can assume that there is no collision if one of the tetrominoes is invalid.
     */
    if (!is_tetromino_valid(first) || !is_tetromino_valid(second)) {
        return 0;
    }
    
    /*
     If both tetrominoes do not overlap on the x-axis, we are sure they do not collide.
     */
    if (first.point.x + first.bounds.x > second.point.x + second.bounds.x + second.bounds.width ||
        first.point.x + first.bounds.x + first.bounds.width < second.point.x + second.bounds.x) {
        return 0;
    }
    
    /*
     If both tetrominoes do not overlap on the y-axis, we are sure they do not collide.
     */
    if (first.point.y + first.bounds.y + offset > second.point.y + second.bounds.y + second.bounds.height ||
        first.point.y + first.bounds.y + first.bounds.height + offset < second.point.y + second.bounds.y) {
        return 0;
    }
    
    /*
     Obtain the delta between the two tetrominoes.
     Used later for point translation between the first and the second one.
     */
    delta_x = first.point.x - second.point.x;
    delta_y = first.point.y + offset - second.point.y;
    
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            /*
             Check for the collisions in the iterated point and the translated one.
             */
            if (get_tetromino_value_at(first, j, i) &&
                get_tetromino_value_at(second, j + delta_x, i + delta_y)) {
                return 1;
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
    
    if (is_tetromino_valid(tetromino)) {
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                if (get_tetromino_value_at(tetromino, j, i)) {
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
    tetromino->rotation = (tetromino->rotation + direction) % TETROMINOES_ROTATIONS;
    tetromino->bounds = get_tetromino_bounds(*tetromino);
}
