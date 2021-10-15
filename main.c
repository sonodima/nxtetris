#include <stdio.h>
#include <unistd.h>

#include <curses.h>
#include <form.h>
#include <panel.h>

#include "engine/graphics.h"
#include "engine/controls.h"
#include "engine/types/color.h"

void a(void) {
    int x;
    int toggle = 0;
    Graphics* graphics;
    Controls* controls;
    
    x = 0;
    
    graphics = make_graphics();
    controls = make_controls();

    while(1) {
        update_controls(controls);
        begin_frame(graphics);
        
        
        if (controls->mouse_state == 1) {
            x--;
            toggle = !toggle;
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
        
       // draw_text(graphics, "Test", controls->mouse_position, (Color){white, black, darker}, center, 1, 1);
        
        present_frame();
        usleep(1000 * 4);
        
        /*
        Size a = get_window_size();
        draw_text(graphics, "Hello Curses!", (Point){a.width / 2, a.height / 2}, (Color){white, black, transparent}, center, 1, 1);
        
        rect.height = 10;
        rect.width = 20;
        rect.x = 0;
        rect.y = 0;
        fill_rect(graphics, rect, (Color){white, black, lighter});
        
        draw_text(graphics, "Overlapping Text", (Point){2, 8}, (Color){white, black, transparent}, left, 1, 1);
        */
    }
    
    free_graphics(graphics);

    
    /*
    win = newwin(8, 40, 3, 3);
    box(win, 0, 0);
    panel = new_panel(win);
    

    
    
    while(1) {
        
        c = wgetch(win);
        
        switch (c) {
            case KEY_MOUSE:
                if (getmouse(&event) == OK) {
                    if (event.bstate & BUTTON1_PRESSED) {
                        
                    }
                }
                break;
        }
         
        
        clear();
        
        if (x == 40) {
            x = 0;
        }
        
        mvprintw(1, x++, "Nigger");
        
        update_panels();
        doupdate();
        
        usleep(1000 * 16);
    }
     

        
    getch();
    endwin();*/
}

int main(void) {
    a();
/*
    initscr();
    start_color();
    raw();
    noecho();
    
    keypad(stdscr, 1);
    
    box(stdscr, 0, 0);
    
    init_pair(1, COLOR_RED, NONE);

    attron(COLOR_PAIR(1));
    mvwprintw(stdscr, 2, 1, "test");
    attroff(COLOR_PAIR(1));

    
    refresh();
    getch();
    endwin();
    
    
    
    */
    return 0;
}
