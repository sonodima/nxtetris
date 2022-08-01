#ifndef NXTETRIS_SRC_ENGINE_TYPES_POINT_H_
#define NXTETRIS_SRC_ENGINE_TYPES_POINT_H_

typedef struct {
  int x;
  int y;
} Point;

int point_equals(Point first, Point second);

#endif
