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
CC = gcc
CFLAGS = -g -o0
DEPS = 


isearch_dep = isearch_test.o Isearch.o Intcmp.o Strcmp.o
stack_dep = stack.o stack_test.o
gstack_dep = gstack.o gstack_test.o
ctype_dep = c_not_type_safe_example.o
channelling_dep = channelling.o

execs = isearch stack gstack ctype channelling
functions = $(isearch_dep) $(stack_dep) $(gstack_dep) $(ctype_dep) $(channelling_dep) $(channelling_dep)


# main event (default)
runall: 

# *.o will be generated automatically by implicite rules
# "$@" implies the file name on the left side of :
# "$^" implies the file name on the right side of :
isearch: $(isearch_dep) 
	$(CC) $(CFLAGS) -o $@ $^
stack: $(stack_dep)
	$(CC) $(CFLAGS) -o $@ $^
gstack: $(gstack_dep)
	$(CC) $(CFLAGS) -o $@ $^
ctype: $(ctype_dep)
	$(CC) $(CFLAGS) -o $@ $^
channelling: $(channelling_dep)
	$(CC) $(CFLAGS) -o $@ $^



# clean
.PHONY: clean
clean:	
	-rm -f $(execs) $(functions)
