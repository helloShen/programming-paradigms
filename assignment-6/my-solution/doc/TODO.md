#################
#   22/01/2021  #
#################

A typical RSS feed as a sequence of "items":
```xml
<item>
  <title>At Installation Mass, New Pope Strikes a Tone of Openness</title>
  <link>http://www.nytimes.com/2005/04/24/international/worldspecial2/24cnd-pope.html</link>
  <description>The Mass, which drew 350,000 spectators, marked an important moment in the transformation of Benedict XVI.</description>
  <author>By IAN FISHER and LAURIE GOODSTEIN</author>
  <pubDate>Sun, 24 Apr 2005 00:00:00 EDT</pubDate>
  <guid isPermaLink="false">http://www.nytimes.com/2005/04/24/international/worldspecial2/24cnd-pope.html</guid>
</item>
```


1. need url / url connection, to establish the connection, and get news feed.
	-->libcurl
2. need xml parser, to extract titles, descriptions, links, and articles from xml news feed files. 
	-->expat
3. need a html parser, to analyse the web page and get the content of an article.
	-->(google)gumbo-parser
4. need tokenizer to segment words from articles, calculate the frequences and build index.
	-->streamtokenizer
5. the core index is the "word -> article" mapping.
6. this mapping can be stored in a hashset using word as the key.
	-->hashset/vector


#################
#   23/01/2021  #
#################

1. [done]CURL works
2. [done]Handout "librssnews.a" works only on linux, not on MACOS(darwin). 
3. [done]Decided to use CURL + expat + streamtokenizer + hashset/vector, maybe gumbo-parser for HTML.


#################
#   24/01/2021  #
#################

1. [done]expat test passed!


#################
#   25/01/2021  #
#################

1. [done]article done!
2. [done]rssparser done!

to be continued:
1. hashmap
2. article_scanner
  2.1 rewrite streamtokenizer
  2.2 count frequencies


#################
#   26/01/2021  #
#################
1. [done]hashmap cancle, hashset is good enough!
2. [done]word tokenization is done!
3. [done]frequence counting is done! 
4. [done]stopwords are loaded!

to be continue:
1. build index with bag of words.

#################
#   27/01/2021  #
#################
1. [done] indexer
2. [done] move curl connection module and gumbo parser module to working space. done!

to be continued:
1. test indexer


#################
#   28/01/2021  #
#################
1. [done] test_index.c download_articles()
2. [done] rewrite curlconne.c dump_url(), now it dumps web data into memory instead of local files.
3. [done] rssparser.c parserss() now accept input web page stream from memory.

to be continued:
0. optimize makefile source files dependencies
1. change htmlparser.c clean_test() so that it can also get input stream from memory.
2. establish the chain of data stream for "index_article(index *idx, article *atc);"


#################
#   29/01/2021  #
#################
1. [done] Optimize makefile source files dependencies.
2. [done] Create recursive makefile structure. Makefile in root directory will run 
sub-makefile under unit-test repository.

to be continued:
1. change htmlparser.c clean_test() so that it can also get input stream from memory.
2. establish the chain of data stream for "index_article(index *idx, article *atc);"



#################
#   30/01/2021  #
#################
1. [done] define structure article and data in their own .c file. 
2. [done] clean my code, reduce code coupling, especially the unit test.
3. [done] tokenizer.c now accept char * input stream.

to be continued:
0. set stopwords hashset as a field of tokenizer. user don't have to create themselves a hashset, and pass it to tokenizer. They just need to tell tokenizer where to find the 
stopwords stream.
1. continue to clean the code, to minimize the coupling in unit test.


#################
#   31/01/2021  #
#################
3 Largely used C unit-test frameworks:
==> 1. [MinUnit](http://www.jera.com/techinfo/jtns/jtn002.html)
==> 2. [Check](https://libcheck.github.io/check/)
[Tutorial for Check](https://libcheck.github.io/check/doc/check_html/check_3.html). It also recommend to use [Autotools].
==> 3. [gtest](https://github.com/google/googletest)


Interesting Knowledges:
==> 1. Some discussion on topic of unit-test toolkits in C on stackoverflow: 
<https://stackoverflow.com/questions/65820/unit-testing-c-code>
==> 2. Another way to build project using [Autotools](https://www.lrde.epita.fr/~adl/autotools.html), seems better than Makefile.
==> 3. A great pragmatic book of C programming: [C Programming FAQs: Frequently Asked Questions](http://c-faq.com/index.html)
==> 4. Another great reference: [CERT C Coding Standard](https://wiki.sei.cmu.edu/confluence/display/c/SEI+CERT+C+Coding+Standard)

Some thoughts:
1. Currently I wiil just include [MinUnit] as a basic unit-test Macro. I don't want to introduce too much complex into this project. Keep simple, keep foolish.
2. After finish basic features, we can try to use [Check] and [Autotools].


1. [done] decide to stick with naive unit-test code.
2. [done] python generate stopwords array data, and use "#include" load stopwords into array.

to do:
1. de-coupling unit-test
2. re-write article three fields


##############################
#   01/02/2021 - 07/02/2021  #
##############################
1. [done] rebuild all unit-test so far.
2. [done] finish testing vector, hashset, tokenizer, ...
3. [done] re-write article

todo:
1. test and clean legacy of article
2. test indexer


################
#   08/02/2021 #
################
[done] 1. optimize legacy of article
  [done] 1.1 rssparser
  [done] 1.2 htmlparser
[done] 2. identify duplicate articles
[done] 3. test data
[done] 4. re-write indexer

todo:
1. test indexer


#############################
#   09/02/2021 - 12/02/2021 #
#############################
[done] 1. learn to use lldb to debug
[done] 2. test index

[done] 3. move bag_into_index(), build_index() from index into engine
    engine
    /    \
tokenizer index

[done] 4. hashset<article> articles;
[done] 5. isolate test_hashset 
[done] 6. isolate test_vector

todo:
1. engine.c, engine.h


##############
# 13/02/2021 #
##############
[done] 1. copy data when building bag_of_words
[done] 2. build_index() in engine

todo:
0. debug articles
1. search() in engine
2. curlconn.c treat 301 redirction


##############
# 15/02/2021 #
##############
[done] 1. debug articles
[done] 2. engine can run!

todo:
1. curlconn.c treat 301 redirection
2. try to write index into file, and also read it from file. No need to download article and build index everytime. Or I can optimize it with con-current programming.


##############
# 16/02/2021 #
##############
[done] 1. curlconn.c treat 301 redirection
[done] 2. rss_search user interface
[done] 3. test the whole application
[done] 4. entire application run successfully.


##############
# 17/02/2021 #
##############
[done] 1. take a break


##############
# 18/02/2021 #
##############
further optimization:
  1. concurrent curlconn
  2. data permanence


##############
# 02/03/2021 #
##############
[done] 1. finish assignment 6 - multithreads rss search engine

