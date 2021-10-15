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
            x,
            graphics->size.height / 2 - 1, 4, 2
        }, (Color){red, red, darker});
        
        fill_rect(graphics, (Rect){
            x + 5,
            graphics->size.height / 2 - 1, 4, 2
        }, (Color){green, red, darker});
        
        fill_rect(graphics, (Rect){
            x + 10,
            graphics->size.height / 2 - 1, 4, 2
        }, (Color){white, red, darker});
        
        fill_rect(graphics, (Rect){
            x + 15,
            graphics->size.height / 2 - 1, 4, 2
        }, (Color){blue, black, darker});
        */
        
        fill_rect(graphics, (Rect){
            controls->mouse_position.x - 2, controls->mouse_position.y - 1,
            4, 2
        }, (Color){red, black, toggle ? darker : lighter});
                
        present_frame();
        usleep(1000 * 4);
    }
    
    free_controls(controls);
    free_graphics(graphics);
    return 0;
}
