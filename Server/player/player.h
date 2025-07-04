#ifndef PLAYER_H
#define PLAYER_H

#include "../game/lobby.h"
#include "../config.h"

typedef struct player {
    unsigned long long int id;
    char name[NAME_LEN];
    char token[B64_TOKEN_LEN];
    int fd;
    int is_playing: 1;
    lobby_t lobby[MAX_LOBBY];
    unsigned short lobby_count;
    UT_hash_handle hh;
} player_t;


player_t* register_player(int, const char*);
int add_lobby(player_t*, lobby_t*);
int remove_lobby(player_t*, lobby_t*);
void start_playing(player_t*);
void stop_playing(player_t*);

#endif