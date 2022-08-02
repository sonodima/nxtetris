#include "game_modes.h"

#include <ncurses.h>

/**
 * Internal function used to share user input routines with multiple game modes.
 * @param game Pointer to the game.
 * @param controls Pointer to the controls handler.
 * @return 1 if the user dropped dropped a piece this tick, otherwise 0.
 */
unsigned int handle_shared_game_input(Game* game, Controls* controls) {
  unsigned int mouse_x;
  unsigned int ret;

  ret = 0;

  /* Handle tetromino placing on mouse left click */
  if (controls->mouse_state == 1) {
    process_game_event(game, GAME_EVENT_DROP, NULL);
    ret = 1;
  }

  switch (controls->pressed_key) {
    case KEY_RIGHT:
      /* Handle clockwise tetromino rotation */
      process_game_event(game, GAME_EVENT_ROT_CL, NULL);
      break;

    case KEY_LEFT:
      /* Handle counter-clockwise tetromino rotation */
      process_game_event(game, GAME_EVENT_ROT_CC, NULL);
      break;

    case KEY_UP:
      /* Handle piece switch */
      process_game_event(game, GAME_EVENT_CHP_UP, NULL);
      break;

    case KEY_DOWN:
      /* Handle piece switch */
      process_game_event(game, GAME_EVENT_CHP_DN, NULL);
      break;
  }

  /*
   * Handle mouse input to place the temporary tetromino.
   * The x-axis is limited by the bounds of the game.
   */
  mouse_x = controls->mouse_position.x - game->bounds.x;
  process_game_event(game, GAME_EVENT_SET_X, &mouse_x);

  return ret;
}

void handle_game_mode_sp(Game* game, Controls* controls, Graphics* graphics) {
  /* Update game position to screen center */
  game->bounds.x = (graphics->size.width - game->bounds.width) / 2;
  game->bounds.y = (graphics->size.height - game->bounds.height) / 2;

  handle_shared_game_input(game, controls);

  tick_game(game, graphics);
}

void handle_game_mode_mp(Game* game_a, Game* game_b, Controls* controls, Graphics* graphics,
                         unsigned int* active_player) {
  Game* active_game;
  Game* idle_game;

  /* Update games positions */
  game_a->bounds.x = (graphics->size.width - game_a->bounds.width - game_b->bounds.width) / 2 - 1;
  game_b->bounds.x = (graphics->size.width - game_a->bounds.width + game_b->bounds.width) / 2 + 3;
  game_b->bounds.y = game_a->bounds.y = (graphics->size.height - game_a->bounds.height) / 2;

  active_game = *active_player == 0 ? game_a : game_b;
  idle_game = *active_player == 1 ? game_a : game_b;

  active_game->disable_input = 0;
  idle_game->disable_input = 1;

  if (handle_shared_game_input(active_game, controls)) {
    /* Invert the other player's lines if 3 or more lines are removed in a single action */
    if (active_game->last_removed_lines >= 3) {
      invert_board_lines(
          idle_game->board,
          idle_game->board->rows - active_game->last_removed_lines - 1,
          idle_game->board->rows - 1
      );
    }

    /* Switch the current player after an action */
    *active_player = !*active_player;
  }

  tick_game(game_a, graphics);
  tick_game(game_b, graphics);
}

void handle_game_mode_cpu(Game* game_a, Game* game_b, Controls* controls, Graphics* graphics, CPU* cpu,
                          unsigned int* active_player) {
  Game* active_game;
  Game* idle_game;
  int event_param;
  unsigned int s, r, x;
  Point placing_point, test_point;
  int max_size, max_removed_lines, removed_lines, max_filled_lines, filled_lines;

  CPUAction result;

  placing_point.y = 0;
  max_removed_lines = -1;
  max_filled_lines = -1;
  max_size = -1;

  /* Update games positions */
  game_a->bounds.x = (graphics->size.width - game_a->bounds.width - game_b->bounds.width) / 2 - 1;
  game_b->bounds.x = (graphics->size.width - game_a->bounds.width + game_b->bounds.width) / 2 + 3;
  game_b->bounds.y = game_a->bounds.y = (graphics->size.height - game_a->bounds.height) / 2;

  active_game = *active_player == 0 ? game_a : game_b;
  idle_game = *active_player == 1 ? game_a : game_b;

  active_game->disable_input = 0;
  idle_game->disable_input = 1;

  if (*active_player == 0) {
    /* User round */
    if (handle_shared_game_input(active_game, controls)) {
      *active_player = !*active_player;
    }
  } else {
    /* CPU round */
    for (s = 0; s < TETROMINOES_COUNT; ++s) {
      process_game_event(game_b, GAME_EVENT_CHP_UP, NULL);
      for (r = 0; r < TETROMINOES_ROTATIONS; ++r) {
        process_game_event(game_b, GAME_EVENT_ROT_CL, NULL);

        for (x = 0; x < game_b->board->cols; ++x) {
          placing_point.x = x - 1;
          test_point = intersect_tetromino_with_board(game_b->board, game_b->placing_piece, placing_point);

          filled_lines = get_filled_lines_count_for_action(game_b->board, game_b->placing_piece, placing_point);

          // removed_lines = test_board_line_removal_for_action(game_b->board, game_b->placing_piece, placing_point);

          if (filled_lines > max_filled_lines) {
            max_filled_lines = filled_lines;

            result.shape = game_b->placing_piece.shape;
            result.rotation = game_b->placing_piece.rotation;
            result.x_off = x;
          } else if (max_filled_lines <= 0 && test_point.y > max_size) {
            max_size = test_point.y;

            result.shape = game_b->placing_piece.shape;
            result.rotation = game_b->placing_piece.rotation;
            result.x_off = x;
          }
        }
      }
    }

    event_param = result.shape;
    process_game_event(game_b, GAME_EVENT_SET_CHP, &event_param);

    event_param = result.rotation;
    process_game_event(game_b, GAME_EVENT_SET_ROT, &event_param);

    event_param = result.x_off;
    process_game_event(game_b, GAME_EVENT_SET_X, &event_param);

    process_game_event(game_b, GAME_EVENT_DROP, NULL);
    *active_player = 0;
  }

  tick_game(game_a, graphics);
  tick_game(game_b, graphics);
}
