#####################################################
#
# generate all applications at once:
# 	make
# or one by one:
# 	make genetic_strcmp
# 	make mimic_stack
# 	...
#
# clear all generated files:
# 	make clean
#
#####################################################

# built-in variables
CC			= gcc
CFLAGS		= -g -O0 -std=c11
DEP_DIR 	= deps
DEP_FILE	= $(DEP_DIR)/Makefile.dependencies

ISEARCH_O		= isearch_test.o Isearch.o Intcmp.o Strcmp.o
STACK_O 		= stack.o stack_test.o
GSTACK_O		= gstack.o gstack_test.o
CTYPE_O 		= c_not_type_safe_example.o
CHANNELLING_O 	= channelling.o
AG_TKT_O		= agents_tickets.o
AG_TKT_MUTEX_O	= agents_tickets_mutex.o
RW_SEM_O		= read_write_semaphore.o
AG_TKT_POOL_O	= agents_tickets_pool.o 
REMOVE_SEM_O	= remove_sem.o
AG_TKT_TP_O		= agents_tickets_using_threads_pool.o threads_pool.o

ALL_T = isearch stack gstack ctype channelling agtkt agtktm agtktp rmsem agtkttp


# clean
.PHONY: all depend clean 

all: 

depend: 
	mkdir -p $(DEP_DIR)
	$(CC) $(CFLAGS) -MM *.c > $(DEP_FILE)
-include $(DEP_FILE)

# *.o will be generated automatically by implicite rules
# "$@" implies the file name on the left side of :
# "$^" implies the file name on the right side of :
isearch: depend $(ISEARCH_O) 
	$(CC) $(CFLAGS) -o $@ $(ISEARCH_O)

stack: depend $(STACK_O)
	$(CC) $(CFLAGS) -o $@ $(STACK_O)

gstack: depend $(GSTACK_O)
	$(CC) $(CFLAGS) -o $@ $(GSTACK_O)

ctype: depend $(CTYPE_O)
	$(CC) $(CFLAGS) -o $@ $(CTYPE_O)

channelling: depend $(CHANNELLING_O)
	$(CC) $(CFLAGS) -o $@ $(CHANNELLING_O)

agtkt: depend $(AG_TKT_O)
	$(CC) $(CFLAGS) -o $@ $(AG_TKT_O)

agtktm: depend $(AG_TKT_MUTEX_O)
	$(CC) $(CFLAGS) -o $@ $(AG_TKT_MUTEX_O)

rwsem: depend $(RW_SEM_O)
	$(CC) $(CFLAGS) -o $@ $(RW_SEM_O)

agtktp: depend $(AG_TKT_POOL_O)
	$(CC) $(CFLAGS) -o $@ $(AG_TKT_POOL_O)

rmsem: depend $(REMOVE_SEM_O)
	$(CC) $(CFLAGS) -o $@ $(REMOVE_SEM_O)

agtkttp: depend $(AG_TKT_TP_O)
	$(CC) $(CFLAGS) -o $@ $(AG_TKT_TP_O)

clean:	
	rm -f *.o $(ALL_T) $(DEP_FILE)
