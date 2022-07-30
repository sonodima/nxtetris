#ifndef NXTETRIS_SRC_GAME_CPU_H_
#define NXTETRIS_SRC_GAME_CPU_H_

#include "game.h"

typedef struct {
  Game *game;
} CPU;

CPU *make_cpu(Game *game);

void free_cpu(CPU *cpu);

void do_cpu_action(CPU *cpu);

#endif
