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

ROOT	= ..
DEP_DIR	= deps
DEP_FILE= $(DEP_DIR)/Makefile.dependencies

CC 		= gcc
# DFLAG = -DNDEBUG
CFLAGS 	= -g -Wall -std=c11 -Wno-unused-function $(DFLAG) \
		  -I$(ROOT) 
LDFLAGS	= -L$(ROOT)/lib -lrss_search \
		  -lcurl -lexpat -lgumbo

RM 		= rm -f


# ===================== END OF FLAGS SETTINGS =======================


ALL_O = testutils.o \
		test_curl.o test_expat.o test_gumbo.o \
		test_vector.o test_hashset.o test_article.o test_data.o \
		test_curlconn.o test_rssparser.o test_htmlparser.o test_tokenizer.o test_index.o \
		test_nothing.o

CURL_O 		= testutils.o test_curl.o 
EXPAT_O		= testutils.o test_expat.o
GUMBO_O		= testutils.o test_gumbo.o 
VECTOR_O	= testutils.o test_vector.o
HASHSET_O	= testutils.o test_hashset.o
ARTICLE_O	= testutils.o test_article.o
DATA_O		= testutils.o test_data.o
CURLCONN_O	= testutils.o test_curlconn.o 
RSSPARSER_O	= testutils.o test_rssparser.o 
HTMLPARSER_O= testutils.o test_htmlparser.o 
TOKENIZER_O	= testutils.o test_tokenizer.o 
INDEX_O	= testutils.o test_index.o 

NOTHING_O	= test_nothing.o



ALL_T = testcurl testexpat testgumbo \
		testvector testhashset testarticle testdata \
		testcurlconn testrssparser testhtmlparser testtokenizer testindex \
		testnothing


.PHONY: all depend clean

all: $(ALL_T)

depend: 
	mkdir -p $(DEP_DIR)
	@$(CC) $(CFLAGS) -MM *.c > $(DEP_FILE)
-include $(DEP_FILE)

testcurl: depend $(CURL_O)
	$(CC) $(CFLAGS) -o $@ $(CURL_O) $(LDFLAGS)

testexpat: depend $(EXPAT_O)
	$(CC) $(CFLAGS) -o $@ $(EXPAT_O) $(LDFLAGS)

testgumbo: depend $(GUMBO_O)
	$(CC) $(CFLAGS) -o $@ $(GUMBO_O) $(LDFLAGS)

testvector: depend $(VECTOR_O)
	$(CC) $(CFLAGS) -o $@ $(VECTOR_O) $(LDFLAGS)

testhashset: depend $(HASHSET_O)
	$(CC) $(CFLAGS) -o $@ $(HASHSET_O) $(LDFLAGS)

testarticle: depend $(ARTICLE_O)
	$(CC) $(CFLAGS) -o $@ $(ARTICLE_O) $(LDFLAGS)

testdata: depend $(DATA_O)
	$(CC) $(CFLAGS) -o $@ $(DATA_O) $(LDFLAGS)

testcurlconn: depend $(CURLCONN_O)
	$(CC) $(CFLAGS) -o $@ $(CURLCONN_O) $(LDFLAGS)

testrssparser: depend $(RSSPARSER_O)
	$(CC) $(CFLAGS) -o $@ $(RSSPARSER_O) $(LDFLAGS)

testhtmlparser: depend $(HTMLPARSER_O)
	$(CC) $(CFLAGS) -o $@ $(HTMLPARSER_O) $(LDFLAGS)

testtokenizer: depend $(TOKENIZER_O)
	$(CC) $(CFLAGS) -o $@ $(TOKENIZER_O) $(LDFLAGS)

testindex: depend $(INDEX_O)
	$(CC) $(CFLAGS) -o $@ $(INDEX_O) $(LDFLAGS)

testnothing: depend $(NOTHING_O)
	$(CC) $(CFLAGS) -o $@ $(NOTHING_O) $(LDFLAGS)

clean:
	$(RM) *.o $(ALL_T) $(DEP_FILE)
