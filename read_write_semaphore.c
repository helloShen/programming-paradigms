/**
 * read_write_semaphore.c is a simple implementation of producor-consumer by using semaphore.
 * ------------------------------------------------------------------------------------------
 * Check the following documents for more informations:
 * ==> https://linux.die.net/man/7/sem_overview
 * ==> https://linux.die.net/man/3/sem_open
 * ==> http://www.qnx.com/developers/docs/6.5.0/index.jsp?topic=%2Fcom.qnx.doc.neutrino_lib_ref%2Fs%2Fsem_open.html
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef _RW_PARAMS_
#define _RW_PARAMS_

typedef struct {
	char *buffer;
	int buffer_p;
	int buffer_size;
	unsigned count;
	sem_t *rsem;
	sem_t *wsem;
} rw_params;

void new_rw_params(rw_params *ps, char *buffer, int bufferp, int buffersize, unsigned count, sem_t *rsem, sem_t *wsem) {
	ps->buffer = buffer;
	ps->buffer_p = bufferp;
	ps->buffer_size = buffersize;
	ps->count = count;
	ps->rsem = rsem;
	ps->wsem = wsem;
}

#endif // _RW_PARAMS_


void *read(void *args_addr) {
	char c;
	rw_params *params = (rw_params *)args_addr;
	while (1) {
		sem_wait(params->rsem); // wait for decrementing read semaphore by 1, if it's greater than 0.
		c = params->buffer[params->buffer_p++ % params->buffer_size];
		sem_post(params->wsem); // increment write semaphore by 1
		params->count++;
		if (c != '\0') {
			printf("%c", c);
		} else {
			printf("\n"); break;
		}
	}
	pthread_exit((void *)&params->count); // pass length of string to pthread_join() function
}

/**
 * Get the infraction part of current time in nanoseconds since epoch.
 * Used as random seed for srand() function.
 */
unsigned getnano_uint(void) {
	long nano;
	unsigned uint_nano;
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	nano = spec.tv_nsec;
	uint_nano = (unsigned)nano;	// 4 bytes unsigned int is large enough to hold 9 digits
	return uint_nano;
}

void *write(void *args_addr) {
	rw_params *params = (rw_params *)args_addr;
	int r;
	char c;
	srand(getnano_uint()); // use part of system current time as random seed.
	while (1) {
		r = rand();
		c = (r % 20 == 0)? '\0' : 'a' + r % 26; // expect a string of length 20 in average
		sem_wait(params->wsem); // wait for decrementing write semaphore by 1, if it's greater than 0.
		params->buffer[params->buffer_p++ % params->buffer_size] = c;
		sem_post(params->rsem); // increment read semaphore by 1
		if (c == '\0') break;
	}
	pthread_exit(NULL);
}

unsigned run(void) {
	size_t bsize = 8;
	char buffer[bsize]; 			// buffer of 8 bytes.

	pthread_t rid, wid;				// read and write thread id
	sem_t *r_sem, *w_sem;			// read and write semaphore
	rw_params r_params, w_params;	// read(), write() parameters structure
	int rerr, werr;					// thread_create() return error code
	void *rexit; 					// store thread exit value

	/* 
	 * ==> 0. MAX_OS doesn't support sem_init(), sem_destroy() and sem_getvalue().
	 * We can only use sem_open(), sem_close() instead.
	 * ==> 1. sem_open() creates a new POSIX semaphore or opens an existing semaphore.
	 * ==> 2. A named semaphore is identified by a name of the form "/somename".
	 * that is, a null-terminated string of up to NAME_MAX-4 (i.e., 251) characters 
	 * consisting of an initial slash. 
	 * ==> 3. If O_CREAT is specified in oflag, then the semaphore is created if it does 
	 * not already exist. 
	 * ==> 4. If both O_CREAT and O_EXCL are specified in oflag, then an error is 
	 * returned if a semaphore with the given name already exists.
	 * ==> 5. The semaphore's mode (just like file modes). For portability, you should 
	 * set the read, write, and execute bits to the same value. An easy way of doing this 
	 * is to use the constants from <sys/stat.h>:
	 *		S_IRWXG for group access.
	 *		S_IRWXO for other's access.
	 *		S_IRWXU for your own access.
	 */
	r_sem = sem_open("/rsem", O_CREAT, S_IRWXG, 0);
	w_sem = sem_open("/wsem", O_CREAT, S_IRWXG, bsize); // write() function is ahead of read() by maximum 8 bytes.

	new_rw_params(&r_params, buffer, 0, bsize, 0, r_sem, w_sem);
	new_rw_params(&w_params, buffer, 0, bsize, 0, r_sem, w_sem);

	rerr = pthread_create(&rid, NULL, read, &r_params);
	werr = pthread_create(&wid, NULL, write, &w_params);

	pthread_join(rid, &rexit); // get string length from pthread_exit() function.
	pthread_join(wid, NULL);

	sem_close(r_sem);
	sem_close(w_sem);

	return *(unsigned *)rexit; // return string length
}

int main(void) {
	unsigned lensum = 0;
	for (int i = 0; i < 100; i++) {
		lensum += run();
	}
	printf("===> String average length = %u\n", lensum / 100); // string length shall be about 20 in average.
}



