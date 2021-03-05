#include "threads_pool.h"
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/* threads pool */
static pthread_t *pool;
static unsigned pool_size;
static unsigned create_p;
static unsigned recycle_p;
/* semaphore */
static sem_t *create_sem;
static sem_t *recycle_sem;
static const char *kThreadPoolCreateSem = "/create_sem";
static const char *kThreadPoolRecycleSem = "/recycle_sem";
/* recycle thread id */
static pthread_t recycle_tid;
/* signal recycle thread to exit */
static volatile sig_atomic_t recycle_sig;
#define RECYCLE_RUN 0
#define RECYCLE_TO_EXIT 1

/**
 * Producer of producer-consumer pattern.
 */
void create_thread(thread_fn thd_fn, void *arg) {
	sem_wait(create_sem);
	pthread_create(pool + create_p++ % pool_size, NULL, thd_fn, arg);
	sem_post(recycle_sem);
}

/**
 * Consumer of producer-consumer pattern.
 * Automatically recycle exited threads.
 */
static void *recycle_threads(void *arg) {
	while (RECYCLE_RUN == recycle_sig) {
		sem_wait(recycle_sem);
		pthread_join(*(pool + recycle_p++ % pool_size), NULL);
		sem_post(create_sem);
	}
	while (recycle_p < create_p) {
		sem_wait(recycle_sem);
		pthread_join(*(pool + recycle_p++ % pool_size), NULL);
		sem_post(create_sem);
	}
	pthread_exit(NULL);
}

/**
 * Create the threads pool, and run recycle_thread() thread.
 */
int threads_pool_init(unsigned size) {
	pool = malloc(size * sizeof *pool);
	pool_size = size;
	create_p = 0;
	recycle_p = 0;
	create_sem = sem_open(kThreadPoolCreateSem, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, size);
	recycle_sem = sem_open(kThreadPoolRecycleSem, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
	recycle_sig = RECYCLE_RUN;
	int code = pthread_create(&recycle_tid, NULL, recycle_threads, NULL);
	if (0 == code) return THREAD_POOL_INIT_OK;
	return THREAD_POOL_INIT_ERR;
}

/**
 * Dispose resources
 */
static void threads_pool_destroy(void) {
	free(pool);	
	sem_unlink(kThreadPoolCreateSem);
	sem_unlink(kThreadPoolRecycleSem);
}

/**
 * Signal recycle_threads() to exit, and wait it to finish it's job.
 */
void threads_pool_close(void) {
	recycle_sig = RECYCLE_TO_EXIT;
	pthread_join(recycle_tid,NULL);
	threads_pool_destroy();
}








