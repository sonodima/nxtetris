#include <stdlib.h>
#include <time.h>
#include <curses.h>

#include "../engine/utils.h"

#include "game.h"
#include "board.h"

Game* make_game(Graphics* graphics, Controls* controls, Rect bounds) {
  Game* game;

  game = malloc(sizeof(Game));
  game->graphics = graphics;
  game->controls = controls;
  game->bounds = bounds;
  game->gravity_clock = clock();
  game->score = 0;

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
 * Draws a rectangle outside the game bounds.
 * @param game Pointer to the game.
 */
void draw_game_bounds(Game* game) {
  Color bounds_color = {COLOR_WHITE, COLOR_BLACK, ALPHA_TRANSPARENT};
  Rect bounds_rect;
  Point score_point;
  char score_buffer[32];

  /*
   The drawn game border must be bigger than the actual game.
   */
  bounds_rect = game->bounds;
  bounds_rect.x -= 1;
  bounds_rect.y -= 1;
  bounds_rect.width += 1;
  bounds_rect.height += 1;
  draw_rect(game->graphics, bounds_rect, bounds_color, 1);

  sprintf(score_buffer, " Score: %d ", game->score);
  score_point.x = bounds_rect.x + 1;
  score_point.y = bounds_rect.y;
  draw_text(game->graphics, score_buffer, score_point, bounds_color, VERTICAL_ALIGNMENT_LEFT, 1, 0);
}

/**
 * Sets the placing_piece to a random tetromino.
 * @param game Pointer to the game.
 */
void initialize_placing_piece(Game* game) {
  int color = random_number(1, 6);
  game->placing_piece.color.background = COLOR_BLACK;
  game->placing_piece.color.foreground = color;
  game->placing_piece.color.alpha = ALPHA_LIGHT;
  game->placing_piece.rotation = random_number(0, TETROMINOES_ROTATIONS - 1);
  game->placing_piece.shape = random_number(0, TETROMINOES_COUNT - 1);
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
  Rect tetromino_bounds;

  tetromino_bounds = get_tetromino_bounds(piece);

  /*
   * Limit its position in the X axis.
   * This is kinda overcomplicated but it works great.
   */
  if (placing_x < -1) {
    placing_x = -1;
  } else if (placing_x + tetromino_bounds.width > bounds.width - 1) {
    placing_x = bounds.width - tetromino_bounds.width;
  }

  placing_point.x = placing_x - tetromino_bounds.x;
  placing_point.y = -tetromino_bounds.y;
  return placing_point;
}

void tick_game(Game* game) {
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
      placing_point = get_placing_point(game->placing_piece, game->bounds, (int)game->placing_piece_x);
      placing_point.x += board_offset.x;
      placing_point.y += board_offset.y;
      draw_tetromino(game->graphics, game->placing_piece, placing_point);
      break;

    case GAME_STATE_FALLING:

      break;

    case GAME_STATE_FINISHED:
      break;

    default:
      break;
  }

  draw_game_bounds(game);
}

void process_game_event(Game* game, GameEvent event, void* data) {
  int temp;
  Point placing_point;

  switch (event) {
    case GAME_EVENT_SET_X:
      game->placing_piece_x = *(int*)data - 1;
      break;

    case GAME_EVENT_DROP:
      placing_point = get_placing_point(game->placing_piece, game->bounds, (int)game->placing_piece_x);
      placing_point.y = intersect_tetromino_with_board(game->board, game->placing_piece, (int)game->placing_piece_x);

      add_tetromino_to_board(
          game->board,
          game->placing_piece,
          placing_point
      );
      game->state = GAME_STATE_IDLE;
      break;

    case GAME_EVENT_ROT_CL:
      temp = ((int)game->placing_piece.rotation + 1) % TETROMINOES_ROTATIONS;
      game->placing_piece.rotation = temp;
      break;

    case GAME_EVENT_ROT_CC:
      temp = ((int)game->placing_piece.rotation - 1) % TETROMINOES_ROTATIONS;
      game->placing_piece.rotation = temp;
      break;

    case GAME_EVENT_CHP_UP:
      temp = ((int)game->placing_piece.shape + 1) % TETROMINOES_COUNT;
      game->placing_piece.shape = temp;
      break;

    case GAME_EVENT_CHP_DN:
      temp = ((int)game->placing_piece.shape - 1) % TETROMINOES_COUNT;
      game->placing_piece.shape = temp;
      break;

    default:
      break;
  }
}