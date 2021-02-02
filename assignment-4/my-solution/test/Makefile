# Recursive makefile structure
# ============================
# ==> 1. This is the level-2 sub-makefile, which is runned by another level-1
# main makefile under ".." project root directory.
# ==> 2. All tests depend on our core source file, which is wrapped into 
# "../lib/librss_search.a" static libaray. We add "-L$(CORE_DIR)/lib -lrss_search" 
# into LDFLAGS to tell linker where find it.
# 
# Important docs
# ==============
# Lua project Makefile: I learn a lot from it.
# 	==> <https://github.com/lua/lua/blob/master/makefile>
# About recursive make: a good book [Managing Projects with GNU Make]
# 	==> <http://uploads.mitechie.com/books/Managing_Projects_with_GNU_Make_Third_Edition.pdf>
# About static and dynamic libraries:
#	==> <https://www.geeksforgeeks.org/static-vs-dynamic-libraries/>
#	==> <https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html>
# About library name: compiler will add "lib" prefix and ".a" postfix
#	==> <http://forums.codeblocks.org/index.php?topic=921.0>
# About linking curl library: my local curl library --> /usr/lib/libcurl.dylib
# 	==> <https://curl.se/libcurl/c/libcurl-tutorial.html>
# 	==> <https://stackoverflow.com/questions/6302282/how-do-i-link-libcurl-to-my-c-program-in-linux>
#
# Tips about flags settings
# =========================
# ==> 1. adding the '-DNDEBUG' defines the CT symbol NDEBUG which
# suppresses all the asserts and what not. You'd do that for
# a production build, but it is a good idea to leave it in
# for the purposes of this class.
# ==> 2. VPATH specifies a list of directories that make should search for .c source files.
# But not .o or .h or executable files. So I didn't use it here.
# ==> 3. -I specifies a directory of include .h header files
# ==> 4. -l links against a library file. gcc -L looks in a directory for library files

CORE_DIR= ..
CC 		= gcc
# DFLAG = -DNDEBUG
CFLAGS 	= -g -Wall -std=c11 -Wno-unused-function $(DFLAG) \
		  -I$(CORE_DIR) -I/usr/local/include
LDFLAGS	= -L$(CORE_DIR)/lib -lrss_search \
		  -lcurl \
		  -lexpat \
		  -L/usr/local/lib -lgumbo 
# 		  -L../lib -lrssnews
DEP_DIR	= deps
DEP_FILE= $(DEP_DIR)/Makefile.dependencies
RM 		= rm -f


# ===================== END OF FLAGS SETTINGS =======================


ALL_C = $(wildcard *.c)
ALL_H = $(wildcard *.h)
ALL_O = $(SRCS:.c=.o)

T_CURL_O 		= test_curl.o 
T_RSSLIB_O		= test_rsslib.o
T_EXPAT_O		= test_expat.o
T_GUMBO_O		= test_gumbo.o 
T_RSSPARSER_O	= test_rssparser.o 
T_TOKENIZER_O	= test_tokenizer.o 
T_CURLCONN_O	= test_curlconn.o 
T_HTMLPARSER_O	= test_htmlparser.o 
# T_INDEXER_O		= test_indexer.o 


ALL_T = testcurl testexpat testgumbo testrssparser testtokenizer testcurlconn \
		testhtmlparser 
# 		testindexer
		#testlib # this test is supposed to fail


.PHONY: all depend clean

all: $(ALL_T) depend

depend: $(ALL_C) $(ALL_H)
	mkdir -p $(DEP_DIR)
	$(CC) $(CFLAGS) -MM $(ALL_C) $(LDFLAGS) > $(DEP_FILE)

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

testcurlconn: $(T_CURLCONN_O) depend
	$(CC) $(CFLAGS) -o $@ $(T_CURLCONN_O) $(LDFLAGS)

testhtmlparser: $(T_HTMLPARSER_O) depend
	$(CC) $(CFLAGS) -o $@ $(T_HTMLPARSER_O) $(LDFLAGS)

testindexer: $(T_INDEXER_O) depend
	$(CC) $(CFLAGS) -o $@ $(T_INDEXER_O) $(LDFLAGS)

clean:
	$(RM) $(ALL_O) $(ALL_T) $(DEP_FILE)
