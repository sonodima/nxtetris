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
    game->tetrominoes = make_vector(sizeof(Tetromino));
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
    
    color.alpha = ALPHA_LIGHT;
    color.foreground = random_number(1, 6);
    color.background = COLOR_BLACK;
    
    tetromino.shape = random_number(0, TETROMINOES_COUNT - 1);
    tetromino.rotation = random_number(0, TETROMINOES_ROTATIONS - 1);
    tetromino.point = game->temp_tetronimo.point;
    tetromino.color = color;
    tetromino.bounds = get_tetromino_bounds(tetromino);
    tetromino.placement = PLACEMENT_STATE_PLACING;
    
    return tetromino;
}

void remove_game_tetromino(Game* game) {
    
}

void tick_game_gravity(Game* game) {
    unsigned int i;
    clock_t current_clock;
    double delta_time;
    Tetromino* tetromino;
    
    current_clock = clock();
    delta_time = (double)(current_clock - game->gravity_clock) / CLOCKS_PER_SEC;
    
    if (delta_time > GRAVITY_TIME) {
        game->gravity_clock = current_clock;
        
        for (i = 0; i < game->tetrominoes->count; ++i) {
            tetromino = vector_get(game->tetrominoes, i);
            
            if (tetromino->placement == PLACEMENT_STATE_FALLING) {
                ++tetromino->point.y;
                tetromino->color.alpha = ALPHA_LIGHTER;
                
                if (tetromino->point.y == game->graphics->size.height - tetromino->bounds.height /* or collision is detected */) {
                    tetromino->placement = PLACEMENT_STATE_PLACED;
                    tetromino->color.alpha = ALPHA_DARKER;
                    
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
    
    color.foreground = COLOR_WHITE;
    color.background = COLOR_BLACK;
    color.alpha = ALPHA_DARKER;
    
    draw_text(game->graphics, score_string, point, color, VERTICAL_ALIGNMENT_RIGHT, 1, 0);
}

void tick_game(Game* game) {
    unsigned int i;
        
    switch (game->state) {
        case GAME_STATE_PLACING:
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
    for (i = 0; i < game->tetrominoes->count; ++i) {
        draw_tetromino(game->graphics, *(Tetromino*)vector_get(game->tetrominoes, i));
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
    draw_rect(game->graphics, game->bounds, (Color){COLOR_WHITE, COLOR_BLACK, ALPHA_LIGHTER}, 1);
    
    
    draw_score_text(game);
}

void process_game_event(Game* game, GameEvent event, void* data) {
    int i;
    
    switch (event) {
        case GAME_EVENT_SET_X:
            if (game->state == GAME_STATE_PLACING) {
                i = *(int*)data;
                
                if (i < 0) {
                    i = 0;
                }
                
                if (i > game->bounds.width - game->temp_tetronimo.bounds.width) {
                    i = game->bounds.width - game->temp_tetronimo.bounds.width;
                }
                
                game->temp_tetronimo.point.x = game->bounds.x + i;
                game->temp_tetronimo.point.y = game->bounds.y + 1;
            }
            break;
            
        case GAME_EVENT_DROP:
            if (game->state == GAME_STATE_PLACING) {
                game->temp_tetronimo.placement = PLACEMENT_STATE_FALLING;
                vector_push_back(game->tetrominoes, &game->temp_tetronimo);
                game->state = GAME_STATE_FALLING;
            }
            break;
            
        case GAME_EVENT_ROT_CL:
            if (game->state == GAME_STATE_PLACING) {
                rotate_tetromino(&game->temp_tetronimo, 1);
                start_sound(game->sounds.rot_cl);
            }
            break;
            
        case GAME_EVENT_ROT_CC:
            if (game->state == GAME_STATE_PLACING) {
                rotate_tetromino(&game->temp_tetronimo, -1);
                start_sound(game->sounds.rot_cc);
            }
            break;
    }
}
