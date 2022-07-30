#include <stdlib.h>
#include <curses.h>

#include "engine/utils.h"

#include "game.h"

Game* make_game(Graphics* graphics, PiecesPool* pieces_pool, Rect bounds) {
  Game* game;

  game = malloc(sizeof(Game));
  game->graphics = graphics;
  game->pieces_pool = pieces_pool;
  game->bounds = bounds;
  reset_game(game);

  return game;
}

void reset_game(Game* game) {
  game->score = 0;
  game->finished_for_overflow = 0;
  game->disable_input = 0;

  /*
   * Initialized with GAME_STATE_IDLE so that a new
   * temp tetromino is spawned on launch.
   */
  game->state = GAME_STATE_IDLE;

  /*
   * Create the matrix that will hold the board.
   */
  if (game->board) {
    free_matrix(game->board);
  }
  game->board = make_matrix(game->bounds.height, game->bounds.width);
}

void free_game(Game* game) {
  if (game) {
    if (game->board) {
      free_matrix(game->board);
    }

    free(game);
  }
}

void draw_game_bounds(Game* game) {
  Color draw_color;
  Rect bounds_rect;
  Point text_point;
  char text_buffer[32];
  unsigned int pieces_count;

  draw_color.alpha = ALPHA_TRANSPARENT;
  draw_color.background = COLOR_BLACK;
  draw_color.foreground = COLOR_WHITE;

  /* The drawn game border must be bigger than the actual game. */
  bounds_rect = game->bounds;
  bounds_rect.x -= 1;
  bounds_rect.y -= 1;
  bounds_rect.width += 1;
  bounds_rect.height += 1;
  draw_rect(game->graphics, bounds_rect, draw_color);

  sprintf(text_buffer, " Score: %d ", game->score);
  text_point.x = bounds_rect.x + 1;
  text_point.y = bounds_rect.y;
  draw_text(game->graphics, text_buffer, text_point, draw_color, VERTICAL_ALIGNMENT_LEFT, 1, 0);

  /* Color the count text accordingly to the remaining pieces count */
  pieces_count = get_piece_count(game->pieces_pool, game->placing_piece.shape);
  if (pieces_count > game->pieces_pool->count_per_piece / 2) {
    draw_color.foreground = COLOR_CYAN;
  } else if (pieces_count > game->pieces_pool->count_per_piece / 4) {
    draw_color.foreground = COLOR_YELLOW;
  } else if (pieces_count > 1) {
    draw_color.foreground = COLOR_RED;
  } else {
    draw_color.foreground = COLOR_WHITE;
    draw_color.background = COLOR_RED;
  }

  if (!game->disable_input) {
    sprintf(text_buffer, " Count: %d ", pieces_count);
    text_point.x = bounds_rect.x + bounds_rect.width - 1;
    text_point.y = bounds_rect.y + bounds_rect.height;
    draw_text(game->graphics, text_buffer, text_point, draw_color, VERTICAL_ALIGNMENT_RIGHT, 1, 0);
  }
}

void initialize_placing_piece(Game* game) {
  int color = random_number(1, 6);

  game->placing_piece.color.background = COLOR_BLACK;
  game->placing_piece.color.foreground = color;
  game->placing_piece.color.alpha = ALPHA_DARKER;
  game->placing_piece.rotation = random_number(0, TETROMINOES_ROTATIONS - 1);

  do {
    game->placing_piece.shape = random_number(0, TETROMINOES_COUNT - 1);
  } while (!is_piece_available(game->pieces_pool, game->placing_piece.shape));
}

/**
 * Obtains the relative draw position for a tetromino.
 * @param piece Tetromino to use.
 * @param bounds Bounds of the board. (used to limit the x-axis)
 * @param placing_x X-axis offset.
 * @return Calculated relative draw point.
 */
Point get_placing_point(Tetromino piece, Rect bounds, int placing_x) {
  Point placing_point;
  Size tetromino_size;

  tetromino_size = get_tetromino_size(piece);

  /*
   * Limit its position in the X axis.
   * This is kinda overcomplicated but it works great.
   */
  if (placing_x < -1) {
    placing_x = -1;
  } else if (placing_x + tetromino_size.width > bounds.width - 1) {
    placing_x = bounds.width - tetromino_size.width;
  }

  placing_point.x = placing_x;
  placing_point.y = 0;

  return placing_point;
}

void tick_game(Game* game) {
  Tetromino preview_piece;
  Point placing_point, intersected_point;
  Point board_offset;

  board_offset.x = game->bounds.x;
  board_offset.y = game->bounds.y;
  draw_board(game->graphics, game->board, board_offset);

  switch (game->state) {
    case GAME_STATE_IDLE:
      /* Finish the game if all the pieces were used */
      if (has_pieces_left(game->pieces_pool)) {
        initialize_placing_piece(game);
        game->state = GAME_STATE_PLACING;
      } else {
        game->state = GAME_STATE_FINISHED;
      }
      break;

    case GAME_STATE_PLACING:
      if (!game->disable_input) {
        /* Draw the top tetromino */
        placing_point = get_placing_point(game->placing_piece, game->bounds, (int) game->placing_piece_x);

        /* Draw the dynamic bottom preview tetromino */
        preview_piece = game->placing_piece;
        preview_piece.color.alpha = ALPHA_LIGHTER;
        intersected_point = intersect_tetromino_with_board(game->board, game->placing_piece, placing_point);

        if (intersected_point.y >= placing_point.y) {
          /* Only draw the placing (top preview) tetromino if the intersected preview is lower*/
          draw_tetromino(game->graphics, game->placing_piece, game_rel_to_abs(game, placing_point));
        } else {
          /* Highlight with white the tetrominoes that when placed would result in the game to finish */
          preview_piece.color.foreground = COLOR_WHITE;
        }

        draw_tetromino(game->graphics, preview_piece, game_rel_to_abs(game, intersected_point));
      }
      break;

    default:
      break;
  }

  draw_game_bounds(game);
}

void drop_piece(Game* game) {
  Point placing_point;
  unsigned int removed_lines;

  /* Calculate the intersection point with the board and adds the current tetromino to it */
  placing_point = get_placing_point(game->placing_piece, game->bounds, (int) game->placing_piece_x);
  placing_point = intersect_tetromino_with_board(game->board, game->placing_piece, placing_point);

  /* Check if the tetromino would overflow the board vertically */
  if (placing_point.y >= 0) {
    add_tetromino_to_board(game->board, game->placing_piece, placing_point);

    /* Remove filled lines and handle score increment */
    removed_lines = attempt_board_line_removal(game->board);
    game->score += removed_lines_to_points(removed_lines);

    /* Decrement the pieces count for the current shape */
    if (get_piece_count(game->pieces_pool, game->placing_piece.shape) > 0) {
      game->pieces_pool->counts[game->placing_piece.shape]--;
    }

    /* Revert game state to idle to allow inputs */
    game->state = GAME_STATE_IDLE;
  } else {
    game->finished_for_overflow = 1;
    game->state = GAME_STATE_FINISHED;
  };
}

void process_game_event(Game* game, GameEvent event, void* data) {
  int temp;

  if (game->disable_input) {
    return;
  }

  switch (event) {
    case GAME_EVENT_SET_X:
      game->placing_piece_x = *(int*) data - 1;
      break;

    case GAME_EVENT_DROP:
      drop_piece(game);
      break;

    case GAME_EVENT_ROT_CL:
      temp = ((short) game->placing_piece.rotation + 1) % TETROMINOES_ROTATIONS;
      game->placing_piece.rotation = temp;
      break;

    case GAME_EVENT_ROT_CC:
      temp = ((short) game->placing_piece.rotation - 1) % TETROMINOES_ROTATIONS;
      if (temp < 0) {
        temp += TETROMINOES_ROTATIONS;
      }
      game->placing_piece.rotation = temp;
      break;

    case GAME_EVENT_SET_ROT:
      temp = *(int*) data;
      if (temp >= 0 && temp < TETROMINOES_ROTATIONS) {
        game->placing_piece.rotation = temp;
      }
      break;

    case GAME_EVENT_CHP_UP:
      game->placing_piece.shape = get_next_available_tetromino(game);
      break;

    case GAME_EVENT_CHP_DN:
      game->placing_piece.shape = get_previous_available_tetromino(game);
      break;

    case GAME_EVENT_SET_CHP:
      temp = *(int*) data;
      if (temp >= 0 && temp < TETROMINOES_COUNT) {
        game->placing_piece.shape = temp;
      }
      break;

    default:
      break;
  }
}

Point game_rel_to_abs(Game* game, Point point) {
  Point result;
  result.x = game->bounds.x + point.x;
  result.y = game->bounds.y + point.y;
  return result;
}

unsigned int removed_lines_to_points(unsigned int count) {
  unsigned int points;

  switch (count) {
    case 0:
      points = 0;
      break;

    case 1:
      points = 1;
      break;

    case 2:
      points = 3;
      break;

    case 3:
      points = 6;
      break;

    default:
      points = 12;
      break;
  }

  return points;
}

void draw_game_end_screen(Graphics* graphics, const char* sub_message) {
  Color color;
  Point point;

  color.background = COLOR_BLACK;
  color.foreground = COLOR_WHITE;
  color.alpha = ALPHA_DARKER;

  point.x = graphics->size.width / 2;
  point.y = graphics->size.height / 2 - 1;
  draw_text(graphics, "Game Over", point, color, VERTICAL_ALIGNMENT_CENTER, 1, 0);

  point.y += 2;
  draw_text(graphics, sub_message, point, color, VERTICAL_ALIGNMENT_CENTER, 0, 0);

  point.x = 0;
  point.y = graphics->size.height - 1;
  draw_text(graphics, " [→] NEW GAME ", point, color, VERTICAL_ALIGNMENT_LEFT, 1, 0);
}

int get_next_available_tetromino(Game* game) {
  int available;

  available = -1;
  if (has_pieces_left(game->pieces_pool)) {
    available = ((int) game->placing_piece.shape + 1) % TETROMINOES_COUNT;
    while (!is_piece_available(game->pieces_pool, available)) {
      available = ((int) available + 1) % TETROMINOES_COUNT;
    }
  }

  return available;
}

int get_previous_available_tetromino(Game* game) {
  int available;

  available = -1;
  if (has_pieces_left(game->pieces_pool)) {
    available = ((int) game->placing_piece.shape - 1) % TETROMINOES_COUNT;
    if (available < 0) {
      available += TETROMINOES_COUNT;
    }
    while (!is_piece_available(game->pieces_pool, available)) {
      available = ((int) available - 1) % TETROMINOES_COUNT;
      if (available < 0) {
        available += TETROMINOES_COUNT;
      }
    }
  }

  return available;
}

