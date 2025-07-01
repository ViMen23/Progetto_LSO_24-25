#ifndef PLAYER_H
#define PLAYER_H
#include "lobby.h"
#define LOBBY_NUM 5
#define NAME_LEN 32

typedef struct {
    int socket;
    char name[NAME_LEN];
    unsigned long long int id;
    bool is_playing: 1;
    lobby_t lobby[LOBBY_NUM];
} player_t;

#endif