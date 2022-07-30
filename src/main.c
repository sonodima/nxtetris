#include <unistd.h>
#include <curses.h>

#include "engine/types/color.h"
#include "engine/media/graphics.h"
#include "engine/controls.h"
#include "game/pieces_pool.h"
#include "game/game.h"
#include "game/cpu.h"
#include "game/game_modes.h"

#define FRAME_INTERVAL 2
#define SHOW_MENU 1

#if SHOW_MENU
#include "main_menu.h"
#endif

int main() {
  Graphics *graphics;
  Controls *controls;
  PiecesPool *pieces_pool;
  Game *game_a;
  Game *game_b;
  CPU *cpu;
  GameDataSP data_sp;
  GameDataMP data_mp;
  Rect game_bounds;
  Point footer_pos;
  Color footer_color;
  GameMode game_mode;
  unsigned int prog_running;
  unsigned int in_end_scene;
#if SHOW_MENU
  MainMenu *main_menu;
  unsigned int in_menu;
#endif

  prog_running = 1;

  data_sp.is_running = 1;
  data_mp.is_running = 1; /* todo remove */
  data_mp.active_player = 0;

  game_bounds.x = 0;
  game_bounds.y = 0;
  game_bounds.width = 10;
  game_bounds.height = 15;

  footer_pos.x = 0;
  footer_color.alpha = ALPHA_TRANSPARENT;
  footer_color.background = COLOR_WHITE;
  footer_color.foreground = COLOR_BLACK;

  game_mode = GAME_MODE_SP;

  graphics = make_graphics();
  controls = make_controls();
#if SHOW_MENU
  main_menu = make_main_menu(graphics);
#endif
  pieces_pool = make_pieces_pool(2);
  game_a = make_game(graphics, pieces_pool, game_bounds);
  game_b = make_game(graphics, pieces_pool, game_bounds);
  cpu = make_cpu(game_b);

  while (prog_running) {
#if SHOW_MENU
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
          };
          break;

        case KEY_DOWN:main_menu->selected_mode = (main_menu->selected_mode + 1) % GAME_MODES_COUNT;
          break;

        case KEY_RIGHT:in_menu = 0;
          game_mode = main_menu->selected_mode;
          break;
      }

      draw_main_menu(main_menu);

      present_frame();
      usleep(1000 * FRAME_INTERVAL);
    }
#endif

    /* Handle initialization routines that are always executed */
    reset_game(game_a);
    reset_game(game_b);

    /* Handle initialization routines that depend on the game mode */
    switch (game_mode) {
      case GAME_MODE_SP:reset_pieces_pool(pieces_pool, 20);
        break;

      case GAME_MODE_MP:
      case GAME_MODE_CPU:reset_pieces_pool(pieces_pool, 40);
        break;

      default:break;
    }

    /* Main process loop */
    while ((game_mode == GAME_MODE_SP && game_a->state != GAME_STATE_FINISHED)
        || ((game_mode == GAME_MODE_MP || game_mode == GAME_MODE_CPU)
            && (game_a->state != GAME_STATE_FINISHED && game_b->state != GAME_STATE_FINISHED))) {
      update_controls(controls);
      begin_frame(graphics);

      switch (game_mode) {
        case GAME_MODE_SP:handle_game_mode_sp(game_a, controls, &data_sp);
          break;

        case GAME_MODE_MP:handle_game_mode_mp(game_a, game_b, controls, &data_mp);
          break;

        case GAME_MODE_CPU:handle_game_mode_cpu(game_a, game_b, controls, cpu, &data_mp);
          break;

        default:break;
      }

      /* Draw footer text and rectangle */
      footer_pos.y = graphics->size.height - 1;
      draw_text(graphics, " nxtetris, by sonodima @ Università Ca' Foscari ",
                footer_pos, footer_color, VERTICAL_ALIGNMENT_LEFT, 1, 0);

      present_frame();
      usleep(1000 * FRAME_INTERVAL);
    }

    in_end_scene = 1;
    while (in_end_scene) {
      update_controls(controls);
      begin_frame(graphics);

      switch (controls->pressed_key) {
        case KEY_RIGHT:in_end_scene = 0;
          break;
      }

      if (game_mode == GAME_MODE_SP) {
        draw_game_end_screen(graphics, "You Lost");
      } else {

        if (game_a->finished_for_overflow || game_b->finished_for_overflow) {
          if (game_a->finished_for_overflow) {
            draw_game_end_screen(graphics, "Player [B] Won!");
          } else if (game_b->finished_for_overflow) {
            draw_game_end_screen(graphics, "Player [A] Won!");
          }
        } else {
          if (game_a->score > game_b->score) {
            draw_game_end_screen(graphics, "Player [A] Won! Reason: Score");
          } else if (game_a->score < game_b->score) {
            draw_game_end_screen(graphics, "Player [B] Won! Reason: Score");
          } else {
            draw_game_end_screen(graphics, "Tie!");
          }
        }
      }

      present_frame();
      usleep(1000 * FRAME_INTERVAL);
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
