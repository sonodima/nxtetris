#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include <curses.h>

#include "engine/graphics.h"
#include "engine/controls.h"
#include "engine/types/color.h"

#include "game/tetromino.h"
#include "game/game.h"

#define FRAME_INTERVAL 4

int main(void) {
    Graphics* graphics;
    Controls* controls;
    Game* game;
    int is_running = 1;

    srand((unsigned int)time(0));
    
    graphics = make_graphics();
    controls = make_controls();
    game = make_game(graphics, controls);

    while (is_running) {
        update_controls(controls);
        begin_frame(graphics);
        
        tick_game(game);
        // draw_rect(graphics, (Rect){controls->mouse_position.x, controls->mouse_position.y, 1, 1}, (Color){red, black, darker});
        
        present_frame();
        usleep(1000 * FRAME_INTERVAL);
    }
    
    free_game(game);
    free_controls(controls);
    free_graphics(graphics);
    return 0;
}
