#include "board.h"

#include <curses.h>

void draw_board(Graphics* graphics, Board* board, Point offset) {
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

void add_tetromino_to_board(Board* board, Tetromino tetromino, Point position) {
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

unsigned int check_board_collision(Board* board, Tetromino tetromino, Point point) {
  Size tetromino_size;
  unsigned int x, y, b_x, b_y;

  tetromino_size = get_tetromino_size(tetromino);

  for (x = 0; x < (unsigned int)tetromino_size.width; ++x) {
    for (y = 0; y < (unsigned int)tetromino_size.height; ++y) {

      if (get_tetromino_value_at(tetromino, x, y)) {
        b_x = point.x + x + 1;
        b_y = point.y + y;

        if (b_x >= 0 && b_x < board->cols + 1 && b_y >= 0 && b_y < board->rows) {
          if (board->data[b_y][b_x] != 0) {
            return 1;
          }
        }
      }
    }
  }

  return 0;
}

Point intersect_tetromino_with_board(Board* board, Tetromino tetromino, Point point) {
  Point test_point;
  Size tetromino_size;
  unsigned int r_shift;

  test_point = point;
  tetromino_size = get_tetromino_size(tetromino);

  for (r_shift = point.y; r_shift <= board->rows - tetromino_size.height; ++r_shift) {
    test_point.y = (int)r_shift;
    if (check_board_collision(board, tetromino, test_point)) {
      /* Decrement the y coordinate, as we want to return the first available placement point */
      test_point.y--;
      return test_point;
    }
  }

  return test_point;
}

void remove_board_line(Board* board, unsigned int row) {
  unsigned int x, y;

  for (x = 0; x < board->cols; ++x) {
    for (y = row; y > 1; --y) {
      board->data[y][x] = board->data[y - 1][x];
    }
  }
}

unsigned int attempt_board_line_removal(Board* board) {
  unsigned int x, y, filled, removed_lines;

  removed_lines = 0;

  for (y = 0; y < board->rows; ++y) {
    filled = 1;
    for (x = 0; x <= board->cols; ++x) {
      if (board->data[y][x] == 0) {
        filled = 0;
      }
    }

    if (filled) {
      remove_board_line(board, y);
      removed_lines++;
    }
  }

  return removed_lines;
}
