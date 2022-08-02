#ifndef NXTETRIS_SRC_ENGINE_UTILS_H_
#define NXTETRIS_SRC_ENGINE_UTILS_H_

/**
 * Generates a pseudo-random number in the given range.
 * This function automatically handles seed generation the first time it is called.
 * @param min Minimum included value.
 * @param max Maximum included value.
 * @return Generated pseudo-random number.
 */
int random_number(int min, int max);

/**
 * Limits a value to the given range.
 * @param value Input value.
 * @param min Minimum value limit.
 * @param max Maximum value limit.
 * @return Clamped value result.
 */
int clamp(int value, int min, int max);

#endif
