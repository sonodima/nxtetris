#include <unistd.h>
#include <curses.h>

#include "engine/types/color.h"
#include "engine/media/graphics.h"
#include "engine/controls.h"
#include "game/game.h"

#define FRAME_INTERVAL 20

int main() {
  Graphics* graphics;
  Controls* controls;
  Game* game;
  unsigned int is_running;
  int mouse_x;
  Rect game_bounds;
  Point footer_pos;
  Color footer_color;

  is_running = 1;
  mouse_x = 0;

  game_bounds.x = 0;
  game_bounds.y = 0;
  game_bounds.width = 10;
  game_bounds.height = 15;

  footer_pos.x = 0;
  footer_pos.y = 0;
  footer_color.alpha = ALPHA_TRANSPARENT;
  footer_color.background = COLOR_BLACK;
  footer_color.foreground = COLOR_WHITE;

  graphics = make_graphics();
  controls = make_controls();
  game = make_game(graphics, controls, game_bounds);

  /* Main process loop */
  while (is_running) {
    update_controls(controls);
    begin_frame(graphics);

    /*
     Update game position to screen center.
     todo: 2 players: handle bounds correctly
     */
    game->bounds.x = (graphics->size.width - game->bounds.width) / 2;
    game->bounds.y = (graphics->size.height - game->bounds.height) / 2 - 1;

    /* Handle tetromino placing on mouse left click */
    if (game->controls->mouse_state == 1) {
      process_game_event(game, GAME_EVENT_DROP, NULL);
    }

    switch (game->controls->pressed_key) {
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
        is_running = false;
        break;
    }

    /*
     * Handle mouse input to place the temporary tetromino.
     * The x-axis is limited by the bounds of the game.
     */
    mouse_x = game->controls->mouse_position.x - game->bounds.x;
    process_game_event(game, GAME_EVENT_SET_X, &mouse_x);

    tick_game(game);

    footer_pos.y = graphics->size.height - 1;
    draw_text(graphics, " nxtetris, by sonodima @ Università Ca' Foscari ",
              footer_pos, footer_color, VERTICAL_ALIGNMENT_LEFT, 1, 0);

    present_frame();
    usleep(1000 * FRAME_INTERVAL);
  }

  free_game(game);
  free_controls(controls);
  free_graphics(graphics);
  return 0;
}
