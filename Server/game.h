#ifndef GAME_H
#define GAME_H
#include "game_state.h"

typedef struct {
    unsigned char board[3][3];
    unsigned char mode;
    unsigned int player_turn : 1;
    en_game_status status;
    en_game_result result;
    unsigned long long int id;
    int player1_fd;
    int player2_fd;
} game_t;

#endif