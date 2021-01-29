####################################################################################################
# Important DOCS
# --------------
# About static and dynamic libraries:
#	--> <https://www.geeksforgeeks.org/static-vs-dynamic-libraries/>
#	--> <https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html>
# About library name: compiler will add "lib" prefix and ".a" postfix
#	--> <http://forums.codeblocks.org/index.php?topic=921.0>
# About linking curl library: my local curl library --> /usr/lib/libcurl.dylib
# 	--> <https://curl.se/libcurl/c/libcurl-tutorial.html>
# 	--> <https://stackoverflow.com/questions/6302282/how-do-i-link-libcurl-to-my-c-program-in-linux>
####################################################################################################

CC = gcc
CFLAGS = -g -Wall -std=c11 \
		 -I../ \
		 -I/usr/local/include
LDFLAGS = -lcurl \
		  -lexpat \
		  -L/usr/local/lib -lgumbo \
		  -L../lib \
		  -lrssnews 

T_CURL_C = test_curl.c
T_CURL_O = $(T_CURL_C:.c=.o)

T_RSSLIB_C = test_rsslib.c 
T_RSSLIB_O = $(T_RSSLIB_C:.c=.o)

T_EXPAT_C = test_expat.c
T_EXPAT_O = $(T_EXPAT_C:.c=.o)

T_GUMBO_C = test_gumbo.c
T_GUMBO_O = $(T_GUMBO_C:.c=.o)

VECTOR_C = ../vector.c
VECTOR_H = $(VECTOR_C:.c=.h) ../bool.h
VECTOR_O = $(VECTOR_C:.c=.o)

HASHSET_C = ../hashset.c
HASHSET_H = $(HASHSET_C:.c=.h)
HASHSET_O = $(HASHSET_C:.c=.o)

ARTICLE_C = ../article.c
ARTICLE_H = $(ARTICLE_C:.c=.h) ../rsstag.h
ARTICLE_O = $(ARTICLE_C:.c=.o)

RSSPARSER_C = ../rssparser.c $(VECTOR_C) $(HASHSET_C) $(ARTICLE_C)
RSSPARSER_H = ../rssparser.h $(VECTOR_H) $(HASHSET_H) $(ARTICLE_H)
RSSPARSER_O = $(RSSPARSER_C:.c=.o)

T_RSSPARSER_C = test_rssparser.c $(RSSPARSER_C) 
T_RSSPARSER_H = $(RSSPARSER_H)
T_RSSPARSER_O = $(T_RSSPARSER_C:.c=.o) 

STACK_C = ../stack.c
STACK_H = $(STACK_C:.c=.h)
STACK_O = $(STACK_C:.c=.o)

TOKENIZER_C = ../tokenizer.c $(VECTOR_C) $(HASHSET_C) $(STACK_C)
TOKENIZER_H = ../tokenizer.h $(VECTOR_H) $(HASHSET_H) $(STACK_H) ../bool.h
TOKENIZER_O = $(TOKENIZER_C:.c=.o)

T_TOKENIZER_C = test_tokenizer.c $(TOKENIZER_C)
T_TOKENIZER_O = $(T_TOKENIZER_C:.c=.o)

DEP_DIR = deps
DEP_FILE = $(DEP_DIR)/Makefile.dependencies

SRCS = $(T_CURL_C) $(T_RSSLIB_C) $(T_EXPAT_C) $(T_GUMBO_C) \
	   $(T_RSSPARSER_C) $(TOKENIZER_C) $(T_TOKENIZER_C)
HDRS = $(T_RSSPARSER_H) $(TOKENIZER_H)
OBJS = $(SRCS:.c=.o)

EXECS = testcurl \
		testexpat \
		testgumbo \
		testrssparser \
		testtokenizer
		#testlib 


.PHONY: default depend clean

default: $(EXECS)

depend: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -MM $(SRCS) $(LDFLAGS) > $(DEP_FILE)

-include $(DEP_FILE)

testcurl: $(T_CURL_O) depend
	$(CC) $(CFLAGS) -o $@ $(T_CURL_O) $(LDFLAGS)

testlib: $(T_RSSLIB_O) depend
	$(CC) $(CFLAGS) -o $@ $(T_RSSLIB_O) $(LDFLAGS)

testexpat: $(T_EXPAT_O) depend
	$(CC) $(CFLAGS) -o $@ $(T_EXPAT_O) $(LDFLAGS)

testgumbo: $(T_GUMBO_O) depend
	$(CC) $(CFLAGS) -o $@ $(T_GUMBO_O) $(LDFLAGS)

testrssparser: $(T_RSSPARSER_O) depend
	$(CC) $(CFLAGS) -o $@ $(T_RSSPARSER_O) $(LDFLAGS)

testtokenizer: $(T_TOKENIZER_O) depend
	$(CC) $(CFLAGS) -o $@ $(T_TOKENIZER_O) $(LDFLAGS)



clean:
	-rm -f *.o ../*.o $(EXECS) $(DEP_FILE)
