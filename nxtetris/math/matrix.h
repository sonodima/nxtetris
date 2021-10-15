#ifndef math_matrix_h
#define math_matrix_h

typedef struct {
    unsigned int rows;
    unsigned int cols;
    int** data;
} Matrix;

Matrix* make_matrix(unsigned int rows, unsigned int cols);

Matrix* make_matrix_with_data(int* data, unsigned int rows, unsigned int cols);

void free_matrix(Matrix* matrix);

void print_matrix(Matrix* matrix);

void rotate_matrix(Matrix* matrix, int steps);

#endif /* math_matrix_h */
