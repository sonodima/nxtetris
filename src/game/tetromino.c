#include "tetromino.h"

TetrominoState get_tetromino_state(Tetromino tetromino) {
  return tetrominoes[tetromino.shape][tetromino.rotation];
}

unsigned int get_tetromino_value_at(Tetromino tetromino, unsigned int x, unsigned int y) {
  TetrominoState state;

  /* Avoid useless checks and segfaults */
  if (!is_tetromino_valid(tetromino)) {
    return 0;
  }

  state = get_tetromino_state(tetromino);

  /*
   * If the value we want to check is out of bounds, we can assume
   * that in that point it certainly is not equal to 1.
   */
  if (x >= state.width || y >= state.height) {
    return 0;
  }

  /* Obtain the value at the given coords */
  return state.data >> (y * 4 + x) & 1;
}

unsigned int is_tetromino_valid(Tetromino tetromino) {
  return tetromino.shape < TETROMINOES_COUNT && tetromino.rotation < TETROMINOES_ROTATIONS;
}

void draw_tetromino(Graphics *graphics, Tetromino tetromino, Point position) {
  TetrominoState state;
  Rect rect;
  unsigned int x, y;

  /*
   * Only draw the tetromino if its properties are valid.
   * This check should never fail, but if it does, at least you can't blame me.
   */
  if (!is_tetromino_valid(tetromino)) {
    draw_text(graphics, "UNKN", position, tetromino.color, VERTICAL_ALIGNMENT_LEFT, 1, 1);
    return;
  }

  state = get_tetromino_state(tetromino);

  /*
   * Iteration can be limited by the bounding box, as we are sure that all the points
   * outside it are equal to 0.
   */
  for (y = 0; y < state.height; ++y) {
    for (x = 0; x < state.width; ++x) {
      /*
       * Check if the value for the current coordinates is equal to 1.
       * In that case, we can draw it to the screen.
       */
      if (get_tetromino_value_at(tetromino, x, y)) {
        rect.x = (int) (position.x + x);
        rect.y = (int) (position.y + y);
        rect.width = 1;
        rect.height = 1;

        fill_rect(graphics, rect, tetromino.color);
      }
    }
  }
}

Size get_tetromino_size(Tetromino tetromino) {
  Size result;
  TetrominoState state;

  result.height = 0;
  result.width = 0;

  /*
   * Only draw the tetromino if its properties are valid.
   * This check should never fail, but if it does, at least you can't blame me.
   */
  if (!is_tetromino_valid(tetromino)) {
    return result;
  }

  state = get_tetromino_state(tetromino);

  result.height = (int) state.height;
  result.width = (int) state.width;
  return result;
}
