#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

// forward declarations
typedef struct player player_t; 
typedef struct lobby lobby_t;
typedef struct game game_t;

typedef struct {
    int *signal_pipe;
    int server_fd;
    int epoll_fd;
    int server_running;

    player_t *players;
    lobby_t *lobbies;
    game_t *games;
} app_context_t;

#endif