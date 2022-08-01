#ifndef NXTETRIS_SRC_ENGINE_CORE_MATRIX_H_
#define NXTETRIS_SRC_ENGINE_CORE_MATRIX_H_

typedef struct {
  unsigned int rows;
  unsigned int cols;
  int** data;
} Matrix;

Matrix* make_matrix(unsigned int rows, unsigned int cols);

Matrix* make_matrix_with_data(const int* data, unsigned int rows, unsigned int cols);

void free_matrix(Matrix* matrix);

void print_matrix(Matrix* matrix);

#endif
