#include <stdio.h>
#include <unistd.h>

#include <curses.h>

#include "engine/graphics.h"
#include "engine/controls.h"
#include "engine/types/color.h"

#include "game/tetrominoes.h"

#define FRAME_INTERVAL 4

int main(void) {
    Graphics* graphics;
    Controls* controls;
    int is_running = 1;
    
    Color color;
    int rotation = 0;
    
    graphics = make_graphics();
    controls = make_controls();

    while (is_running) {
        update_controls(controls);
        begin_frame(graphics);
        
        if (controls->mouse_state == 1) {
            if (rotation < TETROMINOES_ROTATIONS - 1) {
                ++rotation;
            } else {
                rotation = 0;
            }
        }
         
        if (controls->pressed_key == KEY_BACKSPACE) {
            is_running = 0;
        }
        
        color.alpha = lighter;
        color.background = black;
        color.foreground = red;
        
        if (test_tetromino_collision(1, rotation, (Point){4, 2}, controls->mouse_position)) {
            color.alpha = darker;
        }
        
        draw_tetromino(graphics, 1, rotation, (Point){4, 2}, color);

        
        present_frame();
        usleep(1000 * FRAME_INTERVAL);
    }
    
    free_controls(controls);
    free_graphics(graphics);
    return 0;
}
