#include "controls.h"

#include <stdlib.h>

#include <curses.h>

Controls* make_controls(void) {
    Controls* controls;
    
    controls = malloc(sizeof(Controls));
    controls->enabled = 1;
    controls->pressed_key = -1;

    /*
     Disable keypress echoing.
     */
    noecho();
    
    /*
     Enable keypad mode. Also cause mouse events
     to report correctly.
     */
    keypad(stdscr, 1);
    
    /*
     Turn getch() into a non-blocking function.
     If no input is ready, ERR is returned.
     */
    nodelay(stdscr, 1);
    
    /*
     Make mouse events visible to the window.
     */
    mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, 0);
    
    /*
     Disable line buffering.
     */
    cbreak();
    
    /*
     Tell XTerm to report the mouse position.
     */
    printf("\033[?1003h\n");
    
    return controls;
}

void update_controls(Controls* controls) {
    int pressed_key;
    MEVENT mouse_event;
    
    if (controls->enabled) {
        pressed_key = getch();
        if (pressed_key != ERR) {
            if (pressed_key == KEY_MOUSE) {
                if (getmouse(&mouse_event) == OK) {
                    if (mouse_event.bstate & BUTTON1_PRESSED) {
                        controls->mouse_state = 1;
                    }
                    
                    controls->mouse_position.x = mouse_event.x;
                    controls->mouse_position.y = mouse_event.y;
                }
            } else {
                controls->pressed_key = pressed_key;
            }
        } else {
            controls->mouse_state = 0;
            controls->pressed_key = -1;
        }
    }
}
