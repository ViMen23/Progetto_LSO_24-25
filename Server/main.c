#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>

#include "config.h"
#include "server/server.h"

static void signal_handler(int sig) {
    printf("Stopped with signal %d", sig);
    stop_server();
}

static int init_server(int port) {
    memset(&server, 0, sizeof(server));

    if ( (server.server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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

    if (add_epoll_interest(server.server_fd, EPOLLIN | EPOLLET) == -1) {
        close(server.server_fd);
        close(server.epoll_fd);
        return -1;
    }

    server.running = 1;

    return 0;
}

int main(int argc, char *argv[])
{
    int port = SERVER_PORT;

    // Custom closure
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGHUP, signal_handler);
    signal(SIGQUIT, signal_handler);

    signal(SIGPIPE, SIG_IGN); // SIG_IGN = IGNORA IL SIGNAL

    if ( init_server(port) < 0) {
        perror("Error starting the server");
        return -1;
    }
}