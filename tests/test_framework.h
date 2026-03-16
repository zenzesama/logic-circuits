#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

extern int printf(const char* format, ...);

static int _tests_run = 0;
static int _tests_failed = 0;

/**
 * @brief Assert that two bits are equal,
 *        prints output only if failed.
 * 
 */
#define ASSERT_BIT_EQ(expected, actual, label)\
    do {\
        _tests_run++;\
        if ((expected) != (actual)) {\
            _tests_failed++;\
            printf("  FAIL [%s]: expected %d, got %d\n",\
                   label, (int)(expected), (int)(actual));\
        }\
    } while(0)

/**
 * @brief prints a summary for the current test
 *        call once at end of each test file's main()
 * 
 */
#define PRINT_SUMMARY(suite_name)\
    do {\
        if (_tests_failed == 0)\
            printf("[%s] All %d tests passed.\n", suite_name, _tests_run);\
        else\
            printf("[%s] %d/%d tests FAILED.\n", suite_name,\
                   _tests_failed, _tests_run);\
    } while(0)

#endif