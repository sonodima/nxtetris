#ifndef NXTETRIS_GAME_H_
#define NXTETRIS_GAME_H_

#include <stdio.h>
#include <time.h>

#include "../engine/media/graphics.h"
#include "../engine/media/audio.h"
#include "../engine/controls.h"
#include "../engine/types/rect.h"
#include "../engine/core/vector.h"
#include "tetromino.h"
#include "enums/game_state.h"
#include "enums/game_event.h"

typedef struct {
    Sound* bg;
    Sound* lock;
    Sound* rot_cl;
    Sound* rot_cc;
} Sounds;

typedef struct {
    Graphics* graphics;
    Controls* controls;
    Rect bounds;
    Tetromino temp_tetronimo;
    Vector* tetrominoes;
    clock_t gravity_clock;
    GameState state;
    unsigned int score;
    Sounds sounds;
} Game;

Game* make_game(Graphics* graphics, Controls* controls, Rect bounds);

void free_game(Game* game);

Tetromino spawn_game_tetromino(Game* game);

void remove_game_tetromino(Game* game);

void tick_game_gravity(Game* game);

void tick_game(Game* game);

void process_game_event(Game* game, GameEvent event, void* data);

#endif /* NXTETRIS_GAME_H_ */
