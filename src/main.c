#include <unistd.h>
#include <curses.h>
#include <string.h>

#include "engine/types/color.h"
#include "engine/media/graphics.h"
#include "engine/controls.h"
#include "game/pieces_pool.h"
#include "game/game.h"
#include "game/cpu.h"
#include "game/game_modes.h"
#include "game/ui/main_menu.h"
#include "game/ui/end_screen.h"

/**
 * Delay, in milliseconds, between each draw call.
 */
#define FRAME_INTERVAL 2

/**
 * Writes the end message to a string buffer.
 * The buffer should be at least 32 bytes long.
 * @param game_mode The current game mode.
 * @param game_a Pointer to the first game.
 * @param game_b Pointer to the second game.
 * @param buffer Output buffer.
 */
void get_end_message(GameMode game_mode, Game* game_a, Game* game_b, char* buffer) {
  if (game_mode == GAME_MODE_SP) {
    sprintf(buffer, "Score: %d\n", game_a->score);
  } else {
    if (game_a->finished_for_overflow || game_b->finished_for_overflow) {
      if (game_a->finished_for_overflow) {
        strcpy(buffer, "Player [B] Won!");
      } else if (game_b->finished_for_overflow) {
        strcpy(buffer, "Player [A] Won!");
      }
    } else {
      if (game_a->score > game_b->score) {
        strcpy(buffer, "Player [A] Won! Reason: Score");
      } else if (game_a->score < game_b->score) {
        strcpy(buffer, "Player [B] Won! Reason: Score");
      } else {
        strcpy(buffer, "Tie!");
      }
    }
  }
}

/**
 * Program initialization, main loop and cleanup.
 * @return 0 if there were no errors.
 */
int main() {
  Graphics* graphics;
  Controls* controls;
  MainMenu* main_menu;
  PiecesPool* pieces_pool;
  Game* game_a;
  Game* game_b;
  CPU* cpu;
  Rect game_bounds;
  Point footer_pos;
  Color footer_color;
  GameMode game_mode;
  unsigned int is_running;
  unsigned int in_end_screen;
  unsigned int in_menu;
  unsigned int active_player;
  char end_message[32];

  game_bounds.x = 0;
  game_bounds.y = 0;
  game_bounds.width = 10;
  game_bounds.height = 15;

  footer_pos.x = 0;
  footer_color.alpha = ALPHA_TRANSPARENT;
  footer_color.background = COLOR_WHITE;
  footer_color.foreground = COLOR_BLACK;

  is_running = 1;

  graphics = make_graphics();
  controls = make_controls();
  main_menu = make_main_menu(graphics);
  pieces_pool = make_pieces_pool(2);
  game_a = make_game(graphics, pieces_pool, game_bounds);
  game_b = make_game(graphics, pieces_pool, game_bounds);
  cpu = make_cpu(game_b);

  /* Program loop, that keeps the game from closing after it finishes */
  while (is_running) {
    /* Main menu loop */
    in_menu = 1;
    while (in_menu) {
      update_controls(controls);
      begin_frame(graphics);

      switch (controls->pressed_key) {
        case KEY_UP:
          if (main_menu->selected_mode <= 0) {
            main_menu->selected_mode = GAME_MODES_COUNT - 1;
          } else {
            main_menu->selected_mode--;
          }
          break;

        case KEY_DOWN:
          main_menu->selected_mode = (main_menu->selected_mode + 1) % GAME_MODES_COUNT;
          break;

        case KEY_RIGHT:
          in_menu = 0;
          game_mode = main_menu->selected_mode;
          break;

        case KEY_LEFT:
          in_menu = 0;
          is_running = 0;
          break;
      }

      draw_main_menu(main_menu);

      present_frame();
      usleep(1000 * FRAME_INTERVAL);
    }

    if (is_running) {
      /* Handle initialization routines that are always executed */
      reset_game(game_a);
      reset_game(game_b);

      active_player = 0;

      /* Handle initialization routines that depend on the game mode */
      switch (game_mode) {
        case GAME_MODE_SP:
          reset_pieces_pool(pieces_pool, 20);
          break;

        case GAME_MODE_MP:
        case GAME_MODE_CPU:
          reset_pieces_pool(pieces_pool, 40);
          break;

        default:
          break;
      }

      /* Main process loop, only exit if at least one game is in the GAME_STATE_FINISHED state */
      while (game_a->state != GAME_STATE_FINISHED && game_b->state != GAME_STATE_FINISHED) {
        update_controls(controls);
        begin_frame(graphics);

        switch (game_mode) {
          case GAME_MODE_SP:
            handle_game_mode_sp(game_a, controls);
            break;

          case GAME_MODE_MP:
            handle_game_mode_mp(game_a, game_b, controls, &active_player);
            break;

          case GAME_MODE_CPU:
            handle_game_mode_cpu(game_a, game_b, controls, cpu, &active_player);
            break;
        }

        /* Draw footer text and rectangle */
        footer_pos.y = graphics->size.height - 1;
        draw_text(
            graphics,
            " nxtetris, by sonodima @ Università Ca' Foscari ",
            footer_pos, footer_color,
            VERTICAL_ALIGNMENT_LEFT,
            1, 0
        );

        present_frame();
        usleep(1000 * FRAME_INTERVAL);
      }

      /* Game end screen loop, drawn after at least one game finished */
      in_end_screen = 1;
      while (in_end_screen) {
        update_controls(controls);
        begin_frame(graphics);

        switch (controls->pressed_key) {
          case KEY_RIGHT:
            in_end_screen = 0;
            break;
        }

        get_end_message(game_mode, game_a, game_b, end_message);
        draw_end_screen(graphics, end_message);

        present_frame();
        usleep(1000 * FRAME_INTERVAL);
      }
    }
  }

  /* Program cleanup */
  free_cpu(cpu);
  free_game(game_a);
  free_game(game_b);
  free_pieces_pool(pieces_pool);
  free_main_menu(main_menu);
  free_controls(controls);
  free_graphics(graphics);
  return 0;
}
