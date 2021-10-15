#include "graphics.h"

#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include <curses.h>

Graphics* make_graphics(void) {
    Graphics* graphics;
    
    /*
     Enable wchar console output.
     */
    setlocale(LC_ALL, "");
    
    /*
     Start curses mode.
     */
    initscr();
    
    /*
     Start the curses color functionality.
     */
    start_color();
    
    /*
     Hide the cursor.
     */
    curs_set(0);
    
    graphics = malloc(sizeof(Graphics));
    graphics->size = get_window_size();

    return graphics;
}

void free_graphics(Graphics* graphics) {
    if (graphics) {
        free(graphics);
    }
}

void begin_frame(Graphics* graphics) {
    if (graphics) {
        graphics->size = get_window_size();
    }

    clear();
}

void present_frame(void) {
    refresh();
}

Size draw_text(Graphics* graphics, const char* text, Point point, Color color,
               VerticalAlignment alignment, int bold, int underline) {
    unsigned int length;
    Size size;
    
    length = (unsigned int)strlen(text);
    
    switch (alignment) {
        case center:
            point.x -= (int)(length / 4);
            break;
        case right:
            point.x -= length;
            break;
        case left:
            break;
    }
    
    if (bold) {
        attron(A_BOLD);
    }
    
    if (underline) {
        attron(A_UNDERLINE);
    }
    
    init_pair(color.foreground * 8 + color.background, color.foreground, color.background);
    attron(COLOR_PAIR(color.foreground * 8 + color.background));
    
    mvprintw(point.y, point.x * 2, text);
    
    attroff(COLOR_PAIR(color.foreground + color.background * 8));
        
    if (bold) {
        attroff(A_BOLD);
    }
    
    if (underline) {
        attroff(A_UNDERLINE);
    }
    
    size.height = 1;
    size.width = length;
    return size;
}

void draw_rect(Graphics* graphics, Rect rect, Color color) {
    unsigned int i, j;
    
    if (graphics) {
        init_pair(color.foreground * 8 + color.background, color.foreground, color.background);
        attron(COLOR_PAIR(color.foreground * 8 + color.background));
        
        for (i = 0; i < rect.height; ++i) {
            /* Ignore rows that would exceed the drawing context. */
            if (i <= graphics->size.height + 1) {
                for (j = 0; j < rect.width; ++j) {
                    /* Ignore cols that would exceed the drawing context. */
                    if (j <= graphics->size.width + 1) {
                        mvprintw(rect.y + i, (rect.x + j) * 2,
                                 get_drawable_character(color.alpha));
                    }
                }
            }
        }
        
        attroff(COLOR_PAIR(color.foreground + color.background * 8));
    }
}

Size get_window_size(void) {
    Size size;
    getmaxyx(stdscr, size.height, size.width);
    size.width /= 2;
    return size;
}

char* get_drawable_character(unsigned int alpha) {
    char* character = "  ";

    switch (alpha) {
        case lighter:
            character = "░░";
            break;
        case light:
            character = "▒▒";
            break;
        case dark:
            character = "▓▓";
            break;
        case darker:
            character = "██";
            break;
    }
    
    return character;
}
