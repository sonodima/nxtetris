#include "collisions.h"

int test_box_collision(Rect rect, Point point) {
    return ((point.x >= rect.x && point.y >= rect.y) &&
            (point.x <= rect.x + rect.width &&
             point.y <= rect.y + rect.height));
}
