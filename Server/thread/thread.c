#include "thread.h"
#include "../config.h"

#include <stdio.h>



static void* worker_function(void *arg)
{
    thread_pool_t* pool = (thread_pool_t*) arg;

    while (1) {
        task_t task;
        if (task_queue_take(&pool->queue, &task, &pool->shutdown) == -1)
            break;

        printf("Worker thread %ld processing task for player fd %d\n", pthread_self(), task.player_fd);

        // TODO
    }

    printf("Worker thread %ld is shutting down.\n", pthread_self());
    return NULL;
}


int thread_pool_create(thread_pool_t *pool, int thread_count, int queue_capacity)
{
    pool->threads = (pthread_t*) malloc(sizeof(pthread_t)*thread_count);

    if(!pool->threads) {
        fprintf(stderr, "Failed to allocate thread pool");
        return -1;
    }

    pool->size = thread_count;

    if (task_queue_create(&pool->queue, QUEUE_CAPACITY) == -1) {
        free(pool->threads);
        return -1;
    }    

    for (int i = 0; i < thread_count; ++i) {
        if (pthread_create(&pool->threads[i], NULL, worker_function, (void*) pool) != 0) {
            perror("failure to create a thread");
            _thread_pool_destroy(pool, i);
            return -1;
        }
    }

    #if DEBUG
        printf("Pool created successfully")
    #endif

    return 0;
}


int thread_pool_add_task(thread_pool_t *pool, task_t task)
{
    return task_queue_put(&pool->queue, task, &pool->shutdown);
}


static void _thread_pool_destroy(thread_pool_t *pool, int index)
{

    pool->shutdown = 1;

    task_queue_cond_broadcast(&pool->queue);

    for (int i = 0; i < index; ++i)
        pthread_join(pool->threads[i], NULL);

    task_queue_destroy(&pool->queue);
    free(pool->threads);
    pool->threads = NULL;

    pool->size = 0;
}


void thread_pool_destroy(thread_pool_t *pool)
{
    _thread_pool_destroy(pool, pool->size);

    #if DEBUG
        printf("Pool destroyed successfully")
    #endif
    
}
