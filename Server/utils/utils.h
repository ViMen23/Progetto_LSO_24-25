#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

int add_epoll_interest(int epoll_fd, int fd, uint32_t);
int removeNclean_epoll_interest(int epoll_fd, int fd);

#endif