#include <pthread.h>
#include <stdio.h>

#ifndef _AGENT_
#define _AGENT_

/**
 * If arguments that pthread_create() pass to sell_tickets() are more than one, 
 * they need to be wrapped into a single structure.
 */
typedef struct {
	int agent_id;
	int tickets_tosell;
} agent;

/**
 * Constructor
 */
void new_agent(agent *a, int agentid, int ticketsnum) {
	a->agent_id = agentid;
	a->tickets_tosell = ticketsnum;
}

#endif // _AGENT_


/**
 * Implement void *(*start_rtn)(void *);
 * -------------------------------------
 * Each thread execute this function.
 */
static void *sell_tickets(void *agent_addr) {
	agent *a = (agent *)agent_addr;
	printf("agent@%d has %d tickets to sell.\n", a->agent_id, a->tickets_tosell);
	while (a->tickets_tosell > 0) {
		a->tickets_tosell--;
		printf("agent@%d sells a ticket, remain %d tickets to sell.\n", a->agent_id, a->tickets_tosell);
	}
	/* void pthread_exit(void *rval_ptr);
	 * ----------------------------------
	 * agent leave his id when finish his job, 
	 * pthread_join() funciton will get this information. */
	pthread_exit((void *)&a->agent_id); 
}


int main(void) {
	int num_agents = 10;
	int num_tickets = 150;	

	pthread_t tid[10];		// to store 10 thread ids
	agent agents[10];		// arguments pass to 10 threads
	int id;					// agent id
	int err;				// thread_create() function return the error code
	void *tret; 			// to store pthread_exit() exit status rval_ptr

	for (int i = 0; i < num_agents; i++) {
		id = i + 1;
		new_agent(&agents[i], id, num_tickets / num_agents);
		/* int pthread_create(pthread_t *tidp,
		 *					  const pthread_attr_t *attr,
		 *					  void *(*start_rtn)(void *),
		 *					  void *arg);
		 * ----------------------------------------------
		 * @tidp: address to store thread id
		 * @attr: thread attributes
		 * @start_rtn: pointer to the function that thread will run
		 * @arg: pointer to the structure include all arguments for start_rtn function. */
		err = pthread_create(&tid[i], NULL, sell_tickets, &agents[i]);
		if (err != 0) {
			printf("error[%d]: can't create thread.", err);		
		} else {
			printf("thread of agent@%d created, thread id = @%lx\n", id, (unsigned long)tid[i]);
		}
	}
	/* If we don't block main thread to wait for child threads, it will exit before child
	 * child threads finish their job. The values it passed to child threads would be no
	 * longer valid. */
	for (int i = 0; i < num_agents; i++) {
		/* int pthread_join(pthread_t thread, void **rval_ptr);
		 * ----------------------------------------------------
		 * ==>1. Blocks the calling thread until the target thread "thread" terminates, 
		 * unless thread has already terminated. 
		 * ==>2. If value_ptr is non-NULL and pthread_join() returns successfully, 
		 * then the value passed to pthread_exit() by the target thread is placed in 
		 * value_ptr. If the target thread has been canceled then value_ptr is set to 
		 * PTHREAD_CANCELED. */
		pthread_join(tid[i], &tret);
		printf("agent@%d has finished his job!\n", *(int *)tret);
	}
}