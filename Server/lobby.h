#ifndef LOBBY_H
#define LOBBY_H

typedef struct {
    int creator_fd;
    int opponent_fd;
    unsigned long long int id;
    unsigned char mode;
} lobby_t;

#endif