#include "matrix.h"

#include <stdlib.h>

Matrix* make_matrix(unsigned int rows, unsigned int cols) {
	Matrix* matrix;
	int** data;
	unsigned int i;

	matrix = malloc(sizeof(Matrix));
	matrix->rows = rows;
	matrix->cols = cols;

	data = calloc(rows, sizeof(int*));
	for (i = 0; i < rows; ++i) {
		data[i] = calloc(cols, sizeof(int));
	}
	matrix->data = data;

	return matrix;
}

Matrix* make_matrix_with_data(const int* data, unsigned int rows, unsigned int cols) {
	Matrix* matrix = 0;
	unsigned int i, j;

	if (data) {
		matrix = make_matrix(rows, cols);
		for (i = 0; i < rows; ++i) {
			for (j = 0; j < cols; ++j) {
				matrix->data[i][j] = data[(i * cols) + j];
			}
		}
	}

	return matrix;
}

void free_matrix(Matrix* matrix) {
	unsigned int i;

	if (matrix) {
		/* Free all the memory allocated to the rows */
		for (i = 0; i < matrix->rows; ++i) {
			free(matrix->data[i]);
		}
		free(matrix->data);
		free(matrix);
	}
}
