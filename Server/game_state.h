#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum {
    NEW,
    WAITING,
    ONGOING,
    DONE
} en_game_status;


typedef enum {
    WIN,
    DRAW,
    LOSE,
    NONE
} en_game_result;

#endif