#ifndef TASK_H
#define TASK_H

#include "../utils/task_queue.h"


typedef struct thread_pool {
    pthread_t *threads;
    int size;
    int shutdown; // Flag to shutdown pool
    task_queue_t queue;
} thread_pool_t;


int thread_pool_create(thread_pool_t*, int, int);

int thread_pool_add_task(thread_pool_t*, task_t);

void thread_pool_destroy(thread_pool_t*);

#endif