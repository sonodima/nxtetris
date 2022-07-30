#include "board.h"

#include <curses.h>

void draw_board(Graphics *graphics, Board *board, Point offset) {
  unsigned int x, y;
  int col_key;
  Rect rect;
  Color color;

  rect.width = 1;
  rect.height = 1;
  color.alpha = ALPHA_DARKER;

  for (y = 0; y < board->rows; ++y) {
    for (x = 0; x < board->cols + 1; ++x) {
      /* Ignore black pixels (unset) */
      col_key = board->data[y][x];
      if (col_key != COLOR_BLACK) {
        rect.y = (int) y + offset.y;
        rect.x = (int) x + offset.x - 1;
        color.background = col_key;
        color.foreground = col_key;
        fill_rect(graphics, rect, color);
      }
    }
  }
}

void add_tetromino_to_board(Board *board, Tetromino tetromino, Point position) {
  int x, y;

  if (!is_tetromino_valid(tetromino)) {
    return;
  }

  for (y = 0; y < 4; ++y) {
    for (x = 0; x < 4; ++x) {
      if (get_tetromino_value_at(tetromino, x, y)) {
        board->data[position.y + y][position.x + x + 1] = (int) tetromino.color.foreground;
      }
    }
  }
}

unsigned int check_board_collision(Board *board, Tetromino tetromino, Point point) {
  Size tetromino_size;
  unsigned int x, y, b_x, b_y;

  tetromino_size = get_tetromino_size(tetromino);

  for (y = 0; y < (unsigned int) tetromino_size.height; ++y) {
    for (x = 0; x < (unsigned int) tetromino_size.width; ++x) {
      if (get_tetromino_value_at(tetromino, x, y)) {
        b_x = point.x + x + 1;
        b_y = point.y + y;

        if (b_x >= 0 && b_x < board->cols + 1 && b_y >= 0 && b_y < board->rows) {
          if (board->data[b_y][b_x] != COLOR_BLACK) {
            return 1;
          }
        }
      }
    }
  }

  return 0;
}

Point intersect_tetromino_with_board(Board *board, Tetromino tetromino, Point point) {
  Point test_point;
  Size tetromino_size;
  unsigned int r_shift;

  test_point = point;
  tetromino_size = get_tetromino_size(tetromino);

  /* todo check for collisions in the first top line */
  for (r_shift = point.y; r_shift <= board->rows - tetromino_size.height; ++r_shift) {
    test_point.y = (int) r_shift;
    if (check_board_collision(board, tetromino, test_point)) {
      /* Decrement the y coordinate, as we want to return the first available placement point */
      test_point.y--;
      return test_point;
    }
  }

  return test_point;
}

void remove_board_line(Board *board, unsigned int row) {
  unsigned int x, y;

  for (y = row; y > 1; --y) {
    for (x = 0; x <= board->cols; ++x) {
      board->data[y][x] = board->data[y - 1][x];
    }
  }
}

unsigned int attempt_board_line_removal(Board *board) {
  unsigned int x, y, filled, removed_lines;

  removed_lines = 0;

  for (y = 0; y < board->rows; ++y) {
    filled = 1;
    for (x = 0; x <= board->cols; ++x) {
      if (board->data[y][x] == COLOR_BLACK) {
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

void invert_board_lines(Board *board, unsigned int from, unsigned int to) {
  unsigned int x, y;

  if (from >= 0 && from < board->rows && to >= 0 && to < board->rows) {
    for (y = from; y <= to; ++y) {
      for (x = 0; x <= board->cols; ++x) {
        board->data[y][x] = board->data[y][x] != COLOR_BLACK ? COLOR_BLACK : COLOR_WHITE;
      }
    }
  }
}
