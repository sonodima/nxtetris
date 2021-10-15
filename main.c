#include <stdio.h>
#include <unistd.h>

#include <curses.h>
#include <form.h>
#include <panel.h>

#include "engine/graphics.h"
#include "engine/controls.h"
#include "engine/types/color.h"

int main(void) {
    Graphics* graphics;
    Controls* controls;
    int is_running = 1;
    
    Color color;
    int toggle = 0;
    
    graphics = make_graphics();
    controls = make_controls();

    while (is_running) {
        update_controls(controls);
        begin_frame(graphics);
        
        if (controls->mouse_state == 1) {
            toggle = !toggle;
        }
         
        if (controls->pressed_key == KEY_UP) {
            is_running = 0;
        }
        
        /*
        fill_rect(graphics, (Rect){
            controls->mouse_position.x - 2, controls->mouse_position.y - 1,
            4, 2
        }, (Color){red, black, toggle ? darker : lighter});
        */
        
        color.alpha = darker;
        color.background = black;
        color.foreground = red;
        draw_text(graphics, "Test", controls->mouse_position, color, center, 1, 1);
        
        
        present_frame();
        usleep(1000 * 4);
    }
    
    free_controls(controls);
    free_graphics(graphics);
    return 0;
}
