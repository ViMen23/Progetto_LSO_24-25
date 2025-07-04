#include "context_manager.h"
#include "../external/uthash.h"
#include "../player/player.h"
#include "../game/lobby.h"
#include "../game/game.h"


// Doesn't check for uniqueness
void context_add_player(app_context_t *ctx, player_t *new_player)
{
    HASH_ADD(hh, ctx->players, id, sizeof(new_player->id), new_player);
}

// Doesn't check for uniqueness
void context_add_lobby(app_context_t *ctx, lobby_t *new_lobby)
{
    HASH_ADD(hh, ctx->lobbies, id, sizeof(new_lobby->id), new_lobby);
}

// Doesn't check for uniqueness
void add_global_game(app_context_t *ctx, game_t *new_game)
{
    HASH_ADD(hh, ctx->games, id, sizeof(new_game->id), new_game);
}


player_t* context_find_player(app_context_t *ctx, unsigned long long int id)
{
    player_t *player = NULL;
    HASH_FIND(hh, ctx->players, &id, sizeof(id), player);
    return player;
}

lobby_t* context_find_lobby(app_context_t *ctx, unsigned long long int id)
{
    lobby_t *lobby = NULL;
    HASH_FIND(hh, ctx->lobbies, &id, sizeof(id), lobby);
    return lobby;
}

game_t* context_find_game(app_context_t *ctx, unsigned long long int id)
{
    game_t *game = NULL;
    HASH_FIND(hh, ctx->games, &id, sizeof(id), game);
    return game;
}


void context_remove_player(app_context_t *ctx, unsigned long long int id)
{
    player_t *player;
    if (player = context_find_player(ctx, id))
        HASH_DELETE(hh, ctx->players, player);
}

void context_remove_lobby(app_context_t *ctx, unsigned long long int id)
{
    lobby_t *lobby;
    if (lobby = context_find_lobby(ctx, id))
        HASH_DELETE(hh, ctx->lobbies, lobby);
}

void context_remove_game(app_context_t *ctx, unsigned long long int id)
{
    game_t *game;
    if (game = context_find_game(ctx, id))
        HASH_DELETE(hh, ctx->games, game);
}