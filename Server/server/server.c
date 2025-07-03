#include "server.h"
#include "../utils/epoll.h"
#include "config.h"
#include "../player/player.h"
#include "../global_info.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>



int init_server(int port) {
    memset(&server, 0, sizeof(server));

    if ( (server.server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
        perror("socket not created");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (bind(server.server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("server bind failure");
        close(server.server_fd);
        return -1;
    }

    if (listen(server.server_fd, LIS_QUEUE_MAX_SIZE) < 0) {
        perror("server listen failure");
        close(server.server_fd);
        return -1;
    }

    
    if ( (server.epoll_fd = epoll_create1(EPOLL_CLOEXEC)) == -1) {
        perror("epoll socket not created");
        close(server.server_fd);
        return -1;
    }

    if (add_epoll_interest(server.server_fd, server.epoll_fd, EPOLLIN | EPOLLET) == -1) {
        close(server.server_fd);
        close(server.epoll_fd);
        return -1;
    }

    server.running = 1;

    return 0;
}

int stop_server()
{
    return (server.running = 0) != 0;
}

int run_server()
{
    struct epoll_event events[MAX_EVENTS];

    printf("Server started\n");
    printf("Press CTRL+C to terminate the server\n");

    while(server.running) {

        // For now timeout is -1, but if we want to add a timeout to check for idle player it should be done like this
        /*
            // A timeout of 5 seconds (5000 ms) for periodic checks
            int timeout_ms = 5000;

            while (server.running) {
                int n = epoll_wait(server.epoll_fd, events, MAX_EVENTS, timeout_ms);

                if (n == 0) {
                    // Timeout reached, no network events
                    check_for_idle_players();
                    continue; // Go back to waiting
                }

                // Process the 'n' network events...
            }
        */
        int n = epoll_wait(server.epoll_fd, events, MAX_EVENTS, -1);


        if (n == -1) {
            perror("epoll wait failure");
            break;
        }


        for(int i = 0; i < n; ++i) {

            int fd = events[i].data.fd;
            uint32_t flags = events[i].events;

            if( (flags & EPOLLERR) || (flags & EPOLLHUP) ) {
                fprintf(stderr, "epoll error on fd %d\n", fd);
                
                removeNclean_epoll_interest(server.epoll_fd, fd);
                continue;
            }

            if(flags & EPOLLIN) {

                if(fd == server.server_fd) {
                    const char* name = "ciao"; // TODO to implement
                    accept_new_player(name);
                    continue;
                }

            }

            if (flags & EPOLLRDHUP) {
                fprintf(stderr, "client associated to fd %d has closed the connection\n", fd);
                removeNclean_epoll_interest(server.epoll_fd, fd);
            }

        
            // TODO

        }

        // do some work
    }
}

static int accept_new_player(const char* name) {
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;

    socklen_t client_size = sizeof(client_addr);
    int fd = accept4(server.server_fd, (struct sockaddr *) &client_addr, &client_size, SOCK_NONBLOCK);

    if (fd == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("accept failure");
        return -1;
    }

    printf("New Player connected and associated to fd %d\n", fd);
    
    if ( add_epoll_interest(server.epoll_fd, fd, EPOLLIN | EPOLLRDHUP | EPOLLET) == -1)
        return -1;

    player_t *new_player = register_player(fd, name);

    if(!new_player)
        return -1;

    
    add_global_player(new_player);
}