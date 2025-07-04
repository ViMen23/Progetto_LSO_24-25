#include "player.h"

#include <stdio.h>

player_t* register_player(int fd, const char *name) {

    player_t *new_player = (player_t*) malloc(sizeof(player_t));

    if(!new_player) {
        fprintf(stderr, "Failed to register new player (%s)\n", name);
        return NULL;
    }

    new_player->fd = fd;
    strncpy(new_player->name, name, sizeof(new_player->name) -1);
    new_player->name[sizeof(new_player->name)-1] = '\0';

    new_player->id = atomic_fetch_add(&counter, 1);
    new_player->is_playing = 0;
    new_player->lobby_count = 0;

    return new_player;
}

// !!! Method is not thread safe
int add_lobby(player_t *player, lobby_t *lobby) {
    if (player->lobby_count >= MAX_LOBBY) {
        fprintf(stderr, "Failed to add lobby to player (name = %s, id = %llu)", player->name, player->id);
        return -1;
    }

    player->lobby[player->lobby_count++] = *lobby;
    
    return 0;
}

// !!! Method is not thread safe
int remove_lobby(player_t *player, lobby_t *lobby)
{  
    unsigned short index = find_lobby_pos(player->lobby, lobby, player->lobby_count);

    if( index == player->lobby_count) {
        fprintf(stderr, "Failed to remove lobby to player (name = %s, id = %llu)", player->name, player->id);
        return -1;
    }

    player[index] = player[--player->lobby_count];
    return 0;
}

void start_playing(player_t *player) {
    player->is_playing = 1;
}

void stop_playing(player_t *player) {
    player->is_playing = 0;
}


// !!! Method is not thread safe
static unsigned short find_lobby_pos(lobby_t arr[], lobby_t *to_find, unsigned short size)
{
    unsigned short i;
    for(i = 0; i < size && arr[i].id != to_find->id; ++i);
    return i;
}