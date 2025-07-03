#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>

int add_epoll_interest(int epoll_fd, int fd, uint32_t);
int removeNclean_epoll_interest(int epoll_fd, int fd);

#endif