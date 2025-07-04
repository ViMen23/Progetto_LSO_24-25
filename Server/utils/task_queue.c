#include "task_queue.h"

#include <stdio.h>

int task_queue_create(task_queue_t *q, size_t cap)
{
    if (q == NULL || cap == 0)
        return -1;

    q->capacity = cap;
    q->size = 0;
    q->head = 0;
    q->tail = 0;

    q->tasks = (task_t*) malloc(sizeof(task_t)*cap);

    if (!q->tasks) {
        fprintf(stderr, "Failed to allocate the task_queue\n");
        return -1;
    }


    pthread_mutex_init(&q->mutex, NULL);

    if (pthread_cond_init(&q->not_empty, NULL) != 0) {
        fprinf(stderr, "Failed to initialize not empty condition variable");
        pthread_mutex_destroy(&q->mutex);
        free(q->tasks);
        return -1;
    }

    if (pthread_cond_init(&q->not_full, NULL) != 0) {
        fprinf(stderr, "Failed to initialize not full condition variable");
        pthread_mutex_destroy(&q->mutex);
        pthread_cond_destroy(&q->not_empty);
        free(q->tasks);
        return -1;
    }

    return 0;
}


int task_queue_put(task_queue_t *q, task_t task, int *signal)
{
    pthread_mutex_lock(&q->mutex);

    while (q->size == q->capacity && *signal == 0)
        pthread_cond_wait(&q->not_full, &q->mutex);
    
    if (*signal) {
        pthread_mutex_unlock(&q->mutex);
        return -1;
    }

    q->tasks[q->tail++] = task;
    q->tail %= q->capacity;
    q->size++;

    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);

    return 0;
}

int task_queue_take(task_queue_t *q, task_t *task, int *signal)
{
    pthread_mutex_lock(&q->mutex);

    while (!q->size && *signal == 0)
        pthread_cond_wait(&q->not_empty, &q->mutex);
        
    if(*signal) {
        pthread_mutex_unlock(&q->mutex);
        return -1;
    }

    *task  = q->tasks[q->head++];
    q->head %= q->capacity;
    q->size--;

    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);

    return 0;
}


void task_queue_destroy(task_queue_t *q)
{
    if (q == NULL)
        return;

    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
    free(q->tasks);

    q->capacity = 0;
    q->size = 0;
    q->head = 0;
    q->tail = 0;

    q->tasks = NULL;
}

void task_queue_cond_broadcast(task_queue_t *q)
{
    pthread_cond_broadcast(&q->not_empty);
    pthread_cond_broadcast(&q->not_full);
}