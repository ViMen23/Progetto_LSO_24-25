#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include "app_context.h"


void context_add_player(app_context_t*, player_t*);
void context_add_lobby(app_context_t*, lobby_t*);
void context_add_game(app_context_t*, game_t*);

player_t* context_find_player(app_context_t*, unsigned long long int);
lobby_t* context_find_lobby(app_context_t*, unsigned long long int);
game_t* context_find_game(app_context_t*, unsigned long long int);

void context_remove_player(app_context_t*, unsigned long long int);
void context_remove_lobby(app_context_t*, unsigned long long int);
void context_remove_game(app_context_t*, unsigned long long int);

#endif