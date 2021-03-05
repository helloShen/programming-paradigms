/**
 * Using semaphore to limit maximum thread number.
 * https://stackoverflow.com/questions/66404929/always-unlink-the-posix-named-semaphore-in-shared-memory?noredirect=1
 */
#include "threads_pool.h"
#include "testutils.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <assert.h>

/**
 * If arguments that pthread_create() pass to sell_tickets() are more than one, 
 * they need to be wrapped into a single structure.
 */
typedef struct {
	unsigned agent_id;				// simulate an agent
	unsigned tickets_tosell;		// agent's personal goal of the day
	unsigned *tickets_pool;			// shared tickets pool
	pthread_mutex_t *pool_lock;		// mutex lock for visiting the shared tickets pool
} agent;

/**
 * Constructor
 */
static void new_agent(agent *a, unsigned agentid, unsigned tickets_num, unsigned *pool, pthread_mutex_t *lock) {
	a->agent_id = agentid;
	a->tickets_tosell = tickets_num;
	a->tickets_pool = pool;
	a->pool_lock = lock;
}

/**
 * Implement void *(*start_rtn)(void *);
 * -------------------------------------
 * Each thread execute this function.
 */
static void *sell_tickets(void *agent_addr) {
	agent *a = (agent *)agent_addr;
	while (a->tickets_tosell > 0) {
		/* If the mutex is already locked, the calling thread shall block until the 
		 * mutex becomes available. If you don't want the thread to be blocked, use 
		 * pthread_mutex_trylock() instead. */
		pthread_mutex_lock(a->pool_lock);	// begin of race condition
		(*a->tickets_pool)--;
		fprintf(stdout, "agent@%d sells a ticket, %d tickets left in pool.\n", a->agent_id, *a->tickets_pool);
		fflush(stdout);
		pthread_mutex_unlock(a->pool_lock);	// end of race condition
		a->tickets_tosell--;
		fprintf(stdout, "agent@%d has %d tickets to sell.\n", a->agent_id, a->tickets_tosell);
		fflush(stdout);
	}
	/* void pthread_exit(void *rval_ptr);
	 * ----------------------------------
	 * agent leave his id when finish his job, 
	 * pthread_join() funciton will get this information. */
	pthread_exit((void *)&a->agent_id); 
}

typedef struct {
	unsigned num_agents;
	unsigned num_tickets;
} project;

void test_threads_pool(project *p) {
	tstart("threads_pool::threads_pool_init()::create_thread()::threads_pool_close()");

	unsigned tickets_pool;
	pthread_mutex_t lock;
	agent agents[p->num_agents];
	unsigned id;

	tickets_pool = p->num_tickets;		// shared resource
	pthread_mutex_init(&lock, NULL);
	threads_pool_init(10);	

	for (int i = 0; i < p->num_agents; i++) {
		id = i + 1;
		new_agent(&agents[i], id, p->num_tickets / p->num_agents, &tickets_pool, &lock);
		create_thread(sell_tickets, &agents[i]);
	}
	threads_pool_close();	
	pthread_mutex_destroy(&lock);

	assert(0 == tickets_pool);
	tpass();
}

int main(void) {
	project p;
	p.num_agents = 30;
	p.num_tickets = 300;
	test_threads_pool(&p);
}