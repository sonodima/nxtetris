#include "graphics.h"

#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include <curses.h>

Graphics* make_graphics(void) {
    Graphics* graphics;
    
    /* Enable wchar console output. */
    setlocale(LC_ALL, "");
    
    /* Start curses mode. */
    initscr();
    
    /* Start the curses color functionality. */
    start_color();
    
    /* Hide the cursor. */
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
    
    /*
     Calculate the correct x-axis coordinate
     for text alignment.
     */
    length = (unsigned int)strlen(text);
    switch (alignment) {
        case center:
            point.x -= (int)(length / 4);
            break;
        case right:
            point.x -= length / 2;
            break;
        case left:
            break;
    }
    
    /* Enable styling attributes. */
    if (bold) {
        attron(A_BOLD);
    }
    if (underline) {
        attron(A_UNDERLINE);
    }
    
    /*
     Create and enable the color pair.
     Color pair range goes from 0 to 77.
     The first digit describes the foreground color,
     the second one describes the background color.
     */
    init_pair(color.foreground * 10 + color.background, color.foreground, color.background);
    attron(COLOR_PAIR(color.foreground * 10 + color.background));
    
    /*
     Draw the string at the calculated coordinates.
     The x-axis value is multiplied by two due to the
     graphics context being y=2x.
     */
    mvprintw(point.y, point.x * 2, text);
    
    /* Disable color attribute. */
    attroff(COLOR_PAIR(color.foreground * 10 + color.background));
    
    /* Disable styling attributes. */
    if (bold) {
        attroff(A_BOLD);
    }
    if (underline) {
        attroff(A_UNDERLINE);
    }
    
    /*
     The height of the string is always 1 unit,
     The width gets divided by 2 to factor for
     */
    size.height = 1;
    size.width = length / 2;
    return size;
}

void fill_rect(Graphics* graphics, Rect rect, Color color) {
    unsigned int i, j;
    
    if (graphics) {
        /*
         Create and enable the color pair.
         Color pair range goes from 0 to 77.
         The first digit describes the foreground color,
         the second one describes the background color.
         */
        init_pair(color.foreground * 8 + color.background, color.foreground, color.background);
        attron(COLOR_PAIR(color.foreground * 8 + color.background));
        
        /*
         Iterate all the units in the rect, and only
         draw those that would be drawn inside of the
         graphics context.
         */
        for (i = 0; i < rect.height; ++i) {
            /* Ignore rows that would exceed the graphics context. */
            if (i <= graphics->size.height + 1) {
                for (j = 0; j < rect.width; ++j) {
                    /* Ignore cols that would exceed the graphics context. */
                    if (j <= graphics->size.width + 1) {
                        mvprintw(rect.y + i, (rect.x + j) * 2,
                                 get_drawable_character(color.alpha));
                    }
                }
            }
        }
        
        /* Disable color attribute. */
        attroff(COLOR_PAIR(color.foreground + color.background * 8));
    }
}

void draw_rect(Graphics* graphics, Rect rect, Color color, unsigned int thickness) {
    /* Draw top side. */
    mvhline(rect.y, rect.x * 2, ACS_HLINE, rect.width * 2 );
    
    /* Draw left side. */
    mvvline(rect.y + 1, rect.x * 2 - 1, ACS_VLINE, rect.height - 1);
    
    /* Draw bottom side. */
    mvhline(rect.y + rect.height, rect.x * 2, ACS_HLINE, rect.width * 2);
    
    /* Draw right side. */
    mvvline(rect.y + 1, (rect.x + rect.width) * 2, ACS_VLINE, rect.height - 1);
    
    /* Draw top-left corner. */
    mvprintw(rect.y, rect.x * 2 - 1, "╭");
    
    /* Draw top-right corner. */
    mvprintw(rect.y, (rect.x + rect.width) * 2, "╮");
    
    /* Draw bottom-left corner. */
    mvprintw(rect.y + rect.height, rect.x * 2 - 1, "╰");
    
    /* Draw bottom-left corner. */
    mvprintw(rect.y + rect.height, (rect.x + rect.width) * 2, "╯");
}

Size get_window_size(void) {
    Size size;
    getmaxyx(stdscr, size.height, size.width);
    size.width /= 2;
    return size;
}

char* get_drawable_character(Alpha alpha) {
    char* tuple = "  ";

    /*
     Assign a tuple of characters to
     each alpha value.
     */
    switch (alpha) {
        case lighter:
            tuple = "░░";
            break;
        case light:
            tuple = "▒▒";
            break;
        case dark:
            tuple = "▓▓";
            break;
        case darker:
            tuple = "██";
            break;
        case transparent:
            break;
    }
    
    return tuple;
}
