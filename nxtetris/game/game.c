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
    tetromino.point = game->temp_tetromino.point;
    tetromino.color = color;
    tetromino.bounds = get_tetromino_bounds(tetromino);
    tetromino.placement = PLACEMENT_STATE_PLACING;
    
    return tetromino;
}

void remove_game_tetromino(Game* game) {
    
}

void tick_game_gravity(Game* game) {
    unsigned int i, j, collided;
    clock_t current_clock;
    double delta_time;
    Tetromino* tetromino, *current;
    
    current_clock = clock();
    delta_time = (double)(current_clock - game->gravity_clock) / CLOCKS_PER_SEC;
    
    if (delta_time > GRAVITY_TIME) {
        game->gravity_clock = current_clock;
        
        for (i = 0; i < game->tetrominoes->count; ++i) {
            tetromino = vector_get(game->tetrominoes, i);
            
            if (tetromino->placement == PLACEMENT_STATE_FALLING) {
                ++tetromino->point.y;
                tetromino->color.alpha = ALPHA_LIGHTER;
                
                /*
                 Test current tetromino's collisions for all the other tetrominoes.
                 This may seem slow, but thanks to some optimizations inside of
                 test_tetromino_collision, it is only checked if the bounding boxes
                 overlap.
                 */
                collided = 0;
                for (j = 0; j < game->tetrominoes->count; ++j) {
                    current = vector_get(game->tetrominoes, j);

                    /*
                     Test collisions with y-offset = 1.
                     It is crucial to avoid checking collisions with the same tetromino.
                     */
                    if (tetromino != current && test_tetromino_collision(*tetromino, *current, 1)) {
                        collided = 1;
                    }
                }
                
                /*
                 Place the tetronimo if it either hit the bottom side of the game surface or the predicted tetronimo state
                 with offset=1 collided with another one.
                 */
                if (tetromino->point.y + tetromino->bounds.height + tetromino->bounds.y == game->bounds.y + game->bounds.height
                    || collided) {
                    tetromino->placement = PLACEMENT_STATE_PLACED;
                    tetromino->color.alpha = ALPHA_DARKER;
                    game->state = GAME_STATE_PLACED;
                }
            }
        }
    }
}

int is_line_full(Game* game, unsigned int index) {
    unsigned int i, j;
    Tetromino* current;
    
    for (i = 0; i < game->tetrominoes->count; ++i) {
        current = vector_get(game->tetrominoes, i);
        
        /*
         If the tetromino does not overlap on the y-axis, we are sure it does not appear in this line.
         */
        if (current->point.y - game->bounds.y + current->bounds.y > index) {
            continue;
        }
                
        /*
         Iterate all the cols of the current row.
         */
        for (j = 0; j < game->bounds.width; ++j) {
            
            /*
             If there is at least one point where a tetromino is not placed, the check fails.
             todo: game-relative to tetromino-relative space translation
             */
            if (!get_tetromino_value_at(*current, j, i)) {
                return 0;
            }
        }
    }
    
    return 1;
}

// todo: add check to ignore falling tetrominoes
unsigned int check_full_lines(Game* game) {
    unsigned int i, removed = 0;
    
    /*
     Iterate rows, starting from the bottom.
     */
    for (i = game->bounds.height - 1; i >= 0; --i) {
        if (is_line_full(game, i)) {
            
            
            
            
            ++removed;
        }
    }
    
    return removed;
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
    Color bounds_color;
        
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
            game->temp_tetromino = spawn_game_tetromino(game);
            start_sound(game->sounds.lock);
            game->state = GAME_STATE_PLACING;
            break;
            
        case GAME_STATE_FINISHED:
            break;
    }

    tick_game_gravity(game);
    
    /*
     Handle completed row removal.
     Score is incremented for each line removed.
     */
    switch (check_full_lines(game)) {
        case 1:
            game->score += 1;
            break;
        case 2:
            game->score += 3;
            break;
        case 3:
            game->score += 6;
            break;
        case 4:
            game->score += 12;
            break;
        default:
            break;
    }
        
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
        draw_tetromino(game->graphics, game->temp_tetromino);
    }
    
    /*
     Draw game bounds.
     */
    bounds_color.background = COLOR_WHITE;
    bounds_color.foreground = COLOR_BLACK;
    bounds_color.alpha = ALPHA_LIGHTER;
    draw_rect(game->graphics, game->bounds, bounds_color, 1);
    
    draw_score_text(game);
}

void process_game_event(Game* game, GameEvent event, void* data) {
    int i, temp;
    
    switch (event) {
        case GAME_EVENT_SET_X:
            if (game->state == GAME_STATE_PLACING) {
                /*
                 Destructure event argument.
                 */
                i = *(int*)data;
                
                /*
                 Limit placement for left border. (considering tetromino padding and bounds)
                 */
                temp = -game->temp_tetromino.bounds.x;
                if (i < temp) {
                    i = temp;
                }
                
                /*
                 Limit placement for right border. (considering tetromino padding and bounds)
                 */
                temp = game->bounds.width - game->temp_tetromino.bounds.x - game->temp_tetromino.bounds.width;
                if (i > temp) {
                    i = temp;
                }
                
                /*
                 Place the tetromino with the calculated game-relative coordinates.
                 */
                game->temp_tetromino.point.x = game->bounds.x + i;
                game->temp_tetromino.point.y = game->bounds.y + 1;
            }
            break;
            
        case GAME_EVENT_DROP:
            if (game->state == GAME_STATE_PLACING) {
                game->temp_tetromino.placement = PLACEMENT_STATE_FALLING;
                vector_push_back(game->tetrominoes, &game->temp_tetromino);
                game->state = GAME_STATE_FALLING;
            }
            break;
            
        case GAME_EVENT_ROT_CL:
            if (game->state == GAME_STATE_PLACING) {
                rotate_tetromino(&game->temp_tetromino, 1);
                start_sound(game->sounds.rot_cl);
            }
            break;
            
        case GAME_EVENT_ROT_CC:
            if (game->state == GAME_STATE_PLACING) {
                rotate_tetromino(&game->temp_tetromino, -1);
                start_sound(game->sounds.rot_cc);
            }
            break;
    }
}
