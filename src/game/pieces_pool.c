#include "pieces_pool.h"

#include <stdlib.h>

PiecesPool *make_pieces_pool(unsigned int count) {
  PiecesPool* pool;
  pool = malloc(sizeof(PiecesPool));
  pool->count_per_piece = count;
  reset_pieces_pool(pool, count);
  return pool;
}

void free_pieces_pool(PiecesPool* pool) {
  if (pool) {
    free(pool);
  }
}

void reset_pieces_pool(PiecesPool *pool, unsigned int count) {
  unsigned int i;

  pool->count_per_piece = count;

  /* Initialize all piece counts to the maximum value */
  for (i = 0; i < TETROMINOES_COUNT; ++i) {
    pool->counts[i] = count;
  }
}

unsigned int has_pieces_left(PiecesPool* pool) {
  unsigned int i, accum;

  accum = 0;
  for (i = 0; i < TETROMINOES_COUNT; ++i) {
    accum += pool->counts[i];
  }

  return accum > 0;
}

unsigned int get_piece_count(PiecesPool *pool, unsigned int piece) {
  if (piece < 0 || piece >= TETROMINOES_COUNT) {
    return 0;
  }

  return pool->counts[piece];
}

unsigned int is_piece_available(PiecesPool* pool, unsigned int piece) {
  return get_piece_count(pool, piece) > 0;
}
