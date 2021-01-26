#
# Using 'gcc -MM' to automatically generate dependencies rules in a file: Makefile.dependencies
# The following rules are generated:
# 	vector.o: vector.c vector.h bool.h
#	myvectortest.o: myvectortest.c vector.h bool.h
#	vectortest.o: vectortest.c vector.h bool.h
#
# '-include' this file and system will know how to make .o object files.
#
# Add all .c .h source files to the prerequisites of this dependencies files, 
# compiler will update it once any source files are changed.
#
# Add this dependencies file to prerequisites of all targets, will update executable 
# application once this dependencies file is updated.
#
# Currently I feel confortable with this old-school version. There is another modern 
# solution making use of '.d' files, please check 'Makefile_dot_d'
# 
# For more information please check these articles:
# 	http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
#	https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
#	https://www.gnu.org/software/make/manual/html_node/Remaking-Makefiles.html#Remaking-Makefiles
#
# Here is my question about this topic on stackoverflow, some references are really helpful.
#	https://stackoverflow.com/questions/65770067/about-stanford-cs107-assignment-makefile


# 1. C11 is the newest version of C language
# 2. -Wpointer-arith request a warning when addtion and subtraction are done
# on pointers to void and to functions, which is supported in GNU C.

CC = gcc
CFLAGS = -g -Wall -std=gnu11 -Wpointer-arith
LDFLAGS = 

VECTOR_SRCS = vector.c
VECTOR_HDRS = $(VECTOR_SRCS:.c=.h)
VECTOR_OBJS = $(VECTOR_SRCS:.c=.o)	

MY_VECTOR_TEST_SRCS = myvectortest.c $(VECTOR_SRCS)
MY_VECTOR_TEST_OBJS = $(MY_VECTOR_TEST_SRCS:.c=.o)

VECTOR_TEST_SRCS = vectortest.c $(VECTOR_SRCS)
VECTOR_TEST_OBJS = $(VECTOR_TEST_SRCS:.c=.o)

HASHSET_SRCS = hashset.c vector.c
HASHSET_HDRS = hashset.h vector.h
HASHSET_OBJS = $(HASHSET_SRCS:.c=.o)

HASHSET_TEST_SRCS = hashsettest.c $(HASHSET_SRCS)
HASHSET_TEST_OBJS = $(HASHSET_TEST_SRCS:.c=.o)

ST_SRCS = streamtokenizer.c
ST_HDRS = $(ST_SRCS:.c=.h)
ST_OBJS = $(ST_SRCS:.c=.o)

ST_TEST_SRCS = streamtokenizer-test.c $(ST_SRCS)
ST_TEST_OBJS = $(ST_TEST_SRCS:.c=.o)

TEMP_TEST_SRCS = temptest.c
TEMP_TEST_OBJS = $(TEMP_TEST_SRCS:.c=.o)

SRCS = $(VECTOR_SRCS) myvectortest.c vectortest.c \
	   $(HASHSET_SRCS) hashsettest.c \
	   $(ST_SRCS) streamtokenizer-test.c \
	   $(TEMP_TEST_SRCS)
HDRS = $(VECTOR_HDRS) \
	   $(HASHSET_HDRS) \
	   $(ST_HDRS) \
	   $(ST_TEST_HDRS)
OBJS = $(SRCS:.c=.o)

EXES = myvectortest vectortest \
	   hashsettest \
	   streamtokenizer-test \
	   temptest

DEP_DIR = ./deps
DEP_FILE = $(DEP_DIR)/Makefile.dependencies



.PHONY: default depend clean

default: $(EXES)

# 1. The dependencies below make use of make's default rules,
# under which a .o automatically depends on its .c and
# the action taken uses the $(CC) and $(CFLAGS) variables.
# These lines describe a few extra dependencies involved.
#
# 2. -MMD means write a dependencies file containing user headers.
# -MM likes -MMD, but also implies -E only do preprocess,
# and write to stand out by default.

depend:: $(SRCS) $(HDRS) 
	$(CC) $(CFLAGS) -MM $(SRCS) $(LDFLAGS) > $(DEP_FILE)

-include $(DEP_FILE)

vectortest: $(VECTOR_TEST_OBJS) depend
	$(CC) $(CFLAGS) -o $@ $(VECTOR_TEST_OBJS) $(LDFLAGS)

myvectortest: $(MY_VECTOR_TEST_OBJS) depend
	$(CC) $(CFLAGS) -o $@ $(MY_VECTOR_TEST_OBJS) $(LDFLAGS)

hashsettest: $(HASHSET_TEST_OBJS) depend
	$(CC) $(CFLAGS) -o $@ $(HASHSET_TEST_OBJS) $(LDFLAGS)

streamtokenizer-test: $(ST_TEST_OBJS) depend
	$(CC) $(CFLAGS) -o $@ $(ST_TEST_OBJS) $(LDFLAGS)

temptest: $(TEMP_TEST_OBJS) depend
	$(CC) $(CFLAGS) -o $@ $(TEMP_TEST_OBJS) $(LDFLAGS)

clean:
	-rm -f *.o  $(EXES) $(DEP_FILE)





