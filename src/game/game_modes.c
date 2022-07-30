#include "game_modes.h"

#include <stdlib.h>

#include <curses.h>

unsigned int handle_shared_game_input(Game* game, Controls* controls, unsigned int* is_running) {
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

    case KEY_EXIT:
      /* Stop the execution of the game */
      is_running = 0;
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

void handle_game_mode_sp(Game* game, Controls* controls, GameDataSP* data) {
  /* Update game position to screen center */
  game->bounds.x = (game->graphics->size.width - game->bounds.width) / 2;
  game->bounds.y = (game->graphics->size.height - game->bounds.height) / 2;

  handle_shared_game_input(game, controls, &data->is_running);

  tick_game(game);
}

void handle_game_mode_mp(Game* game_a, Game* game_b, Controls* controls, GameDataMP* data) {
  Graphics* s_graphics;
  Game* active_game;

  s_graphics = game_a->graphics;

  /* Update games positions */
  game_a->bounds.x = (s_graphics->size.width - game_a->bounds.width - game_b->bounds.width) / 2 - 1;
  game_b->bounds.x = (s_graphics->size.width - game_a->bounds.width + game_b->bounds.width) / 2 + 3;
  game_b->bounds.y = game_a->bounds.y = (s_graphics->size.height - game_a->bounds.height) / 2;

  active_game = data->active_player == 0 ? game_a : game_b;

  game_a->disable_input = 1;
  game_b->disable_input = 1;
  active_game->disable_input = 0;

  if (handle_shared_game_input(active_game, controls, &data->is_running)) {
    data->active_player = !data->active_player;
  }

  tick_game(game_a);
  tick_game(game_b);
}

void handle_game_mode_cpu(Game* game_a, Game* game_b, Controls* controls, CPU* cpu, GameDataMP* data) {
  Graphics* s_graphics;
  Game* active_game;
  int event_param;
  int s, r, x;
  Point placing_point;
  int cur_size, min_size;

  CPUAction result;

  s_graphics = game_a->graphics;
  placing_point.y = 0;
  min_size = INT32_MAX;

  /* Update games positions */
  game_a->bounds.x = (s_graphics->size.width - game_a->bounds.width - game_b->bounds.width) / 2 - 1;
  game_b->bounds.x = (s_graphics->size.width - game_a->bounds.width + game_b->bounds.width) / 2 + 3;
  game_b->bounds.y = game_a->bounds.y = (s_graphics->size.height - game_a->bounds.height) / 2;

  active_game = data->active_player == 0 ? game_a : game_b;

  game_a->disable_input = 1;
  game_b->disable_input = 1;
  active_game->disable_input = 0;

  if (data->active_player == 0) {
    if (handle_shared_game_input(active_game, controls, &data->is_running)) {
      data->active_player = !data->active_player;
    }
  } else {
    for (s = 0; s < TETROMINOES_COUNT; ++s) {
      process_game_event(game_b, GAME_EVENT_CHP_UP, NULL);
      for (r = 0; r < TETROMINOES_ROTATIONS; ++r) {
        process_game_event(game_b, GAME_EVENT_ROT_CL, NULL);
        for (x = 0; x < game_b->board->cols; ++x) {
          placing_point.x = x;
          cur_size = intersect_tetromino_with_board(game_b->board, game_b->placing_piece, placing_point).y;
          if (cur_size < min_size) {
            min_size = cur_size;
            result.shape = game_b->placing_piece.shape;
            result.rotation = game_b->placing_piece.rotation;
            result.x_off = game_b->placing_piece_x;
          }
        }
      }
    }


    event_param = result.rotation;
    game_b->placing_piece.shape = result.shape;
    game_b->placing_piece.rotation = result.rotation;
    game_b->placing_piece_x = result.x_off;

    process_game_event(game_b, GAME_EVENT_DROP, NULL);
    data->active_player = 0;

    /*
    event_param = 0;
    process_game_event(game_b, GAME_EVENT_SET_CHP, &event_param);
    for (s = 0; s < TETROMINOES_COUNT; ++s) {
      event_param = get_next_available_tetromino(game_b);
      process_game_event(game_b, GAME_EVENT_SET_CHP, &event_param);

      for (r = 0; r < TETROMINOES_ROTATIONS; ++r) {
        process_game_event(game_b, GAME_EVENT_SET_ROT, &r);

        for (x = 0; x < game_b->board->cols; ++x) {
          placing_point.x = x;
          cur_size = intersect_tetromino_with_board(game_b->board, game_b->placing_piece, placing_point).y;
          if (cur_size < min_size) {
            min_size = cur_size;
            result.shape = s;
            result.rotation = r;
            result.x_off = x;
          }
        }
      }*/


  }

  tick_game(game_a);
  tick_game(game_b);
}
