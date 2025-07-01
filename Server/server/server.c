#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#include "server.h"


int add_epoll_interest(int fd, uint32_t flags)
{
    struct epoll_event ev;

    ev.events = flags;
    ev.data.fd = fd;

    if ( (epoll_ctl(server.epoll_fd, EPOLL_CTL_ADD, fd, &ev)) == -1) {
        perror("epoll add interest failure");
        return -1;
    }

    return 0;
}

int removeNclean_epoll_interest(int fd) {
    if ( epoll_ctl(server.epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1 ) {
        perror("epoll del interest failure");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

bool stop_server()
{
    server.running = 0;

    return true;
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
                
                removeNclean_epoll_interest(fd);
                continue;
            }

            if(flags & EPOLLIN) {

                if(fd == server.server_fd) {
                    accept_new_player();
                    continue;
                }

            }

            if (flags & EPOLLRDHUP) {
                fprintf("client associated to fd %d has closed the connection\n", fd);
                removeNclean_epoll_interest(fd);
            }

        
            // TODO

        }

        // do some work
    }
}

static int accept_new_player() {
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;

    int fd = accept(server.server_fd, (struct sockaddr *) &client_addr, sizeof(client_addr));

    if (fd == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("accept failure");
        return -1;
    }

    printf("New Player connected and associated to fd %d\n", fd);

    return add_epoll_interest(fd, EPOLLIN | EPOLLRDHUP | EPOLLET);
}