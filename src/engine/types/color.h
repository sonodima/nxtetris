#ifndef NXTETRIS_SRC_ENGINE_TYPES_COLOR_H_
#define NXTETRIS_SRC_ENGINE_TYPES_COLOR_H_

#include "engine/enums/alpha.h"

/**
 * Color of each object that is drawable by the CLI graphics manager.
 */
typedef struct {
	unsigned int foreground; /*!< Color of text and the virtual opacity mask. */
	unsigned int background; /*!< Color of the pixel's background. */
	Alpha alpha; /*!< Opacity value. */
} Color;

#endif
