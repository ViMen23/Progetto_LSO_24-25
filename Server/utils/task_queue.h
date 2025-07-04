#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <pthread.h>
#include <fcntl.h>


typedef struct {
    int player_fd;
    char command_buffer[1024];
    size_t command_len;
} task_t;


typedef struct {
    task_t *tasks;
    size_t capacity;
    size_t size;
    size_t head;
    size_t tail;
    
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} task_queue_t;


int task_queue_create(task_queue_t*, size_t);

int task_queue_put(task_queue_t*, task_t, int*);

int task_queue_take(task_queue_t*, task_t*, int*);

void task_queue_destroy(task_queue_t*);

#endif
