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

#define FRAME_INTERVAL 20

int main(void) {
    Graphics* graphics;
    Controls* controls;
    Game* game;
    unsigned int is_running = 1;
    int mouse_x = 0;
        
    srand((unsigned int)time(0));
    
    if (!initialize_audio()) {
        return -1;
    }
    
    graphics = make_graphics();
    controls = make_controls();
    game = make_game(graphics, controls, (Rect){4, 1, 10, 15});
    
    /*
     Main process loop.
     */
    while (is_running) {
        update_controls(controls);
        begin_frame(graphics);
        
        /*
         Update game position to screen center.
         */
        game->bounds.x = (graphics->size.width - game->bounds.width) / 2;
        game->bounds.y = (graphics->size.height - game->bounds.height) / 2;

        /*
         Handle tetronimo placing on mouse left click.
         */
        if (game->controls->mouse_state == 1) {
            process_game_event(game, GAME_EVENT_DROP, NULL);
        }
        
        switch (game->controls->pressed_key) {
            case KEY_RIGHT:
                /*
                 Handle clockwise tetronimo rotation.
                 */
                process_game_event(game, GAME_EVENT_ROT_CL, NULL);
                break;
                
            case KEY_LEFT:
                /*
                 Handle counter-clockwise tetronimo rotation.
                 */
                process_game_event(game, GAME_EVENT_ROT_CC, NULL);
                break;
        }
        
        /*
         Handle mouse input to place the temporary tetromino.
         The x-axis is limited by the bounds of the game.
         */
        mouse_x = game->controls->mouse_position.x - game->bounds.x;        
        process_game_event(game, GAME_EVENT_SET_X, &mouse_x);
        
        tick_game(game);
        
        draw_text(graphics, " nxtetris, by sonodima @ Università Ca' Foscari ",
                  (Point){0, graphics->size.height - 1}, (Color){black, white, transparent}, left, 1, 0);
        
        present_frame();
        usleep(1000 * FRAME_INTERVAL);
    }
    
    free_game(game);
    free_controls(controls);
    free_graphics(graphics);
    uninitialize_audio();
    return 0;
}
