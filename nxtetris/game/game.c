#include "game.h"

#include <stdlib.h>

#include <curses.h>

#include "../utils/utils.h"

/*
 Delay between gravity clocks.
 The lower, the faster.
 */
#define GRAVITY_TIME 0.0005

void load_sounds(Game* game) {
    game->sounds.bg = make_sound("audio/bg.aif", 1);
    game->sounds.lock = make_sound("audio/lock.aif", 0);
    game->sounds.rot_cl = make_sound("audio/rot_cl.aif", 0);
    game->sounds.rot_cc = make_sound("audio/rot_cc.aif", 0);
}

Game* make_game(Graphics* graphics, Controls* controls, Rect bounds) {
    Game* game;
    
    game = malloc(sizeof(Game));
    game->graphics = graphics;
    game->controls = controls;
    game->bounds = bounds;
    game->tetrominoes = malloc(sizeof(Tetromino) * 100);
    game->tetrominoes_count = 0;
    game->gravity_clock = clock();
    
    /*
     Initialized with GAME_STATE_PLACED so that a new
     temp tetromino is spawned on launch.
     */
    game->state = GAME_STATE_PLACED;
    
    game->score = 0;
    
    /*
     Load all the sound assets and start
     the background music.
     */
    load_sounds(game);
    start_sound(game->sounds.bg);
    
    return game;
}

void free_game(Game* game) {
    if (game) {
        free(game);
    }
}

Tetromino spawn_game_tetromino(Game* game) {
    Tetromino tetromino;
    Color color;
    
    color.alpha = light;
    color.foreground = random_number(1, 6);
    color.background = black;
    
    tetromino.shape = random_number(0, TETROMINOES_COUNT - 1);
    tetromino.rotation = random_number(0, TETROMINOES_ROTATIONS - 1);
    tetromino.point = game->temp_tetronimo.point;
    tetromino.color = color;
    tetromino.placement = placing;

    return tetromino;
}

void remove_game_tetromino(Game* game) {
    
}

void test_tetronimoes_overlap(Game* game, Tetromino tetronimo) {
    /*
    unsigned int i, j, t;
    unsigned int lower[4];
    unsigned int upper[4];

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
                game->tetrominoes[i].color.alpha = lighter;
                
                if (game->tetrominoes[i].point.y == game->graphics->size.height - 3 /* or collision is detected */) {
                    game->tetrominoes[i].placement = placed;
                    game->tetrominoes[i].color.alpha = darker;
                    
                    game->state = GAME_STATE_PLACED;
                }
            }
        }
    }
}

void draw_score_text(Game* game) {
    char score_string[32];
    Point point;
    Color color;

    sprintf(score_string, "Score: %d", game->score);
    
    point.x = game->bounds.x + game->bounds.width - 1;
    point.y = game->bounds.y;
    
    color.foreground = white;
    color.background = black;
    color.alpha = darker;
    
    draw_text(game->graphics, score_string, point, color, right, 1, 0);
}

void tick_game(Game* game) {
    unsigned int i;
        
    switch (game->state) {
        case GAME_STATE_PLACING:
            /*
             In placing state, handle mouse input to
             decide where to drop the temporary tetromino.
             The x-axis is limited by the bounds of the game.
             */
            i = game->controls->mouse_position.x - 1;
            if (i > game->bounds.x + game->bounds.width) {
                i = game->bounds.x + game->bounds.width;
            }
            game->temp_tetronimo.point.x = i;
            
            
            
            if (game->controls->mouse_state == 1) {
                game->temp_tetronimo.placement = falling;
                game->tetrominoes[game->tetrominoes_count++] = game->temp_tetronimo;
                
                game->state = GAME_STATE_FALLING;
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
                    start_sound(game->sounds.rot_cl);
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
                    start_sound(game->sounds.rot_cc);
                    break;
            }
            break;
        case GAME_STATE_FALLING:
            break;
        case GAME_STATE_PLACED:
            /*
             When the state is GAME_STATE_PLACED, the
             lock sound is played and a new temp tetromino is
             created.
             The state switches back to GAME_STATE_PLACING.
             */
            game->temp_tetronimo = spawn_game_tetromino(game);
            start_sound(game->sounds.lock);
            game->state = GAME_STATE_PLACING;
            break;
        case GAME_STATE_FINISHED:
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
    if (game->state == GAME_STATE_PLACING) {
        draw_tetromino(game->graphics, game->temp_tetronimo);
    }
    
    /*
     Draw game bounds.
     */
    draw_rect(game->graphics, game->bounds, (Color){white, black, lighter}, 1);
    
    draw_score_text(game);
}
