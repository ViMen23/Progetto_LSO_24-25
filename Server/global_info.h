#ifndef GLOBAL_INFO_H
#define GLOBAL_INFO_H

#include "game.h"
#include "player.h"

typedef struct {
    lobby_t *lobbies;
    game_t *games;
    player_t *players;
} global_info_t;


static global_info_t global_info = { .lobbies = NULL, .games = NULL, .players = NULL };

void add_global_player(player_t*);
void add_global_lobby(lobby_t*);
void add_gloval_game(game_t*);

player_t* find_global_player(unsigned long long int);
lobby_t* find_global_lobby(unsigned long long int);
game_t* find_global_game(unsigned long long int);

void remove_global_player(unsigned long long int);
void remove_global_lobby(unsigned long long int);
void remove_global_game(unsigned long long int);



#endif