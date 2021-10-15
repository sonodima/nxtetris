#include "game.h"

#include <stdlib.h>

#include <curses.h>

#include "../utils/utils.h"

#define GRAVITY_TIME 0.002

Game* make_game(Graphics* graphics, Controls* controls) {
    Game* game;
    
    game = malloc(sizeof(Game));
    game->graphics = graphics;
    game->controls = controls;
    game->tetrominoes = malloc(sizeof(Tetromino) * 100);
    game->tetrominoes_count = 0;
    game->gravity_clock = clock();
    game->placement_state = placed;
    
    return game;
}

void free_game(Game* game) {
    
}

Tetromino spawn_game_tetromino(Game* game) {
    Tetromino tetromino;
    Point point;
    Color color;
    
    point.x = game->graphics->size.width / 2 - 2;
    point.y = 1;
    
    color.alpha = light;
    color.foreground = random_number(1, 6);
    color.background = black;
    
    tetromino.shape = random_number(0, TETROMINOES_COUNT - 1);
    tetromino.rotation = random_number(0, TETROMINOES_ROTATIONS - 1);
    tetromino.point = point;
    tetromino.color = color;
    tetromino.placement = placing;

    return tetromino;
}

void remove_game_tetromino(Game* game) {
    
}

void test_tetronimoes_overlap(Game* game, Tetromino tetronimo) {
    unsigned int i, j, t;
    unsigned int lower[4];
    unsigned int upper[4];

    /*
    for (i = 0; i < 4; ++i) {
        lower[i] = 0;
        for (j = 0; j < 4; ++j) {
            if (test_tetromino_collision(graphics, tet, <#Point point#>) j > lower[i]) {
                lower[i] = j;
            }
        }
    }
    
    for (t = 0; t < game->tetrominoes_count; ++t) {
        for (i = 0; i < 4; ++i) {
            upper[i] = 0;
            for (j = 0; j < 4; ++j) {
                if (j > upper[i]) {
                    upper[i] = j;
                }
            }
        }
        
        for (j = 0; j < 4; ++j) {
            if (
        }
        game->tetrominoes[i].point.x
    }
    */
}

void tick_game_gravity(Game* game) {
    unsigned int i;
    clock_t current_clock;
    double delta_time;
    
    current_clock = clock();
    delta_time = (double)(current_clock - game->gravity_clock) / CLOCKS_PER_SEC;
    
    if (delta_time > GRAVITY_TIME) {
        game->gravity_clock = current_clock;

        for (i = 0; i < game->tetrominoes_count; ++i) {
            if (game->tetrominoes[i].placement == falling) {
                ++game->tetrominoes[i].point.y;
                
                if (game->tetrominoes[i].point.y == game->graphics->size.height - 3 /* or collision is detected */) {
                    game->tetrominoes[i].placement = placed;
                    game->tetrominoes[i].color.alpha = darker;
                }
            }
        }
    }
}

/*
 
 if (controls->mouse_state == 1) {
     if (rotation < TETROMINOES_ROTATIONS - 1) {
         ++rotation;
     } else {
         rotation = 0;
     }
 }
  
 
 */

void tick_game(Game* game) {
    unsigned int i;
        
    switch (game->placement_state) {
        case placing:
            /*
             When the temporary tetronimo is in 'placing' state,
             handle mouse and keyoard input to decide where to drop it.
             Note that y=2x, so we need to move the tetronimo in the x
             axis two units a time.
             */
            game->temp_tetronimo.point.x = game->controls->mouse_position.x
                - (game->controls->mouse_position.x % 2) - 2;
            
            if (game->controls->mouse_state == 1) {
                game->temp_tetronimo.placement = falling;
                game->tetrominoes[game->tetrominoes_count++] = game->temp_tetronimo;

                
                game->placement_state = placed;
            }
            
            switch (game->controls->pressed_key) {
                case KEY_RIGHT:
                    /*
                     Handle right tetronimo rotation.
                     */
                    if (game->temp_tetronimo.rotation < TETROMINOES_ROTATIONS - 1) {
                        ++game->temp_tetronimo.rotation;
                    } else {
                        game->temp_tetronimo.rotation = 0;
                    }
                    break;
                    
                case KEY_LEFT:
                    /*
                     Handle left tetronimo rotation.
                     */
                    if (game->temp_tetronimo.rotation > 0) {
                        --game->temp_tetronimo.rotation;
                    } else {
                        game->temp_tetronimo.rotation = TETROMINOES_ROTATIONS - 1;
                    }
                    break;
            }
            break;
        case placed:
            game->placement_state = placing;
            game->temp_tetronimo = spawn_game_tetromino(game);
            break;
        case falling:
            break;
    }

    tick_game_gravity(game);
    
    /*
     Draw all the tetrominos in the context.
     */
    for (i = 0; i < game->tetrominoes_count; ++i) {
        draw_tetromino(game->graphics, game->tetrominoes[i]);
    }
    
    /*
     Draw the temporary tetronimo on top. (before placement)
     */
    if (game->placement_state == placing) {
        draw_tetromino(game->graphics, game->temp_tetronimo);
    }
}
