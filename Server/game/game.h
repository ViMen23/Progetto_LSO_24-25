#ifndef GAME_H
#define GAME_H

#include "game_state.h"
#include "../external/uthash.h"

typedef struct game {
    unsigned long long int id;
    unsigned char board[3][3];
    unsigned char mode;
    unsigned int player_turn : 1;
    en_game_status status;
    en_game_result result;
    int player1_fd;
    int player2_fd;
    UT_hash_handle hh;
} game_t;

#endif