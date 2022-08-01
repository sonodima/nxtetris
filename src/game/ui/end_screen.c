#include "end_screen.h"

#include <curses.h>

void draw_end_screen(Graphics* graphics, const char* sub_message) {
  Color color;
  Point point;

  color.background = COLOR_BLACK;
  color.foreground = COLOR_RED;
  color.alpha = ALPHA_DARKER;

  point.x = graphics->size.width / 2;
  point.y = graphics->size.height / 2 - 1;
  draw_text(graphics, "Game Over", point, color, VERTICAL_ALIGNMENT_CENTER, 1, 0);

  color.foreground = COLOR_WHITE;

  point.y += 2;
  draw_text(graphics, sub_message, point, color, VERTICAL_ALIGNMENT_CENTER, 0, 0);

  point.x = 0;
  point.y = graphics->size.height - 1;
  draw_text(graphics, " [→] NEW GAME ", point, color, VERTICAL_ALIGNMENT_LEFT, 1, 0);
}
