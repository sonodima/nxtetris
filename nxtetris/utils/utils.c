#include "utils.h"

#include <stdlib.h>

int random_number(int min, int max) {
    int low, high;
    
    if (min < max) {
        low = min;
        high = max + 1;
    } else {
        low = max + 1;
        high = min;
    }
    
    return (rand() % (high - low)) + low;
}
