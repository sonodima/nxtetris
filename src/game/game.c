#include <stdlib.h>
#include <curses.h>

#include "../engine/utils.h"

#include "game.h"

Game* make_game(Graphics* graphics, Controls* controls, Rect bounds) {
  Game* game;
  unsigned int i;

  game = malloc(sizeof(Game));
  game->graphics = graphics;
  game->controls = controls;
  game->bounds = bounds;
  game->score = 0;

  /* Initialize all piece counts to the maximum value */
  for (i = 0; i < TETROMINOES_COUNT; ++i) {
    game->pieces_count[i] = PER_PIECE_COUNT;
  }

  /*
   * Initialized with GAME_STATE_IDLE so that a new
   * temp tetromino is spawned on launch.
   */
  game->state = GAME_STATE_IDLE;

  /*
   * Create the matrix that will hold the board.
   */
  game->board = make_matrix(game->bounds.height, game->bounds.width);

  return game;
}

void free_game(Game* game) {
  if (game) {
    free(game->board);
    free(game);
  }
}

/**
 * Draws a rectangle outside the game bounds, the score and the current piece's count.
 * @param game Pointer to the game.
 */
void draw_game_bounds(Game* game) {
  Color bounds_color;
  Rect bounds_rect;
  Point text_point;
  char text_buffer[32];

  bounds_color.alpha = ALPHA_TRANSPARENT;
  bounds_color.background = COLOR_BLACK;
  bounds_color.foreground = COLOR_WHITE;

  /* The drawn game border must be bigger than the actual game. */
  bounds_rect = game->bounds;
  bounds_rect.x -= 1;
  bounds_rect.y -= 1;
  bounds_rect.width += 1;
  bounds_rect.height += 1;
  draw_rect(game->graphics, bounds_rect, bounds_color);

  sprintf(text_buffer, " Score: %d ", game->score);
  text_point.x = bounds_rect.x + 1;
  text_point.y = bounds_rect.y;
  draw_text(game->graphics, text_buffer, text_point, bounds_color, VERTICAL_ALIGNMENT_LEFT, 1, 0);

  sprintf(text_buffer, " Count: %d ", game->pieces_count[game->placing_piece.shape]);
  text_point.x = bounds_rect.x + bounds_rect.width - 1;
  text_point.y = bounds_rect.y + bounds_rect.height;
  draw_text(game->graphics, text_buffer, text_point, bounds_color, VERTICAL_ALIGNMENT_RIGHT, 1, 0);
}

/**
 * Sets the placing_piece to a random tetromino.
 * @param game Pointer to the game.
 */
void initialize_placing_piece(Game* game) {
  // todo add counter for the pieces starting to 20
  int color = random_number(1, 6);

  game->placing_piece.color.background = COLOR_BLACK;
  game->placing_piece.color.foreground = color;
  game->placing_piece.color.alpha = ALPHA_DARKER;
  game->placing_piece.rotation = random_number(0, TETROMINOES_ROTATIONS - 1);

  do {
    game->placing_piece.shape = random_number(0, TETROMINOES_COUNT - 1);
  } while (!is_piece_available(game, game->placing_piece.shape));
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
  Point placing_point;
  Point board_offset;

  board_offset.x = game->bounds.x;
  board_offset.y = game->bounds.y;
  draw_board(game->graphics, game->board, board_offset);

  switch (game->state) {
    case GAME_STATE_IDLE:
      initialize_placing_piece(game);
      game->state = GAME_STATE_PLACING;
      break;

    case GAME_STATE_PLACING:
      /* Draw the top tetromino */
      placing_point = get_placing_point(game->placing_piece, game->bounds, (int)game->placing_piece_x);
      draw_tetromino(game->graphics, game->placing_piece, game_rel_to_abs(game, placing_point));

      /* Draw the dynamic bottom preview tetromino */
      preview_piece = game->placing_piece;
      preview_piece.color.alpha = ALPHA_LIGHTER;
      placing_point = intersect_tetromino_with_board(game->board, game->placing_piece, placing_point);
      draw_tetromino(game->graphics, preview_piece, game_rel_to_abs(game, placing_point));
      break;

    case GAME_STATE_FINISHED:
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
  placing_point = get_placing_point(game->placing_piece, game->bounds, (int)game->placing_piece_x);
  placing_point = intersect_tetromino_with_board(game->board, game->placing_piece, placing_point);
  add_tetromino_to_board(game->board, game->placing_piece, placing_point);

  /* Remove filled lines and handle score increment */
  removed_lines = attempt_board_line_removal(game->board);
  game->score += removed_lines_to_points(removed_lines);

  /* Decrement the pieces count for the current shape */
  if (game->pieces_count[game->placing_piece.shape] > 0) {
    game->pieces_count[game->placing_piece.shape]--;
  }

  /* Revert game state to idle to allow inputs */
  game->state = GAME_STATE_IDLE;
}

void process_game_event(Game* game, GameEvent event, void* data) {
  int temp;

  switch (event) {
    case GAME_EVENT_SET_X:
      game->placing_piece_x = *(int*)data - 1;
      break;

    case GAME_EVENT_DROP:
      drop_piece(game);
      break;

    case GAME_EVENT_ROT_CL:
      temp = ((short)game->placing_piece.rotation + 1) % TETROMINOES_ROTATIONS;
      game->placing_piece.rotation = temp;
      break;

    case GAME_EVENT_ROT_CC:
      temp = ((short)game->placing_piece.rotation - 1) % TETROMINOES_ROTATIONS;
      if (temp < 0) {
        temp += TETROMINOES_ROTATIONS;
      }
      game->placing_piece.rotation = temp;
      break;

    case GAME_EVENT_CHP_UP:
      temp = ((int)game->placing_piece.shape + 1) % TETROMINOES_COUNT;

      // while not placing point available increment - also add check for when all pieces all finished

      game->placing_piece.shape = temp;
      break;

    case GAME_EVENT_CHP_DN:
      temp = ((int)game->placing_piece.shape - 1) % TETROMINOES_COUNT;
      if (temp < 0) {
        temp += TETROMINOES_COUNT;
      }
      game->placing_piece.shape = temp;
      break;

    default:
      break;
  }
}

Point game_rel_to_abs(Game *game, Point point) {
  Point result;
  result.x = game->bounds.x + point.x;
  result.y = game->bounds.y + point.y;
  return result;
}

unsigned int has_pieces_left(Game* game) {
  unsigned int i, accum;

  accum = 0;
  for (i = 0; i < TETROMINOES_COUNT; ++i) {
    accum += game->pieces_count[i];
  }

  return accum > 0;
}

unsigned int is_piece_available(Game* game, unsigned int piece) {
  if (piece < 0 || piece >= TETROMINOES_COUNT) {
    return 0;
  }

  return game->pieces_count[piece] > 0;
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
