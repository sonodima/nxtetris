#ifndef NXTETRIS_SRC_GAME_CPU_H_
#define NXTETRIS_SRC_GAME_CPU_H_

#include "game.h"

typedef struct {
  Game *game;
} CPU;

typedef struct {
  unsigned int rotation;
  unsigned int shape;
  unsigned int x_off;
} CPUAction;

CPU *make_cpu(Game *game);

void free_cpu(CPU *cpu);

CPUAction do_cpu_action(CPU *cpu);

#endif
