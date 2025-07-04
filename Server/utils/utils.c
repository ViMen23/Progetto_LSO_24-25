#include "utils.h"

#include <stdio.h>
#include <sys/epoll.h>

int add_epoll_interest(int epoll_fd, int fd, uint32_t flags)
{
    struct epoll_event ev;

    ev.events = flags;
    ev.data.fd = fd;

    if ( (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev)) == -1) {
        perror("epoll add interest failure");
        return -1;
    }

    return 0;
}

int removeNclean_epoll_interest(int epoll_fd, int fd) {
    if ( epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1 ) {
        perror("epoll del interest failure");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}
