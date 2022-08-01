#include "cpu.h"

#include <stdlib.h>

#include "board.h"
#include "game.h"
#include "tetromino.h"

CPU* make_cpu(Game* game) {
  CPU* cpu;

  cpu = malloc(sizeof(CPU));
  cpu->game = game;

  return cpu;
}

void free_cpu(CPU* cpu) {
  if (cpu) {
    free(cpu);
  }
}

CPUAction do_cpu_action(CPU* cpu) {
  CPUAction action;
  unsigned int r, s, x;
  Board* board;
  Tetromino temp_tetromino;
  Size tetromino_size;
  Point start_point;

  board = cpu->game->board;

  temp_tetromino.shape = 0;
  temp_tetromino.rotation = 0;

  start_point.y = 0;

  for (s = 0; s < TETROMINOES_COUNT; ++s) {
    for (r = 0; r < TETROMINOES_ROTATIONS; ++r) {
      tetromino_size = get_tetromino_size(temp_tetromino);
      for (x = 0; x < board->cols - tetromino_size.width; ++x) {
        start_point.x = (int)x;
        intersect_tetromino_with_board(board, temp_tetromino, start_point);
      }
    }
  }

  action.rotation = r;
  action.shape = s;
  action.x_off = x;
  return action;
}
