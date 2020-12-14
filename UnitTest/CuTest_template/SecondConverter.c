#include <stdio.h>
#include "CuTest.h"

#define SECONDS_PER_MINUTE 60
#define SECONDS_PER_HOUR 3600

/*Function being tested*/
void second_converter(int *hours, int *minutes, int *seconds){
  int remainder = *seconds;
  *hours = remainder / SECONDS_PER_HOUR;
  remainder = remainder % SECONDS_PER_HOUR;
  *minutes = remainder / SECONDS_PER_MINUTE;
  *seconds = remainder %SECONDS_PER_MINUTE;
  return;
}

/*Test cases*/
void test4000Seconds(CuTest *tc){
  int seconds = 4000, minutes, hours;
  second_converter(&hours, &minutes, &seconds);
  int expectedHours = 1;
  int expectedMinutes = 6;
  int expectedSeconds = 40;
  CuAssertIntEquals(tc, expectedHours, hours);
  CuAssertIntEquals(tc, expectedMinutes, minutes);
  CuAssertIntEquals(tc, expectedSeconds, seconds);
}

void test75Seconds(CuTest *tc){
  int seconds = 75, minutes, hours;
  second_converter(&hours, &minutes, &seconds);
  int expectedHours = 0;
  int expectedMinutes = 1;
  int expectedSeconds = 15;
  CuAssertIntEquals(tc, expectedHours, hours);
  CuAssertIntEquals(tc, expectedMinutes, minutes);
  CuAssertIntEquals(tc, expectedSeconds, seconds);
}

void test3700Seconds(CuTest *tc){
  int seconds = 3700, minutes, hours;
  second_converter(&hours, &minutes, &seconds);
  int expectedHours = 1;
  int expectedMinutes = 1;
  int expectedSeconds = 40;
  CuAssertIntEquals(tc, expectedHours, hours);
  CuAssertIntEquals(tc, expectedMinutes, minutes);
  CuAssertIntEquals(tc, expectedSeconds, seconds);
}

void test55Seconds(CuTest *tc){
  int seconds = 55, minutes, hours;
  second_converter(&hours, &minutes, &seconds);
  int expectedHours = 0;
  int expectedMinutes = 0;
  int expectedSeconds = 55;
  CuAssertIntEquals(tc, expectedHours, hours);
  CuAssertIntEquals(tc, expectedMinutes, minutes);
  CuAssertIntEquals(tc, expectedSeconds, seconds);
}

void test3661Seconds(CuTest *tc){
  int seconds = 3661, minutes, hours;
  second_converter(&hours, &minutes, &seconds);
  int expectedHours = 1;
  int expectedMinutes = 1;
  int expectedSeconds = 1;
  CuAssertIntEquals(tc, expectedHours, hours);
  CuAssertIntEquals(tc, expectedMinutes, minutes);
  CuAssertIntEquals(tc, expectedSeconds, seconds);
}

/*Change function to reflect the testcases needed*/
CuSuite* secondConverterGetSuite(){
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test4000Seconds);
  SUITE_ADD_TEST(suite, test3661Seconds);
  SUITE_ADD_TEST(suite, test3700Seconds);
  SUITE_ADD_TEST(suite, test55Seconds);
  SUITE_ADD_TEST(suite, test75Seconds);
  return suite;
}

