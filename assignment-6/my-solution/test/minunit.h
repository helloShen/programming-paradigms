/**
 * 3 Largely used C unit-test frameworks:
 * --------------------------------------
 * ==> 1. [MinUnit](http://www.jera.com/techinfo/jtns/jtn002.html)
 * ==> 2. [Check](https://libcheck.github.io/check/)
 * [Tutorial for Check](https://libcheck.github.io/check/doc/check_html/check_3.html). 
 * ==> 3. [gtest](https://github.com/google/googletest)
 *
 *
 * Interesting Knowledges:
 * -----------------------
 * ==> 1. Some discussion on topic of unit-test toolkits in C on stackoverflow: 
 * <https://stackoverflow.com/questions/65820/unit-testing-c-code>
 * ==> 2. Another way to build project using 
 * [Autotools](https://www.lrde.epita.fr/~adl/autotools.html), it seems better than Makefile.
 * ==> 3. A great pragmatic book of C programming: 
 * [C Programming FAQs: Frequently Asked Questions](http://c-faq.com/index.html)
 * ==> 4. Another great reference: 
 * [CERT C Coding Standard](https://wiki.sei.cmu.edu/confluence/display/c/SEI+CERT+C+Coding+Standard)
 *
 *
 * Some thoughts:
 * --------------
 * 1. Currently I wiil just include [MinUnit] as a basic unit-test Macro. 
 I don't want to introduce too much complex into this project. Keep simple, keep foolish.
 * 2. After finish basic features, we can try to use [Check] and [Autotools].
 *
 *
 * MinUnit is the simplest unit-test tool.
 * ---------------------------------------
 * There'are nothing but two macros.
 * To activate MinUnit, just include minunit.h into the source file.
 * For more info, please check:
 * ==> <http://www.jera.com/techinfo/jtns/jtn002.html>
 *
 * ===================
 * Here's an example #
 * ===================
 *  #include <stdio.h>
 *  #include "minunit.h"
 * 
 *  int tests_run = 0;
 *  
 *  int foo = 7;
 *  int bar = 4;
 *  
 *  static char * test_foo() {
 *      mu_assert("error, foo != 7", foo == 7);
 *      return 0;
 *  }
 *  
 *  static char * test_bar() {
 *      mu_assert("error, bar != 5", bar == 5);
 *      return 0;
 *  }
 *  
 *  static char * all_tests() {
 *      mu_run_test(test_foo);
 *      mu_run_test(test_bar);
 *      return 0;
 *  }
 *  
 *  int main(int argc, char **argv) {
 *      char *result = all_tests();
 *      if (result != 0) {
 *          printf("%s\n", result);
 *      }
 *      else {
 *          printf("ALL TESTS PASSED\n");
 *      }
 *      printf("Tests run: %d\n", tests_run);
 *  
 *      return result != 0;
 *  }
 */
 #define mu_assert(message, test) do { if (!(test)) return message; } while (0)
 #define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
 extern int tests_run;
