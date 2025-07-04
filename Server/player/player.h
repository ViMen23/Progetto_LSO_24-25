#ifndef PLAYER_H
#define PLAYER_H
#define MAX_LOBBY 5
#define NAME_LEN 32


#include "lobby.h"
#include "../external/uthash.h"


#include <stdatomic.h>

static atomic_ullong counter = 0;

typedef struct player {
    unsigned long long int id;
    char name[NAME_LEN];
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