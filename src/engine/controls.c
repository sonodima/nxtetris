#include "controls.h"

#include <stdlib.h>

#include <ncurses.h>

Controls* make_controls(void) {
  Controls* controls;

  /* Disable keypress echoing */
  noecho();

  /*
   * Enable keypad mode.
   * Also cause mouse events to report correctly.
   */
  keypad(stdscr, 1);

  /*
   * Turn getch() into a non-blocking function.
   * If no input is ready, ERR is returned.
   */
  nodelay(stdscr, 1);

  /* Make mouse events visible to the window */
  mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, 0);

  /* Disable line buffering */
  cbreak();

  /* Tell XTerm to report the mouse position */
  printf("\033[?1003h\n");

  controls = malloc(sizeof(Controls));
  controls->enabled = 1;
  controls->pressed_key = -1;
  controls->mouse_state = 0;

  return controls;
}

void free_controls(Controls* controls) {
  if (controls) {
    free(controls);
  }

  /* Tell XTerm to stop reporting the mouse position */
  printf("\033[?1003l\n");
}

void update_controls(Controls* controls) {
  int pressed_key;
  MEVENT mouse_event;

  if (controls && controls->enabled) {
    pressed_key = getch();
    if (pressed_key != ERR) {
      if (pressed_key == KEY_MOUSE) {
        if (getmouse(&mouse_event) == OK) {
          if (mouse_event.bstate & BUTTON1_PRESSED) {
            controls->mouse_state = 1;
          }

          /* Mouse X is divided by two because, in the screen space, y=2x */
          controls->mouse_position.x = mouse_event.x / 2;
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
