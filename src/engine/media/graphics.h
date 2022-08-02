#ifndef NXTETRIS_SRC_ENGINE_MEDIA_GRAPHICS_H_
#define NXTETRIS_SRC_ENGINE_MEDIA_GRAPHICS_H_

#include "engine/enums/vertical_alignment.h"
#include "engine/types/color.h"
#include "engine/types/point.h"
#include "engine/types/rect.h"
#include "engine/types/size.h"

/**
 * CLI graphics manager.
 * Wraps ncurses for easier usage in immediate mode games.
 */
typedef struct {
	Size size; /*!< Size of the graphics context. (updated after each call of begin_frame) */
} Graphics;

/**
 * Creates and initializes the graphics context.
 * Program locale is also updated, to enable wchar support.
 * @return Pointer to the graphics context.
 */
Graphics* make_graphics(void);

/**
 * Releases the memory allocated for the graphics context.
 * @param graphics pointer to the graphics context.
 */
void free_graphics(Graphics* graphics);

/**
 * Clears the terminal window and updates the size property.
 * @param graphics pointer to the graphics context.
 */
void begin_frame(Graphics* graphics);

/**
 * Refreshes the terminal window, pushing any changes
 * made to the graphics buffer.
 */
void present_frame(void);

/**
 * Adds a string to the graphics buffer.
 * @param graphics pointer to the graphics context.
 * @param text text to draw.
 * @param point coordinates at which to draw the text.
 * @param color color structure of the draw operation.
 * @param alignment vertical string alignment.
 * @param bold text weight control.
 * @param underline text underline control.
 * @return The size of the drawn string.
 */
Size draw_text(Graphics* graphics, const char* text, Point point, Color color,
							 VerticalAlignment alignment, int bold, int underline);

/**
 * Adds a filled rectangle to the graphics buffer.
 * @param graphics pointer to the graphics context.
 * @param rect geometric rectangle structure.
 * @param color color structure of the draw operation.
 * @see draw_rect()
 */
void fill_rect(Graphics* graphics, Rect rect, Color color);

/**
 * Adds an empty rectangle to the graphics buffer.
 * More optimized than the box() function in curses.
 * @param graphics pointer to the graphics context.
 * @param rect geometric rectangle structure.
 * @param color color structure of the draw operation.
 * @see fill_rect()
 */
void draw_rect(Graphics* graphics, Rect rect, Color color);

/**
 * Obtains the size of the terminal window.
 * The x-axis is divided by two due to the graphics coordinates context being y=2x.
 * @return The size (in cells) of the terminal.
 */
Size get_window_size(void);

#endif
