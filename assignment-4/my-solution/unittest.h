/**
 * This allows some static functions to be tested elsewhere.
 */
#ifdef _UNIT_TEST_
	#define notest_static
#else
	#define notest_static static
#endif