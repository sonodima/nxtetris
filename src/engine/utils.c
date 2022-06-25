#include "utils.h"

#include <stdlib.h>
#include <time.h>

static unsigned int seed_initialized = 0;

int random_number(int min, int max) {
  int low, high;

  if (seed_initialized == 0) {
    srand(time(NULL));
    seed_initialized = 1;
  }

  if (min < max) {
    low = min;
    high = max + 1;
  } else {
    low = max + 1;
    high = min;
  }

  return (rand() % (high - low)) + low;
}
