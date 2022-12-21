#include "munit.h"
#include "hash_tbl.h"
#include <stdint.h>
#include <string.h>

#include "string_data.h"

static MunitResult
test_new_free(const MunitParameter params[], void* data) {
    (void) params;
    HashTable *ht = NULL;
    for (int j = 0; j < 100; ++j) {
        ht = hashtbl_new();
        munit_assert_not_null(ht);
        hashtbl_free(ht);
    }
    return MUNIT_OK;
}

static MunitResult
test_add(const MunitParameter params[], void* data) {
    (void) params;
    HashTable *ht = hashtbl_new();
    int value = 0;
    munit_assert_true(hashtbl_add_s(ht, "hello", &value, sizeof(value)));
    munit_assert_int(hashtbl_get_count(ht), ==, 1);

    value = 101;
    munit_assert_true(hashtbl_add_s(ht, "bye", &value, sizeof(value)));
    munit_assert_int(hashtbl_get_count(ht), ==, 2);

    value = 10101;
    munit_assert_true(hashtbl_add_s(ht, "gepard", &value, sizeof(value)));
    munit_assert_int(hashtbl_get_count(ht), ==, 3);

    value = 10101;
    munit_assert_true(hashtbl_add_s(ht, "dekabr'", &value, sizeof(value)));
    munit_assert_int(hashtbl_get_count(ht), ==, 4);

    value = 10103;
    munit_assert_true(hashtbl_add_s(ht, "shilo", &value, sizeof(value)));
    munit_assert_int(hashtbl_get_count(ht), ==, 5);

    value = 10105;
    munit_assert_true(hashtbl_add_s(ht, "mulo", &value, sizeof(value)));
    munit_assert_int(hashtbl_get_count(ht), ==, 6);

    hashtbl_free(ht);
    return MUNIT_OK;
}

static MunitResult
test_add_get(const MunitParameter params[], void* data) {
    (void) params;
    HashTable *ht = hashtbl_new();
    int value, *res;

    value = 101;
    munit_assert_true(hashtbl_add_s(ht, "bye", &value, sizeof(value)));
    res = hashtbl_get_s(ht, "bye", NULL);
    munit_assert_ptr_not_null(res);
    munit_assert_int(*res, ==, value);

    value = 113;
    munit_assert_true(hashtbl_add_s(ht, "HI", &value, sizeof(value)));
    res = hashtbl_get_s(ht, "HI", NULL);
    munit_assert_ptr_not_null(res);
    munit_assert_int(*res, ==, value);

    value = 13;
    munit_assert_true(hashtbl_add_s(ht, "LOWW", &value, sizeof(value)));
    res = hashtbl_get_s(ht, "LOWW", NULL);
    munit_assert_ptr_not_null(res);
    munit_assert_int(*res, ==, value);

    value = 112;
    munit_assert_true(hashtbl_add_s(ht, "lol", &value, sizeof(value)));
    res = hashtbl_get_s(ht, "lol", NULL);
    munit_assert_ptr_not_null(res);
    munit_assert_int(*res, ==, value);

    value = 0;
    munit_assert_true(hashtbl_add_s(ht, "HELLO", &value, sizeof(value)));
    res = hashtbl_get_s(ht, "HELLO", NULL);
    munit_assert_ptr_not_null(res);
    munit_assert_int(*res, ==, value);

    value = 10101;
    munit_assert_true(hashtbl_add_s(ht, "gepard", &value, sizeof(value)));
    res = hashtbl_get_s(ht, "gepard", NULL);
    munit_assert_ptr_not_null(res);
    munit_assert_int(*res, ==, value);

    value = 10101;
    munit_assert_true(hashtbl_add_s(ht, "dekabr'", &value, sizeof(value)));
    res = hashtbl_get_s(ht, "dekabr'", NULL);
    munit_assert_ptr_not_null(res);
    munit_assert_int(*res, ==, value);

    value = 10103;
    munit_assert_true(hashtbl_add_s(ht, "shilo", &value, sizeof(value)));
    res = hashtbl_get_s(ht, "shilo", NULL);
    munit_assert_ptr_not_null(res);
    munit_assert_int(*res, ==, value);

    value = 10105;
    munit_assert_true(hashtbl_add_s(ht, "mulo", &value, sizeof(value)));
    res = hashtbl_get_s(ht, "mulo", NULL);
    munit_assert_ptr_not_null(res);
    munit_assert_int(*res, ==, value);

    hashtbl_free(ht);
    return MUNIT_OK;
}

static MunitResult
test_get(const MunitParameter params[], void* data) {
    (void) params;
    HashTable *ht = hashtbl_new();
    int value = 0;
    munit_assert_true(hashtbl_add_s(ht, "hello", &value, sizeof(value)));

    munit_assert_not_null(hashtbl_get_s(ht, "hello", NULL));
    munit_assert_null(hashtbl_get_s(ht, "Hello", NULL));
    munit_assert_null(hashtbl_get_s(ht, "Hllo", NULL));
    munit_assert_null(hashtbl_get_s(ht, "Hlloo", NULL));
    munit_assert_null(hashtbl_get_s(ht, "", NULL));

    value = 10105;
    munit_assert_true(hashtbl_add_s(ht, "mulo", &value, sizeof(value)));
    munit_assert_null(hashtbl_get_s(ht, "_mulo", NULL));
    munit_assert_null(hashtbl_get_s(ht, "mu lo", NULL));
    munit_assert_null(hashtbl_get_s(ht, "mulo_", NULL));

    hashtbl_free(ht);
    return MUNIT_OK;
}

static MunitResult
test_clear(const MunitParameter params[], void* data) {
    (void) params;
    HashTable *ht = hashtbl_new();
    int value = 0;
    munit_assert_true(hashtbl_add_s(ht, "hello", &value, sizeof(value)));

    value = 101;
    munit_assert_true(hashtbl_add_s(ht, "bye", &value, sizeof(value)));

    value = 10101;
    munit_assert_true(hashtbl_add_s(ht, "gepard", &value, sizeof(value)));

    value = 10101;
    munit_assert_true(hashtbl_add_s(ht, "dekabr'", &value, sizeof(value)));

    value = 10103;
    munit_assert_true(hashtbl_add_s(ht, "shilo", &value, sizeof(value)));

    value = 10105;
    munit_assert_true(hashtbl_add_s(ht, "mulo", &value, sizeof(value)));

    munit_assert_int(hashtbl_get_count(ht), ==, 6);
    hashtbl_clear(ht);
    munit_assert_int(hashtbl_get_count(ht), ==, 0);

    munit_assert_null(hashtbl_get_s(ht, "mulo", NULL));
    munit_assert_null(hashtbl_get_s(ht, "shilo", NULL));
    munit_assert_null(hashtbl_get_s(ht, "dekabr", NULL));
    munit_assert_null(hashtbl_get_s(ht, "gepard", NULL));
    munit_assert_null(hashtbl_get_s(ht, "bye", NULL));
    munit_assert_null(hashtbl_get_s(ht, "hello", NULL));

    hashtbl_free(ht);
    return MUNIT_OK;
}

struct HashTest {
    char *key;
    int value;
};

struct HashTest hash_test_data[] = {
    {"alfla",            0},
    {"lsdfjlew",         -12312},
    {"lsdfjlew_",        -3430},
    {"ljlew",            2450},
    {"235829",           2340},
    {"23",               0},
    {"2334322342342342", 80},
};

int hash_test_get(const char *key) {
    int len = sizeof(hash_test_data) / sizeof(struct HashTest);
    for (int j = 0; j < len; ++j) {
        if (!strcmp(hash_test_data[j].key, key)) {
            return hash_test_data[j].value;
        }
    }
    return INT32_MIN;
}

HashTableAction iter(
    void *key, int key_len, void *value, int value_len, void *data
) {
    int res = hash_test_get(key);
    if (res == INT32_MIN) {
        *((bool*)data) = true;
        return HT_ACTION_BREAK;
    }
    return HT_ACTION_NEXT;
}

void hash_test_add(HashTable *ht) {
    int len = sizeof(hash_test_data) / sizeof(struct HashTest);
    for (int j = 0; j < len; ++j) {
        hashtbl_add(
            ht, 
            hash_test_data[j].key, strlen(hash_test_data[j].key),
            &hash_test_data[j].value, sizeof(hash_test_data[j])
        );
    }
}

static MunitResult
test_iter1(const MunitParameter params[], void* data) {
    (void) params;
    HashTable *ht = hashtbl_new();
    bool failed = false;
    hash_test_add(ht);
    hashtbl_each(ht, iter, &failed);
    munit_assert_false(failed);
    hashtbl_free(ht);
    return MUNIT_OK;
}

static MunitResult
test_iter2(const MunitParameter params[], void* data) {
    (void) params;
    return MUNIT_OK;
}

static MunitResult
test_remove(const MunitParameter params[], void* data) {
    (void) params;
    HashTable *ht = hashtbl_new();
    /*hash_test_add(ht);*/
    munit_assert_false(hashtbl_remove_s(ht, "non-existing-key"));
    hashtbl_free(ht);
    return MUNIT_OK;
}

static MunitResult
test_add_remove(const MunitParameter params[], void* data) {
    (void) params;
    HashTable *ht = hashtbl_new();
    hash_test_add(ht);
    int len = sizeof(hash_test_data) / sizeof(struct HashTest);
    for (int j = 0; j < len; ++j) {
        munit_assert_true(hashtbl_remove(
            ht, hash_test_data[j].key, strlen(hash_test_data[j].key)
        ));
    }
    munit_assert_int(hashtbl_get_count(ht), ==, 0);
    hashtbl_free(ht);
    return MUNIT_OK;
}

static MunitResult
test_add_remove_get(const MunitParameter params[], void* data) {
    (void) params;
    return MUNIT_OK;
}

/* Tests are functions that return void, and take a single void*
 * parameter.  We'll get to what that parameter is later. */
static MunitResult
test_compare(const MunitParameter params[], void* data) {
  /* We'll use these later */
  const unsigned char val_uchar = 'b';
  const short val_short = 1729;
  double pi = 3.141592654;
  char* stewardesses = "stewardesses";
  char* most_fun_word_to_type;

  /* These are just to silence compiler warnings about the parameters
   * being unused. */
  (void) params;
  (void) data;

  /* Let's start with the basics. */
  munit_assert(0 != 1);

  /* There is also the more verbose, though slightly more descriptive
     munit_assert_true/false: */
  munit_assert_false(0);

  /* You can also call munit_error and munit_errorf yourself.  We
   * won't do it is used to indicate a failure, but here is what it
   * would look like: */
  /* munit_error("FAIL"); */
  /* munit_errorf("Goodbye, cruel %s", "world"); */

  /* There are macros for comparing lots of types. */
  munit_assert_char('a', ==, 'a');

  /* Sure, you could just assert('a' == 'a'), but if you did that, a
   * failed assertion would just say something like "assertion failed:
   * val_uchar == 'b'".  µnit will tell you the actual values, so a
   * failure here would result in something like "assertion failed:
   * val_uchar == 'b' ('X' == 'b')." */
  munit_assert_uchar(val_uchar, ==, 'b');

  /* Obviously we can handle values larger than 'char' and 'uchar'.
   * There are versions for char, short, int, long, long long,
   * int8/16/32/64_t, as well as the unsigned versions of them all. */
  munit_assert_short(42, <, val_short);

  /* There is also support for size_t.
   *
   * The longest word in English without repeating any letters is
   * "uncopyrightables", which has uncopyrightable (and
   * dermatoglyphics, which is the study of fingerprints) beat by a
   * character */
  munit_assert_size(strlen("uncopyrightables"), >, strlen("dermatoglyphics"));

  /* Of course there is also support for doubles and floats. */
  munit_assert_double(pi, ==, 3.141592654);

  /* If you want to compare two doubles for equality, you might want
   * to consider using munit_assert_double_equal.  It compares two
   * doubles for equality within a precison of 1.0 x 10^-(precision).
   * Note that precision (the third argument to the macro) needs to be
   * fully evaluated to an integer by the preprocessor so µnit doesn't
   * have to depend pow, which is often in libm not libc. */
  munit_assert_double_equal(3.141592654, 3.141592653589793, 9);

  /* And if you want to check strings for equality (or inequality),
   * there is munit_assert_string_equal/not_equal.
   *
   * "stewardesses" is the longest word you can type on a QWERTY
   * keyboard with only one hand, which makes it loads of fun to type.
   * If I'm going to have to type a string repeatedly, let's make it a
   * good one! */
  munit_assert_string_equal(stewardesses, "stewardesses");

  /* A personal favorite macro which is fantastic if you're working
   * with binary data, is the one which naïvely checks two blobs of
   * memory for equality.  If this fails it will tell you the offset
   * of the first differing byte. */
  munit_assert_memory_equal(7, stewardesses, "steward");

  /* You can also make sure that two blobs differ *somewhere*: */
  munit_assert_memory_not_equal(8, stewardesses, "steward");

  /* There are equal/not_equal macros for pointers, too: */
  most_fun_word_to_type = stewardesses;
  munit_assert_ptr_equal(most_fun_word_to_type, stewardesses);

  /* And null/not_null */
  munit_assert_null(NULL);
  munit_assert_not_null(most_fun_word_to_type);

  /* Lets verify that the data parameter is what we expected.  We'll
   * see where this comes from in a bit.
   *
   * Note that the casting isn't usually required; if you give this
   * function a real pointer (instead of a number like 0xdeadbeef) it
   * would work as expected. */
  munit_assert_ptr_equal(data, (void*)(uintptr_t)0xdeadbeef);

  return MUNIT_OK;
}

static MunitResult
test_rand(const MunitParameter params[], void* user_data) {
  int random_int;
  double random_dbl;
  munit_uint8_t data[5];

  (void) params;
  (void) user_data;

  /* One thing missing from a lot of unit testing frameworks is a
   * random number generator.  You can't just use srand/rand because
   * the implementation varies across different platforms, and it's
   * important to be able to look at the seed used in a failing test
   * to see if you can reproduce it.  Some randomness is a fantastic
   * thing to have in your tests, I don't know why more people don't
   * do it...
   *
   * µnit's PRNG is re-seeded with the same value for each iteration
   * of each test.  The seed is retrieved from the MUNIT_SEED
   * envirnment variable or, if none is provided, one will be
   * (pseudo-)randomly generated. */

  /* If you need an integer in a given range */
  random_int = munit_rand_int_range(128, 4096);
  munit_assert_int(random_int, >=, 128);
  munit_assert_int(random_int, <=, 4096);

  /* Or maybe you want a double, between 0 and 1: */
  random_dbl = munit_rand_double();
  munit_assert_double(random_dbl, >=, 0.0);
  munit_assert_double(random_dbl, <=, 1.0);

  /* Of course, you want to be able to reproduce bugs discovered
   * during testing, so every time the tests are run they print the
   * random seed used.  When you want to reproduce a result, just put
   * that random seed in the MUNIT_SEED environment variable; it even
   * works on different platforms.
   *
   * If you want this to pass, use 0xdeadbeef as the random seed and
   * uncomment the next line of code.  Note that the PRNG is not
   * re-seeded between iterations of the same test, so this will only
   * work on the first iteration. */
  /* munit_assert_uint32(munit_rand_uint32(), ==, 1306447409); */

  /* You can also get blobs of random memory: */
  munit_rand_memory(sizeof(data), data);

  return MUNIT_OK;
}

/* This test case shows how to accept parameters.  We'll see how to
 * specify them soon.
 *
 * By default, every possible variation of a parameterized test is
 * run, but you can specify parameters manually if you want to only
 * run specific test(s), or you can pass the --single argument to the
 * CLI to have the harness simply choose one variation at random
 * instead of running them all. */
static MunitResult
test_parameters(const MunitParameter params[], void* user_data) {
  const char* foo;
  const char* bar;

  (void) user_data;

  /* The "foo" parameter is specified as one of the following values:
   * "one", "two", or "three". */
  foo = munit_parameters_get(params, "foo");
  /* Similarly, "bar" is one of "four", "five", or "six". */
  bar = munit_parameters_get(params, "bar");
  /* "baz" is a bit more complicated.  We don't actually specify a
   * list of valid values, so by default NULL is passed.  However, the
   * CLI will accept any value.  This is a good way to have a value
   * that is usually selected randomly by the test, but can be
   * overridden on the command line if desired. */
  /* const char* baz = munit_parameters_get(params, "baz"); */

  /* Notice that we're returning MUNIT_FAIL instead of writing an
   * error message.  Error messages are generally preferable, since
   * they make it easier to diagnose the issue, but this is an
   * option.
   *
   * Possible values are:
   *  - MUNIT_OK: Sucess
   *  - MUNIT_FAIL: Failure
   *  - MUNIT_SKIP: The test was skipped; usually this happens when a
   *    particular feature isn't in use.  For example, if you're
   *    writing a test which uses a Wayland-only feature, but your
   *    application is running on X11.
   *  - MUNIT_ERROR: The test failed, but not because of anything you
   *    wanted to test.  For example, maybe your test downloads a
   *    remote resource and tries to parse it, but the network was
   *    down.
   */

  if (strcmp(foo, "one") != 0 &&
      strcmp(foo, "two") != 0 &&
      strcmp(foo, "three") != 0)
    return MUNIT_FAIL;

  if (strcmp(bar, "red") != 0 &&
      strcmp(bar, "green") != 0 &&
      strcmp(bar, "blue") != 0)
    return MUNIT_FAIL;

  return MUNIT_OK;
}

/* The setup function, if you provide one, for a test will be run
 * before the test, and the return value will be passed as the sole
 * parameter to the test function. */
static void*
test_compare_setup(const MunitParameter params[], void* user_data) {
  (void) params;

  munit_assert_string_equal(user_data, "µnit");
  return (void*) (uintptr_t) 0xdeadbeef;
}

/* To clean up after a test, you can use a tear down function.  The
 * fixture argument is the value returned by the setup function
 * above. */
static void
test_compare_tear_down(void* fixture) {
  munit_assert_ptr_equal(fixture, (void*)(uintptr_t)0xdeadbeef);
}

static char* foo_params[] = {
  (char*) "one", (char*) "two", (char*) "three", NULL
};

static char* bar_params[] = {
  (char*) "red", (char*) "green", (char*) "blue", NULL
};

static MunitParameterEnum test_params[] = {
  { (char*) "foo", foo_params },
  { (char*) "bar", bar_params },
  { (char*) "baz", NULL },
  { NULL, NULL },
};

static MunitTest CIRC_BUF_tests[] = {
  { (char*) "/hash_table/new_free", test_new_free, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/hash_table/add", test_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/hash_table/add_get", test_add_get, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/hash_table/get", test_get, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/hash_table/clear", test_clear, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/hash_table/iter1", test_iter1, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/hash_table/iter2", test_iter2, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/hash_table/remove", test_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/hash_table/add_remove", test_add_remove, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/hash_table/add_remove_get", test_add_remove_get, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
};

/* Creating a test suite is pretty simple.  First, you'll need an
 * array of tests: */
static MunitTest test_suite_tests[] = {
  /* Usually this is written in a much more compact format; all these
   * comments kind of ruin that, though.  Here is how you'll usually
   * see entries written: */
  { (char*) "/example/rand", test_rand, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  /* To tell the test runner when the array is over, just add a NULL
   * entry at the end. */
  /*{ (char*) "/example/parameters", test_parameters, NULL, NULL, MUNIT_TEST_OPTION_NONE, test_params },*/
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

/* Now we'll actually declare the test suite.  You could do this in
 * the main function, or on the heap, or whatever you want. */
static const MunitSuite test_suite = {
  /* This string will be prepended to all test names in this suite;
   * for example, "/example/rand" will become "/µnit/example/rand".
   * Note that, while it doesn't really matter for the top-level
   * suite, NULL signal the end of an array of tests; you should use
   * an empty string ("") instead. */
  (char*) "",
  /* The first parameter is the array of test suites. */
  /*test_suite_tests,*/
  CIRC_BUF_tests,
  /* In addition to containing test cases, suites can contain other
   * test suites.  This isn't necessary in this example, but it can be
   * a great help to projects with lots of tests by making it easier
   * to spread the tests across many files.  This is where you would
   * put "other_suites" (which is commented out above). */
  NULL,
  /* An interesting feature of µnit is that it supports automatically
   * running multiple iterations of the tests.  This is usually only
   * interesting if you make use of the PRNG to randomize your tests
   * cases a bit, or if you are doing performance testing and want to
   * average multiple runs.  0 is an alias for 1. */
  1,
  /* Just like MUNIT_TEST_OPTION_NONE, you can provide
   * MUNIT_SUITE_OPTION_NONE or 0 to use the default settings. */
  MUNIT_SUITE_OPTION_NONE
};

/* This is only necessary for EXIT_SUCCESS and EXIT_FAILURE, which you
 * *should* be using but probably aren't (no, zero and non-zero don't
 * always mean success and failure).  I guess my point is that nothing
 * about µnit requires it. */
#include <stdlib.h>

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  /* Finally, we'll actually run our test suite!  That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&test_suite, (void*) "µnit", argc, argv);
}
