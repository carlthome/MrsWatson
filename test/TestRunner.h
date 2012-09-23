#ifndef MrsWatsonTest_TestRunner_h
#define MrsWatsonTest_TestRunner_h

#include <stdio.h>
#include <string.h>
#if ! WINDOWS
#include <unistd.h>
#endif
#include "PlatformUtilities.h"
#include "CharString.h"

#ifndef __func__
#define __func__ __FUNCTION__
#endif

#define ANSI_COLOR_BLACK   "[30m"
#define ANSI_COLOR_RED     "[31m"
#define ANSI_COLOR_GREEN   "[32m"
#define ANSI_COLOR_YELLOW  "[33m"
#define ANSI_COLOR_BLUE    "[34m"
#define ANSI_COLOR_MAGENTA "[35m"
#define ANSI_COLOR_CYAN    "[36m"
#define ANSI_COLOR_WHITE   "[37m"
#define ANSI_COLOR_RESET   "[0m"

typedef int (*TestCaseExecFunc)(void);
typedef void (*TestCaseSetupFunc)(void);
typedef void (*TestCaseTeardownFunc)(void);

typedef struct {
  char* name;
  char* filename;
  int lineNumber;
  TestCaseExecFunc testCaseFunc;
} TestCaseMembers;
typedef TestCaseMembers* TestCase;

typedef struct {
  char* name;
  int numSuccess;
  int numFail;
  LinkedList testCases;
  TestCaseSetupFunc setup;
  TestCaseTeardownFunc teardown;
} TestSuiteMembers;
typedef TestSuiteMembers* TestSuite;

void addTestToTestSuite(TestSuite testSuite, TestCase testCase);
void runTestSuite(void* testSuitePtr, void* extraData);
void runTestCase(void* item, void* extraData);
void printTestSuccess(void);
void printTestFail(void);

TestSuite newTestSuite(char* name, TestCaseSetupFunc setup, TestCaseTeardownFunc teardown);
TestCase newTestCase(char* name, char* filename, int lineNumber, TestCaseExecFunc testCaseFunc);

#define addTest(testSuite, name, testCaseFunc) { \
  addTestToTestSuite(testSuite, newTestCase(name, __FILE__, __LINE__, testCaseFunc)); \
}

#define _assert(condition) { \
  if(!(condition)) { \
    printTestFail(); \
    printf("    at %s:%d\n", __FILE__, __LINE__); \
    return 1; \
  } \
}

#define _assertFalse(condition) _assert((condition) == false)
#define _assertIsNull(condition) _assert((condition) == NULL)
#define _assertNotNull(condition) _assert((condition) != NULL)

#define _assertIntEquals(condition, expected) { \
  int result = condition; \
  if(result != expected) { \
    printTestFail(); \
    printf("    at %s:%d. Expected %d, got %d.\n", __FILE__, __LINE__, expected, result); \
    return 1; \
  } \
}

#define _assertDoubleEquals(condition, expected) { \
  double result = condition; \
  if(result != expected) { \
    printTestFail(); \
    printf("    at %s:%d. Expected %g, got %g.\n", __FILE__, __LINE__, expected, result); \
    return 1; \
  } \
}

#define _assertCharStringEquals(result, expected) { \
  if(!isCharStringEqualToCString(result, expected, false)) { \
    printTestFail(); \
    printf("    at %s:%d. Expected %s, got %s.\n", __FILE__, __LINE__, expected, result->data); \
    return 1; \
  } \
}

#if WINDOWS
#define FILE_BASENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define FILE_BASENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#endif