#ifndef GLOBAL_INFO_H
#define GLOBAL_INFO_H

#include "utils/list.h"
#include "game.h"
#include "player.h"

DEFINE_LIST_FUNCTIONS(Lobby, lobby_t)
DEFINE_LIST_FUNCTIONS(Player, player_t)
DEFINE_LIST_FUNCTIONS(Game, game_t)

typedef struct {
    LobbyList lobbies;
    GameList games;
    PlayerList players;
} global_info_t;

#endif