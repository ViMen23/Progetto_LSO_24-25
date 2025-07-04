#include "config.h"
#include "server/server.h"

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

static int signal_pipe[2];
    

void signal_handler(int sig) {
    printf("Stopped with signal %d", sig);
    char foo = 'a';
    write(signal_pipe[1], &foo, sizeof(foo));
}


int main(int argc, char *argv[])
{
    app_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if(pipe2(signal_pipe, O_NONBLOCK) == -1) {
        perror("failed to create pipe");
        return -1;
    }
    
    ctx.signal_pipe = signal_pipe;

    // Custom closure
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGHUP, signal_handler);
    signal(SIGQUIT, signal_handler);

    signal(SIGPIPE, SIG_IGN); // SIG_IGN = IGNORA IL SIGNAL


    int port = SERVER_PORT;
    if ( init_server(&ctx, port) < 0) {
        perror("Error starting the server");
        close(signal_pipe[0]);
        close(signal_pipe[1]);
        return -1;
    }

    run_server(&ctx);


    close(signal_pipe[0]);
    close(signal_pipe[1]);
}