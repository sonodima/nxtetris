#include "main_menu.h"

#include <stdlib.h>
#include <curses.h>

MainMenu *make_main_menu(Graphics *graphics) {
  MainMenu *main_menu;
  main_menu = malloc(sizeof(MainMenu));
  main_menu->graphics = graphics;
  main_menu->selected_mode = GAME_MODE_SP;
  return main_menu;
}

void draw_main_menu(MainMenu *menu) {
  unsigned int i;
  Point point;
  Color color;
  int is_current;

  point.x = 1;
  point.y = 1;
  color.alpha = ALPHA_DARKER;
  color.foreground = COLOR_GREEN;
  color.background = COLOR_BLACK;
  draw_text(menu->graphics, "nxtetris - Select GameMode", point, color, VERTICAL_ALIGNMENT_LEFT, 1, 0);
  point.x++;
  point.y += 2;

  color.background = COLOR_BLACK;

  for (i = 0; i < GAME_MODES_COUNT; ++i) {
    is_current = menu->selected_mode == game_modes[i].game_mode;
    color.foreground = is_current ? COLOR_GREEN : COLOR_WHITE;
    draw_text(menu->graphics, game_modes[i].name, point, color, VERTICAL_ALIGNMENT_LEFT, is_current, is_current);
    point.y++;
  }

  point.x = 0;
  point.y = menu->graphics->size.height - 1;
  color.background = COLOR_BLACK;
  color.foreground = COLOR_WHITE;
  draw_text(menu->graphics, " [↑] UP  [↓] DOWN  [→] SELECT ", point, color, VERTICAL_ALIGNMENT_LEFT, 1, 0);
}
