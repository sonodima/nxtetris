#ifndef NXTETRIS_ENGINE_TYPES_POINT_H_
#define NXTETRIS_ENGINE_TYPES_POINT_H_

typedef struct {
    int x;
    int y;
} Point;

int point_equals(Point first, Point second);

#endif /* NXTETRIS_ENGINE_TYPES_POINT_H_ */
