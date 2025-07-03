#ifndef SERVER_H
#define SERVER_H


#define MAX_EVENTS 16

typedef struct {
    int server_fd;
    int epoll_fd;
    int running: 1;
} Server;


extern Server server;

int init_server(int);
int stop_server();
int run_server(); 

#endif