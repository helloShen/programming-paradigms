# Recursive makefile structure
# ============================
# ==> 1. This is the level-1 main Makefile under project .root directory. We have another
# level-2 sub-makefile under "./test" unit-test directory.
# ==> 2. Target "CORE_T" will wrap compiled object files(CORE_O) are wrapped into 
# "lib/librss_search.a" static library. 
# ==> 3. The main entry file with main() function is the only object file not included
# in CORE_O, we call it "APP_O" here.
# ==> 4. Target "TEST_T" run another sub-makefile in "./test" unit-test directory.
# All unit-test object files depend on "lib/librss_search.a" library, 
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
CC 		= gcc
# DFLAG = -DNDEBUG
CFLAGS 	= -g -Wall -std=c11 -Wno-unused-function $(DFLAG)
LDFLAGS	= -lcurl -lexpat -lgumbo

AR 		= ar rcs
RM 		= rm -f

SRC_DIR = src
LIB_DIR = lib
DATA_DIR= data
PY_DIR	= py
TEST_DIR= test
DEP_DIR	= deps
DEP_FILE= $(DEP_DIR)/Makefile.dependencies


# ===================== END OF FLAGS SETTINGS =======================


UTIL_O 	= stack.o vector.o hashset.o strhash.o article.o data.o
CORE_O	= curlconn.o rssparser.o htmlparser.o tokenizer.o index.o \
		  engine.o
LIB_O	= $(UTIL_O) $(CORE_O)
APP_O	= rss_search.o
ALL_O	= $(LIB_O) $(APP_O)

LIB_T	= $(LIB_DIR)/librss_search.a
APP_T	= rss_search


.PHONY:	all test genstopwords depend clean

all: genstopwords depend $(ALL_O)
	$(CC) $(CFLAGS) -o $(APP_T) $(ALL_O) $(LDFLAGS)

test: genstopwords depend $(LIB_O)
	$(AR) $(LIB_T) $(LIB_O)
	+$(MAKE) --directory=$(TEST_DIR)

genstopwords: $(DATA_DIR)/stop-words.in $(PY_DIR)/genstopwords.py
	@python $(PY_DIR)/genstopwords.py $< $(DATA_DIR)/stopwords.data

depend:
	mkdir -p $(DEP_DIR)
	@$(CC) $(CFLAGS) -MM *.c > $(DEP_FILE)
-include $(DEP_FILE)

clean:
	$(RM) *.o $(APP_T) $(LIB_T) $(DEP_FILE)
	+$(MAKE) clean --directory=$(TEST_DIR) 
