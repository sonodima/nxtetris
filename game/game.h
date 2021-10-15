#ifndef game_h
#define game_h

#include <stdio.h>
#include <time.h>

#include "../engine/graphics.h"
#include "../engine/controls.h"
#include "tetromino.h"

typedef struct {
    Graphics* graphics;
    Controls* controls;
    Tetromino temp_tetronimo;
    Tetromino* tetrominoes;
    int tetrominoes_count;
    clock_t gravity_clock;
    PlacementState placement_state;
} Game;

Game* make_game(Graphics* graphics, Controls* controls);

void free_game(Game* game);

Tetromino spawn_game_tetromino(Game* game);

void remove_game_tetromino(Game* game);

void tick_game_gravity(Game* game);

void tick_game(Game* game);

#endif /* game_h */