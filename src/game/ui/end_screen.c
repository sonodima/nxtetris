#include "end_screen.h"

#include <string.h>
#include <ncurses.h>

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

void get_end_message(GameMode game_mode, Game* game_a, Game* game_b, char* buffer) {
	if (game_mode == GAME_MODE_SP) {
		sprintf(buffer, "Score: %d\n", game_a->score);
	} else if (game_a->finished_for_overflow) {
		strcpy(buffer, "Player [B] Won!");
	} else if (game_b->finished_for_overflow) {
		strcpy(buffer, "Player [A] Won!");
	} else if (game_a->score > game_b->score) {
		strcpy(buffer, "Player [A] Won! Reason: Score");
	} else if (game_a->score < game_b->score) {
		strcpy(buffer, "Player [B] Won! Reason: Score");
	} else {
		strcpy(buffer, "Tie!");
	}
}
