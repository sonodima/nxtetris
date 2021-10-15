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
    Game* game;
    int is_running = 1;
    
    Sound* cringe_music;
    Sound* background_music;
    
    srand((unsigned int)time(0));
    
    audio = make_audio();

    /*
     Load sound assets.
     */
    cringe_music = make_sound(audio, "/Users/tommaso/Desktop/bg.aif");
    background_music = make_sound(audio, "/Users/tommaso/Desktop/bg.wav");

    start_sound(cringe_music);
    start_sound(background_music);
    
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
    
    free_sound(background_music);
    free_sound(cringe_music);
    free_audio(audio);
    return 0;
}
