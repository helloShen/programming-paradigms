# About static and dynamic libraries:
#	--> <https://www.geeksforgeeks.org/static-vs-dynamic-libraries/>
#	--> <https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html>
# About library name: compiler will add "lib" prefix and ".a" postfix
#	--> <http://forums.codeblocks.org/index.php?topic=921.0>
# About linking curl library: my local curl library --> /usr/lib/libcurl.dylib
# 	--> <https://curl.se/libcurl/c/libcurl-tutorial.html>
# 	--> <https://stackoverflow.com/questions/6302282/how-do-i-link-libcurl-to-my-c-program-in-linux>

CC = gcc
CFLAGS = -g -Wall \
		 -I../ \
		 -I/usr/local/include \
LDFLAGS = -lcurl \
		  -lexpat \
		  -L/usr/local/lib -lgumbo \
		  -L/Users/Wei/github/letsplayc/src/programming-paradigms/assignment-4/my-solution/lib/ \
		  -lrssnews 


CURL_C = TestCurl.c
CURL_O = $(CURL_C:.c=.o)

RSSLIB_C = TestRssLib.c 
RSSLIB_O = $(RSSLIB_C:.c=.o)

EXPAT_C = TestExpat.c
EXPAT_O = $(EXPAT_C:.c=.o)

GUMBO_C = TestGumbo.c
GUMBO_O = $(GUMBO_C:.c=.o)

DEP_DIR = deps
DEP_FILE = $(DEP_DIR)/Makefile.dependencies

SRCS = $(CURL_C) $(RSSLIB_C) $(EXPAT_C) $(GUMBO_C)
HDRS = 
OBJS = $(SRCS:.c=.o)
EXECS = testcurl \
		testexpat \
		testgumbo
		#testlib 


.PHONY: default depend clean

default: $(EXECS)

depend: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -MM $(SRCS) $(LDFLAGS) > $(DEP_FILE)

-include $(DEP_FILE)

testcurl: $(CURL_O) depend
	$(CC) $(CFLAGS) -o $@ $(CURL_O) $(LDFLAGS)

testlib: $(RSSLIB_O) depend
	$(CC) $(CFLAGS) -o $@ $(RSSLIB_O) $(LDFLAGS)

testexpat: $(EXPAT_O) depend
	$(CC) $(CFLAGS) -o $@ $(EXPAT_O) $(LDFLAGS)

testgumbo: $(GUMBO_O) depend
	$(CC) $(CFLAGS) -o $@ $(GUMBO_O) $(LDFLAGS)

clean:
	-rm -f *.o $(EXECS) $(DEP_FILE)
