#include "collisions.h"

int test_box_collision(Rect rect, Point point) {
    Point temp_point;
    unsigned int i, j;
    
    for (i = 0; i < rect.width; ++i) {
        for (j = 0; j < rect.height; ++j) {
            temp_point.x = rect.x + i;
            temp_point.y = rect.y + j;
            
            if (point_equals(point, temp_point)) {
                return 1;
            }
        }
    }
    
    return 0;
}
