#ifndef NXTETRIS_SRC_ENGINE_CORE_VECTOR_H_
#define NXTETRIS_SRC_ENGINE_CORE_VECTOR_H_

#include <stdlib.h>

typedef struct {
  void* data;
  size_t data_size;
  size_t count;
  size_t max;
} Vector;

Vector* make_vector(size_t data_size);

void* vector_get(Vector* vector, size_t index);

void vector_set(Vector* vector, size_t size, void* data);

void vector_push_back(Vector* vector, void* data);

void vector_remove(Vector* vector, size_t index);

void vector_clear(Vector* vector);

#endif //NXTETRIS_SRC_ENGINE_CORE_VECTOR_H_
