/**
 * Using semaphore to limit maximum thread number.
 * https://stackoverflow.com/questions/66404929/always-unlink-the-posix-named-semaphore-in-shared-memory?noredirect=1
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

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

/**
 * simulate a tickets selling project
 */
struct {
	unsigned num_agents;
	unsigned num_tickets;
} project;

/**
 * shared threads pool
 */
struct {
	unsigned pool_size;
	pthread_t *pool;
} threads_pool;

/**
 * producer, consumer threads id
 */
struct {
	pthread_t producer;
	pthread_t consumer;
} producer_consumer;

/**
 * a pair of semaphore
 */
struct {
	sem_t *producer_sem;
	sem_t *consumer_sem;
} sem;

/**
 * barrier is not implemented in mac os, we use a pair of semaphore instead.
 */
struct {
	sem_t *producer_b;
	sem_t *consumer_b;
} barrier;

/**
 * producer thread
 * create agent sell_tickets() threads
 */
static void *producer(void *arg) {
	/* barrier assure that checkin() and checkout() thread begin to work when two threads are both created. */
	sem_post(barrier.consumer_b);
	sem_wait(barrier.producer_b);

	unsigned tickets_pool = project.num_tickets;	// shared resource for each agent
	pthread_mutex_t tickets_pool_lock;				// mutex lock for visiting tickets pool

	agent agents[project.num_agents];				// arguments pass to 10 threads
	unsigned id;									// agent id
	pthread_t tid;									// current sell_tickets() thread id
	int err;										// thread_create() return err code

	/* int pthread_mutex_init(pthread_mutex_t *mutex,
	 *						  const pthread_mutexattr_t *attr);
	 * --------------------------------------------------------
	 * @mutex: mutex to initialize
	 * @attr: set it to NULL for default options.
	 * @return: 0 if success, otherwise return error code. */
	pthread_mutex_init(&tickets_pool_lock, NULL);

	for (unsigned i = 0; i < project.num_agents; i++) {
		id = i + 1;
		new_agent(&agents[i], id, project.num_tickets / project.num_agents, &tickets_pool, &tickets_pool_lock);

		sem_wait(sem.producer_sem);
		/* int pthread_create(pthread_t *tidp,
		 *					  const pthread_attr_t *attr,
		 *					  void *(*start_rtn)(void *),
		 *					  void *arg);
		 * ----------------------------------------------
		 * @tidp: address to store thread id
		 * @attr: thread attributes
		 * @start_rtn: pointer to the function that thread will run
		 * @arg: pointer to the structure include all arguments for start_rtn function. */
		err = pthread_create(threads_pool.pool + i % threads_pool.pool_size, NULL, sell_tickets, &agents[i]);
		if (err != 0) {
			fprintf(stdout, "error[%d]: can't create thread.\n", err);		
			fflush(stdout);
			/* cancel consumer thread before exiting
			 * ==> https://linux.die.net/man/3/pthread_setcanceltype
			 * -------------------------------------
			 * thread cancel state default setting: PTHREAD_CANCEL_ENABLE
			 * thread cancel type default setting: PTHREAD_CANCEL_DEFERRED
			 * -----------------------------------------------------------
			 * PTHREAD_CANCEL_DEFERRED means that it will wait the pthread_join, 
		     * pthread_cond_wait, pthread_cond_timewait.. to be call when the 
		     * thread receive cancel message. */
			pthread_cancel(producer_consumer.consumer);
			pthread_exit(NULL);
		} else {
			tid = *(threads_pool.pool + i % threads_pool.pool_size);
			fprintf(stdout, "thread of agent@%d created, thread id = @%lx\n", id, (unsigned long)tid);
			fflush(stdout);
			sem_post(sem.consumer_sem);
		}
	}

	/* wait for checkout() thread to finish */
	pthread_join(producer_consumer.consumer, NULL);

	pthread_mutex_destroy(&tickets_pool_lock);
	if (tickets_pool == 0) {
		fprintf(stdout, "Today's tickets are sold out!\n");
		fflush(stdout);
	} else {
		fprintf(stdout, "%d tickets left at the end of the day!\n", tickets_pool);
		fflush(stdout);
	}
	pthread_exit(NULL);
}

/**
 * consumer thread
 * wait for old agent sell_tickets() thread to exit and increment the producer semaphore
 */
static void *consumer(void *arg) {
	/* barrier assure that checkin() and checkout() thread begin to work when two threads are both created. */
	sem_post(barrier.producer_b);
	sem_wait(barrier.consumer_b);

	void *tret; 	// to store pthread_exit() exit status rval_ptr

	/* If we don't block main thread to wait for child threads, it will exit before child
	 * child threads finish their job. The values it passed to child threads would be no
	 * longer valid. */
	for (unsigned i = 0; i < project.num_agents; i++) {
		sem_wait(sem.consumer_sem);
		/* int pthread_join(pthread_t thread, void **rval_ptr);
		 * ----------------------------------------------------
		 * ==>1. Blocks the calling thread until the target thread "thread" terminates, 
		 * unless thread has already terminated. 
		 * ==>2. If value_ptr is non-NULL and pthread_join() returns successfully, 
		 * then the value passed to pthread_exit() by the target thread is placed in 
		 * value_ptr. If the target thread has been canceled then value_ptr is set to 
		 * PTHREAD_CANCELED. */
		pthread_join(*(threads_pool.pool + i % threads_pool.pool_size), &tret);
		sem_post(sem.producer_sem);
		fprintf(stdout, "agent@%d has finished his job!\n", *(unsigned *)tret);
		fflush(stdout);
	}
	pthread_exit(NULL);
}

/**
 * producer-consumer model using semaphore
 * maximum 10 agent sell_tickets() threads work at a time
 */
static void run(void) {
	/* project */
	project.num_agents = 30;
	project.num_tickets = 300;
	/* threads pool */
	threads_pool.pool_size = 10;
	pthread_t pool[threads_pool.pool_size];
	threads_pool.pool = &pool[0];
	/* producer-consumer semaphore */
	sem.producer_sem = sem_open("/producer_sem", O_CREAT | O_EXCL, S_IREAD | S_IWRITE, threads_pool.pool_size);
	sem.consumer_sem = sem_open("/consumer_sem", O_CREAT | O_EXCL, S_IREAD | S_IWRITE, 0);
	/* barrier */
	barrier.producer_b = sem_open("/producer_b", O_CREAT | O_EXCL, S_IREAD | S_IWRITE, 0);
	barrier.consumer_b = sem_open("/consumer_b", O_CREAT | O_EXCL, S_IREAD | S_IWRITE, 0);
	/* create producer(), consumer() threads */
	int producer_err = pthread_create(&producer_consumer.producer, NULL, producer, NULL);
	int consumer_err = pthread_create(&producer_consumer.consumer, NULL, consumer, NULL);
	if (producer_err != 0 || consumer_err != 0) {
		if (producer_err != 0) { 
			fprintf(stdout, "error: checkin() thread creation failed!\n");
			fflush(stdout);
		}
		if (consumer_err != 0) {
			fprintf(stdout, "error: checkout() thread creation failed!\n");
			fflush(stdout);
		}
		sem_unlink("/producer_sem");
		sem_unlink("/consumer_sem");
		sem_unlink("/producer_b");
		sem_unlink("/consumer_b");
		return;
	}

	/*
	 * wait for checkin() and checkout() to exit
	 */
	pthread_join(producer_consumer.producer, NULL);
	pthread_join(producer_consumer.consumer, NULL);

	/*
	 * free resources
	 * --------------
	 * Always use sem_unlink() to remove the semaphore, but not sem_close().
	 * Close a semaphore does NOT remove the semaphore from system shared memory.
	 * Close operation occurs automatically on process termination, we don't have 
	 * to write sem_close() explicitly. 
	 */
	sem_unlink("/producer_sem");
	sem_unlink("/consumer_sem");
	sem_unlink("/producer_b");
	sem_unlink("/consumer_b");
}
	

int main(void) {
	run();
}