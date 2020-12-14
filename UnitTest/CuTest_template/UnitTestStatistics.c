#include <stdio.h>
#include "CuTest.h"

CuSuite* StrUtilGetSuite();
/*Change prototype to relevant function name*/
CuSuite* statisticsGetSuite();

void runAllTests(void){
  CuString *output = CuStringNew();
  CuSuite* suite = CuSuiteNew();
  
  /*Change second argument to whatever the function is called*/
  CuSuiteAddSuite(suite, (CuSuite*) statisticsGetSuite());
  
  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);
  printf("%s\n", output->buffer);
  
}

int main(void){
  runAllTests();
  return 0;
}

