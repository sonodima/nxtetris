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

#include "engine/audio.h"

#define FRAME_INTERVAL 12

int main(void) {
    Graphics* graphics;
    Controls* controls;
    Audio* audio;
    Sound* background_music;
    Sound* background_music2;
    Game* game;
    int is_running = 1;

    int toggle = 0;
    
    srand((unsigned int)time(0));
    
    audio = make_audio();
    background_music = make_sound(audio, "/Users/tommaso/Desktop/bg.aif");
    background_music2 = make_sound(audio, "/Users/tommaso/Desktop/bg.wav");
    start_sound(background_music);
    start_sound(background_music2);

    graphics = make_graphics();
    controls = make_controls();
    game = make_game(graphics, controls);

    while (is_running) {
        update_controls(controls);
        begin_frame(graphics);
        
        if (controls->mouse_state == 1) {
            toggle = !toggle;
            
            if (toggle) {
            } else {
               // stop_sound(background_music);
            }
        }
        
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
