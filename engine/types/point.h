#ifndef engine_types_point_h
#define engine_types_point_h

typedef struct {
    int x;
    int y;
} Point;

int point_equals(Point first, Point second);

#endif /* engine_types_point_h */
