#include "game_modes.h"

#include <stdlib.h>

#include <curses.h>

void handle_game_mode_sp(Game* game, Controls* controls, GameDataSP* data) {
  unsigned int mouse_x;

  /* Update game position to screen center */
  game->bounds.x = (game->graphics->size.width - game->bounds.width) / 2;
  game->bounds.y = (game->graphics->size.height - game->bounds.height) / 2 - 1;

  /* Handle tetromino placing on mouse left click */
  if (controls->mouse_state == 1) {
    process_game_event(game, GAME_EVENT_DROP, NULL);
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
      data->is_running = 0;
      break;
  }

  /*
   * Handle mouse input to place the temporary tetromino.
   * The x-axis is limited by the bounds of the game.
   */
  mouse_x = controls->mouse_position.x - game->bounds.x;
  process_game_event(game, GAME_EVENT_SET_X, &mouse_x);

  tick_game(game);
}

void handle_game_mode_mp(Game *game_a, Game *game_b, Controls *controls, GameDataMP* data) {
  unsigned int mouse_x;
  Graphics* s_graphics;
  Game* active_game;

  s_graphics = game_a->graphics;

  /* Update games positions */
  game_a->bounds.x = (s_graphics->size.width - game_a->bounds.width - game_b->bounds.width) / 2 - 1;
  game_b->bounds.x = (s_graphics->size.width - game_a->bounds.width + game_b->bounds.width) / 2 + 3;
  game_b->bounds.y = game_a->bounds.y = (s_graphics->size.height - game_a->bounds.height) / 2 - 1;

  active_game = data->active_player == 0 ? game_a : game_b;

  game_a->disable_input = 1;
  game_b->disable_input = 1;
  active_game->disable_input = 0;

  /* Handle tetromino placing on mouse left click */
  if (controls->mouse_state == 1) {
    process_game_event(active_game, GAME_EVENT_DROP, NULL);
    /* Also switch active player */
    data->active_player = !data->active_player;
  }

  switch (controls->pressed_key) {
    case KEY_RIGHT:
      /* Handle clockwise tetromino rotation */
      process_game_event(active_game, GAME_EVENT_ROT_CL, NULL);
      break;

    case KEY_LEFT:
      /* Handle counter-clockwise tetromino rotation */
      process_game_event(active_game, GAME_EVENT_ROT_CC, NULL);
      break;

    case KEY_UP:
      /* Handle piece switch */
      process_game_event(active_game, GAME_EVENT_CHP_UP, NULL);
      break;

    case KEY_DOWN:
      /* Handle piece switch */
      process_game_event(active_game, GAME_EVENT_CHP_DN, NULL);
      break;

    case KEY_EXIT:
      data->is_running = 0;
      break;
  }

  /*
   * Handle mouse input to place the temporary tetromino.
   * The x-axis is limited by the bounds of the game.
   */
  mouse_x = controls->mouse_position.x - active_game->bounds.x;
  process_game_event(active_game, GAME_EVENT_SET_X, &mouse_x);

  tick_game(game_a);
  tick_game(game_b);
}

void handle_game_mode_cpu(Game* game_a, Game* game_b, Controls* controls, GameDataMP* data) {

}
