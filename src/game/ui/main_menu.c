#include "main_menu.h"

#include <stdlib.h>
#include <ncurses.h>

MainMenu* make_main_menu(Graphics* graphics) {
	MainMenu* main_menu;

	main_menu = malloc(sizeof(MainMenu));
	main_menu->graphics = graphics;
	main_menu->selected_mode = GAME_MODE_SP;

	return main_menu;
}

void free_main_menu(MainMenu* menu) {
	if (menu) {
		free(menu);
	}
}

void draw_main_menu(MainMenu* menu) {
	unsigned int i;
	Point point;
	Color color;
	int is_current;

	/* Draw the main menu's title */
	point.x = 1;
	point.y = 1;
	color.alpha = ALPHA_DARKER;
	color.foreground = COLOR_GREEN;
	color.background = COLOR_BLACK;
	draw_text(menu->graphics, "nxtetris - Select Game Mode", point, color, VERTICAL_ALIGNMENT_LEFT, 1, 0);

	/* Draw the list of game modes */
	point.x++;
	point.y += 2;
	color.background = COLOR_BLACK;
	for (i = 0; i < GAME_MODES_COUNT; ++i) {
		is_current = menu->selected_mode == game_modes[i].game_mode;
		color.foreground = is_current ? COLOR_YELLOW : COLOR_WHITE;
		draw_text(menu->graphics, game_modes[i].name, point, color, VERTICAL_ALIGNMENT_LEFT, is_current, is_current);
		point.y++;
	}

	/* Draw the instructions in the bottom of the page */
	point.x = 0;
	point.y = menu->graphics->size.height - 1;
	color.background = COLOR_BLACK;
	color.foreground = COLOR_WHITE;
	draw_text(menu->graphics, " [↑] UP  [↓] DOWN  [→] SELECT [←] EXIT", point, color, VERTICAL_ALIGNMENT_LEFT, 1, 0);
}
