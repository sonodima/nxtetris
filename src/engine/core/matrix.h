#ifndef NXTETRIS_SRC_ENGINE_CORE_MATRIX_H_
#define NXTETRIS_SRC_ENGINE_CORE_MATRIX_H_

/**
 * Heap-allocated [row * col] matrix.
 * @see make_matrix
 */
typedef struct {
	unsigned int rows; /*!< Number of rows. */
	unsigned int cols; /*!< Number of columns. */
	int** data; /*!< The data of the matrix. (initialized to a 2-dimensional array of 0s by make_matrix) */
} Matrix;

/**
 * Allocates a matrix with the given size.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return Pointer to the allocated matrix.
 */
Matrix* make_matrix(unsigned int rows, unsigned int cols);

/**
 * Allocates a matrix with the given size, and performs a deep-copy of the given data into it.
 * @param data Pointer to the source data.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return Pointer to the allocated matrix.
 * @see make_matrix
 */
Matrix* make_matrix_with_data(const int* data, unsigned int rows, unsigned int cols);

/**
 * Deallocates a dynamic matrix.
 * @param matrix Pointer to the matrix.
 */
void free_matrix(Matrix* matrix);

#endif
