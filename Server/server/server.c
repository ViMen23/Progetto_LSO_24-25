#include "server.h"
#include "config.h"
#include "../utils/utils.h"
#include "../context/context_manager.h"
#include "../player/player.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/epoll.h>



int init_server(app_context_t* ctx, int port) {

    if ( (ctx->server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
        perror("socket not created");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (bind(ctx->server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("server bind failure");
        close(ctx->server_fd);
        return -1;
    }

    // TO QUESTION THE SECOND ARGUMENT  
    if (listen(ctx->server_fd, LIS_QUEUE_MAX_SIZE) < 0) {
        perror("server listen failure");
        close(ctx->server_fd);
        return -1;
    }

    
    if ( (ctx->epoll_fd = epoll_create1(EPOLL_CLOEXEC)) == -1) {
        perror("epoll socket not created");
        close(ctx->server_fd);
        return -1;
    }

    if (add_epoll_interest(ctx->epoll_fd, ctx->server_fd, EPOLLIN | EPOLLET) == -1) {
        close(ctx->server_fd);
        close(ctx->epoll_fd);
        return -1;
    }

    if (add_epoll_interest(ctx->epoll_fd, ctx->signal_pipe[0], EPOLLIN) == -1) {
        close(ctx->server_fd);
        close(ctx->epoll_fd);
        return -1;
    }
    
    return 0;
}

void stop_server(app_context_t* ctx)
{
    return (ctx->server_running = 0) != 0;
}

void run_server(app_context_t* ctx)
{
    // why do I not need to do memset 0 to this?
    struct epoll_event events[MAX_EVENTS];

    printf("Server started\n");
    printf("Press CTRL+C to terminate the server\n");


    while(ctx->server_running) {

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
        int n = epoll_wait(ctx->epoll_fd, events, MAX_EVENTS, -1);


        if (n == -1) {
            perror("epoll wait failure");
            break;
        }


        for (int i = 0; i < n; ++i) {

            int fd = events[i].data.fd;
            uint32_t flags = events[i].events;

            // TODO it has to do something else, like removing the player and his lobbies from the global
            if ( (flags & EPOLLERR) || (flags & EPOLLHUP) ) {
                fprintf(stderr, "epoll error on fd %d\n", fd);
                
                removeNclean_epoll_interest(ctx->epoll_fd, fd);
                continue;
            }

            if (flags & EPOLLIN) {

                // TODO to implement
                if (fd == ctx->server_fd) {
                    int client_fd;

                    if ((client_fd = accept_new_player(ctx)) == -1) {
                        return -1;
                    }

                    const char* name = "ciao";
                    if (add_player(ctx, client_fd, name) == -1) {
                        return -1;
                    }
                    continue;
                }


                if (fd == ctx->signal_pipe[0]) {
                    printf("Received signal, stopping server...\n");
                    stop_server(ctx);
                    break;
                }

            }

            // TODO it has to do something else, like removing the player and his lobbies from the global
            if (flags & EPOLLRDHUP) {
                fprintf(stderr, "client associated to fd %d has closed the connection\n", fd);
                removeNclean_epoll_interest(ctx->epoll_fd, fd);
            }

        
            // TODO

        }

        // do some work
    }
}

static int accept_new_player(app_context_t *ctx)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    int fd = accept4(ctx->server_fd, (struct sockaddr *) &client_addr, &client_addr_size, SOCK_NONBLOCK);

    if (fd == -1) {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            perror("accept failure");
        return -1;
    }

    #if DEBUG
        printf("New Player connected and associated to fd %d\n", fd);
    #endif

    return fd;
}

static int add_player(app_context_t *ctx, int client_fd, const char *name)
{
    if ( add_epoll_interest(ctx->epoll_fd, client_fd, EPOLLIN | EPOLLRDHUP | EPOLLET) == -1)
        return -1;

    player_t *new_player = register_player(client_fd, name);

    if(!new_player)
        return -1;

    context_add_player(ctx, new_player);
    return 0;
}