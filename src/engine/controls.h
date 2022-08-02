#ifndef NXTETRIS_SRC_ENGINE_CONTROLS_H_
#define NXTETRIS_SRC_ENGINE_CONTROLS_H_

#include "types/point.h"

/**
 * CLI controls handler.
 * Wraps ncurses for easier usage in immediate mode games.
 */
typedef struct {
	int enabled; /*!< General controls toggle. */
	int pressed_key; /*!< The pressed keyboard key. (updated after update_controls is called) */
	Point mouse_position; /*!< The position of the mouse, in client context. (updated after update_controls is called) */
	int mouse_state; /*!< The pressed mouse key. (updated after update_controls is called) */
} Controls;

/**
 * Creates and initializes the controls handler.
 * @return Pointer to the allocated controls handler.
 */
Controls* make_controls(void);

/**
 * Deallocates a controls handler.
 * Note that calling this function will break other existing controls handler,
 * and should only be called at program cleanup.
 * @param controls Pointer to the controls handler.
 */
void free_controls(Controls* controls);

/**
 * Updates the Controls structure with the state of the input devices.
 * @param controls Pointer to the controls handler.
 */
void update_controls(Controls* controls);

#endif
