#include "tetromino.h"

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

int is_tetromino_valid(Tetromino tetromino) {
  return tetromino.shape < TETROMINOES_COUNT && tetromino.rotation < TETROMINOES_ROTATIONS;
}

Rect get_tetromino_bounds(Tetromino tetromino) {
  Rect bounds;
  unsigned int i, j;

  /*
   Set up the bounds to the correct initial values
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

void draw_tetromino(Graphics* graphics, Tetromino tetromino, Point position) {
  Rect rect;
  unsigned int i, j;

  /*
   Only draw the tetromino if its properties are valid.
   This check should never fail, but if it does, at least you can't blame me.
   */
  if (!is_tetromino_valid(tetromino)) {
    draw_text(graphics, "UNKN", position, tetromino.color, VERTICAL_ALIGNMENT_LEFT, 1, 1);
    return;
  }

  /*
   Iteration can be limited by the bounding box, as we are sure that all the points
   outside it are equal to 0.
   */
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j) {
      /*
       Check if the value for the current coordinates is equal to 1.
       In that case, we can draw it to the screen.
       */
      if (get_tetromino_value_at(tetromino, j, i)) {
        rect.x = position.x + j;
        rect.y = position.y + i;
        rect.width = 1;
        rect.height = 1;

        fill_rect(graphics, rect, tetromino.color);
      }
    }
  }
}
