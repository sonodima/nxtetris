#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include <curses.h>

#include "engine/media/graphics.h"
#include "engine/media/audio.h"
#include "engine/controls.h"
#include "engine/types/color.h"

#include "game/tetromino.h"
#include "game/game.h"

#define FRAME_INTERVAL 12

int main(void) {
    Graphics* graphics;
    Controls* controls;
    Game* game;
    int is_running = 1;
        
    srand((unsigned int)time(0));
    
    if (!initialize_audio()) {
        return -1;
    }
    
    graphics = make_graphics();
    controls = make_controls();
    game = make_game(graphics, controls);
    
    /*
     Main process loop.
     */
    while (is_running) {
        update_controls(controls);
        begin_frame(graphics);
        
        tick_game(game);
        
        present_frame();
        usleep(1000 * FRAME_INTERVAL);
    }
    
    free_game(game);
    free_controls(controls);
    free_graphics(graphics);
    uninitialize_audio();
    return 0;
}
