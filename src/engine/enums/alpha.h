#ifndef NXTETRIS_SRC_ENGINE_ENUMS_ALPHA_H_
#define NXTETRIS_SRC_ENGINE_ENUMS_ALPHA_H_

/**
 * Virtual pixel opacity.
 * This is implemented using unicode characters.
 */
typedef enum {
	ALPHA_TRANSPARENT,
	ALPHA_LIGHTER,
	ALPHA_LIGHT,
	ALPHA_DARK,
	ALPHA_DARKER
} Alpha;

#endif
