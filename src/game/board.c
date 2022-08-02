#include "board.h"

#include <curses.h>

void draw_board(Graphics* graphics, Board* board, Point offset) {
  unsigned int x, y;
  int col_key;
  Rect rect;
  Color color;

  rect.width = 1;
  rect.height = 1;
  color.alpha = ALPHA_DARKER;

  for (y = 0; y < board->rows; ++y) {
    for (x = 0; x < board->cols; ++x) {
      /* Ignore black pixels (unset) */
      col_key = board->data[y][x];
      if (col_key != COLOR_BLACK) {
        rect.y = (int)y + offset.y;
        rect.x = (int)x + offset.x;
        color.background = col_key;
        color.foreground = col_key;
        fill_rect(graphics, rect, color);
      }
    }
  }
}

void add_tetromino_to_board(Board* board, Tetromino tetromino, Point position) {
  int x, y;

  if (!is_tetromino_valid(tetromino)) {
    return;
  }

  for (y = 0; y < 4; ++y) {
    for (x = 0; x < 4; ++x) {
      if (get_tetromino_value_at(tetromino, x, y)) {
        board->data[position.y + y][position.x + x] = (int)tetromino.color.foreground;
      }
    }
  }
}

/**
 * Checks if a tetromino (at a given position) collides with other elements in the board.
 * @param board Pointer to the board.
 * @param tetromino Target tetromino.
 * @param point Test position of the tetromino in 2d space.
 * @return 1 if the tetromino collided with other elements, otherwise 0.
 */
unsigned int check_board_collision(Board* board, Tetromino tetromino, Point point) {
  Size tetromino_size;
  unsigned int x, y, b_x, b_y;

  tetromino_size = get_tetromino_size(tetromino);

  for (y = 0; y < (unsigned int)tetromino_size.height; ++y) {
    for (x = 0; x < (unsigned int)tetromino_size.width; ++x) {
      if (get_tetromino_value_at(tetromino, x, y)) {
        b_x = point.x + x;
        b_y = point.y + y;

        if (b_x >= 0 && b_x < board->cols && b_y >= 0 && b_y < board->rows) {
          if (board->data[b_y][b_x] != COLOR_BLACK) {
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

/**
 * Internal function that removes a line by shifting all the elements of the board [bottom to top].
 * @param board Pointer to the board.
 * @param row Row to remove.
 */
void remove_board_line(Board* board, unsigned int row) {
  unsigned int x, y;

  if (row == 0) {
    for (x = 0; x < board->cols; ++x) {
      board->data[0][x] = COLOR_BLACK;
    }
  } else {
    for (y = row; y > 1; --y) {
      for (x = 0; x < board->cols; ++x) {
        board->data[y][x] = board->data[y - 1][x];
      }
    }
  }
}

unsigned int attempt_board_line_removal(Board* board) {
  unsigned int x, y, filled, removed_lines;

  removed_lines = 0;

  for (y = 0; y < board->rows; ++y) {
    filled = 1;
    for (x = 0; x < board->cols; ++x) {
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

unsigned int test_board_line_removal_for_action(Board* board, Tetromino tetromino, Point position) {
  Board* temp;
  unsigned int removed_lines;

  /* Note to future self, this is a bit hacky and could be improved */
  temp = make_matrix_with_data(*board->data, board->rows, board->cols);
  /* 100% sure this seg faults */
  add_tetromino_to_board(temp, tetromino, position);
  removed_lines = attempt_board_line_removal(temp);
  free_matrix(temp);

  return removed_lines;
}

unsigned int get_filled_lines_count_for_action(Board* board, Tetromino tetromino, Point position) {
  unsigned int x, y, filled, result;

  result = 0;

  for (y = 0; y < board->rows; ++y) {
    filled = 1;
    for (x = 0; x <= board->cols; ++x) {
      if (board->data[y][x] == COLOR_BLACK) {
        filled = 0;
      }
    }

    if (filled) {
      result++;
    }
  }

  return result;
}

void invert_board_lines(Board* board, unsigned int from, unsigned int to) {
  unsigned int x, y;

  if (from != to && from >= 0 && from < board->rows && to >= 0 && to < board->rows) {
    for (y = from + 1; y <= to; ++y) {
      for (x = 0; x < board->cols; ++x) {
        board->data[y][x] = board->data[y][x] != COLOR_BLACK ? COLOR_BLACK : COLOR_WHITE;
      }
    }
  }
}
