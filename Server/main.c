#include "config.h"
#include "server/server.h"

#include <signal.h>


static void signal_handler(int sig) {
    printf("Stopped with signal %d", sig);
    stop_server();
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


    run_server();
}