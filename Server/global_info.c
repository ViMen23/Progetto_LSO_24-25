#include "global_info.h"

// Doesn't check for uniqueness
void add_global_player(player_t *new_player)
{
    HASH_ADD(hh, global_info.players, id, sizeof(new_player->id), new_player);
}

// Doesn't check for uniqueness
void add_global_lobby(lobby_t *new_lobby)
{
    HASH_ADD(hh, global_info.lobbies, id, sizeof(new_lobby->id), new_lobby);
}

// Doesn't check for uniqueness
void add_global_game(game_t *new_game)
{
    HASH_ADD(hh, global_info.games, id, sizeof(new_game->id), new_game);
}


player_t* find_global_player(unsigned long long int id)
{
    player_t *player = NULL;
    HASH_FIND(hh, global_info.players, &id, sizeof(id), player);
    return player;
}

lobby_t* find_global_lobby(unsigned long long int id)
{
    lobby_t *lobby = NULL;
    HASH_FIND(hh, global_info.lobbies, &id, sizeof(id), lobby);
    return lobby;
}

game_t* find_global_game(unsigned long long int id)
{
    game_t *game = NULL;
    HASH_FIND(hh, global_info.games, &id, sizeof(id), game);
    return game;
}


void remove_global_player(unsigned long long int id)
{
    player_t *player;
    if (player = find_global_player(id))
        HASH_DELETE(hh, global_info.players, player);
}

void remove_global_lobby(unsigned long long int id)
{
    lobby_t *lobby;
    if (lobby = find_global_lobby(id))
        HASH_DELETE(hh, global_info.lobbies, lobby);
}

void remove_global_game(unsigned long long int id)
{
    game_t *game;
    if (game = find_global_game(id))
        HASH_DELETE(hh, global_info.games, game);
}