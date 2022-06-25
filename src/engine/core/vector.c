#include "vector.h"

#include <string.h>

/*
 * todo: Check whether this is really needed.
 */

Vector* make_vector(size_t data_size) {
  Vector* vector;

  vector = malloc(sizeof(Vector));
  vector->data_size = data_size;
  vector->count = 0;
  vector->max = 16;
  vector->data = calloc(vector->max, vector->data_size);

  return vector;
}

void* vector_get(Vector* vector, size_t index) {
  if (index < 0 || index >= vector->count) {
    return NULL;
  }

  return (char*)vector->data + vector->data_size * index;
}

void vector_set(Vector* vector, size_t index, void* data) {
  /*
   todo: Add checks...
   */
  memcpy((char*)vector->data + vector->data_size * index, data, vector->data_size);
}

void vector_push_back(Vector* vector, void* data) {
  if (vector->count == vector->max) {
    vector->max *= 2;
    vector->data = realloc(vector->data, vector->data_size * vector->max);
  }

  vector_set(vector, vector->count++, data);
}

void vector_remove(Vector* vector, size_t index) {
  if (index < 0 || index >= vector->count) {
    return;
  }

  /*
   Not optimal in terms of performance, but we need to keep
   the order of the vector, so this is a good-enough
   tradeoff.
   */
  memmove((char*)vector->data + vector->data_size * index,
          (char*)vector->data + vector->data_size * (index + 1),
          (vector->count-- - index) * vector->data_size);
}

void vector_clear(Vector* vector) {
  vector->count = 0;
  vector->max = 16;

  free(vector->data);
  vector->data = calloc(vector->max, vector->data_size);
}
