#ifndef SERVER_H
#define SERVER_H

#include <stdbool.h>
#include <sys/epoll.h>

#define MAX_EVENTS 16

typedef struct {
    int server_fd;
    int epoll_fd;
    bool running: 1;
} Server;


extern Server server;


int add_epoll_interest(int, uint32_t);

bool stop_server();

int run_server(); 

#endif