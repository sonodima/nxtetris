#include "board.h"

#include <curses.h>

void draw_board(Graphics* graphics, Matrix* board, Point offset) {
  unsigned int i, j;
  int col_key;
  Rect rect;
  Color color;

  rect.width = 1;
  rect.height = 1;
  color.alpha = ALPHA_DARKER;

  for (i = 0; i < board->rows; ++i) {
    for (j = 0; j < board->cols + 1; ++j) {
      /*
       * Ignore black pixels (unset)
       */
      col_key = board->data[i][j];
      if (col_key != COLOR_BLACK) {
        rect.y = (int)i + offset.y;
        rect.x = (int)j + offset.x - 1;
        color.background = col_key;
        color.foreground = col_key;
        fill_rect(graphics, rect, color);
      }
    }
  }
}

void add_tetromino_to_board(Matrix *board, Tetromino tetromino, Point position) {
  int i, j;

  if (!is_tetromino_valid(tetromino)) {
    return;
  }

  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j) {
      if (get_tetromino_value_at(tetromino, j, i)) {
        board->data[position.y + i][position.x + j + 1] = (int)tetromino.color.foreground;
      }
    }
  }
}

int intersect_tetromino_with_board(Matrix *board, Tetromino tetromino, int x_offset) {
  int i, j;
  Rect tetromino_bounds;
  int max_y[4] = {-1, -1, -1, -1};

  if (!is_tetromino_valid(tetromino)) {
    return 0;
  }

  /**
   * Build a mono-dimensional array with the lowest points of the tetromino.
   */
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (get_tetromino_value_at(tetromino, j, i)) {
        if (j > max_y[i]) {
          max_y[i] = j;
        }
      }
    }
  }

  tetromino_bounds = get_tetromino_bounds(tetromino);

  return board->rows - tetromino_bounds.y - tetromino_bounds.height;

  /*

  for (i = 0; i < 4; i++) {
    if (max_y[i] != -1) {
      for (j = 0; j < (int)board->rows; j++) {


        if (j + max_y[i])

        if (board->data[j + 1][i + x_offset] != 0) {

        }
      }
    }
  }
   */

  return 0;
}

int attempt_board_line_removal(Matrix *board) {
  unsigned int i, j;
  int temp;


  return 0;
}
