#ifndef LOBBY_H
#define LOBBY_H

#include "../external/uthash.h"

typedef struct {
    int creator_fd;
    int opponent_fd;
    unsigned long long int id;
    unsigned char mode;
    UT_hash_handle hh;
} lobby_t;

#endif