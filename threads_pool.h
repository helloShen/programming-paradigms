#ifndef _THREADS_POOL_
#define _THREADS_POOL_

#define THREAD_POOL_INIT_OK 0
#define THREAD_POOL_INIT_ERR 1


typedef void *(*thread_fn)(void *arg);

/**
 * Create a new thread pool
 */
int threads_pool_init(unsigned size);

/**
 * Apply a new thread in threads pool.
 */
void create_thread(thread_fn thd_fn, void *arg);

/**
 * Notice no more threads to be created.
 * Threads pool can start to recycle and destroy resources.
 */
void threads_pool_close(void);

#endif // _THREADS_POOL_
