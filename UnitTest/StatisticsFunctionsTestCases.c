#include <stdio.h>
#include <stdlib.h>
#include "CuTest.h"

/*Function being tested*/
struct data{
    /*Time*/
    int timeStamp;
    int timeInterval;
    /*Speed*/
    int speedMeasurementCount;
    double *speedOfCars; //skal være et array af alle målte hastigheder i løbet af et minut i simulationen
    double averageSpeed;
    double maxSpeed;
    /*Flow*/
    int flowCarCount;
    double calculatedFlow;
    /*Density*/
    int densityCarCount;
    double density;
    // =  max cars / roadLength     max cars = roadlength / (carlength + minSpacing) / roadLength = 1 / (carlength + minSpacing)
    /*Congestion*/
    int congestion;
    /*Road stuff*/
    int roadID;
    int roadLength;
    int direction;
};
typedef struct data data;

double mpdsTokmh(double speed){
    return speed /* * 10 * 3.6*/;
}

void averageSpeed(data *dp){
    int i;
    double average = 0.0;
    
    if (dp->speedMeasurementCount > 0){
        for (i = 0; i < dp->speedMeasurementCount; i++){
            average += dp->speedOfCars[i];
        }
        average = (double) average/dp->speedMeasurementCount;
    }
    dp->averageSpeed = mpdsTokmh(average);
}

void calculateFlow(data *dp){
    dp->calculatedFlow = (double) dp->flowCarCount / dp->timeInterval;
}

void calculateDensity(data *dp){
    dp->density = (double) (dp->densityCarCount * 1000) / (dp->roadLength) ;
}

/*Test cases*/
void testFlow0Cars1min(CuTest *tc){
    data testData;
    testData.flowCarCount = 0;
    testData.timeInterval = 1;
    calculateFlow(&testData);
    double expectedFlow = 0.0;
    CuAssertDblEquals(tc, expectedFlow, testData.calculatedFlow,0.000001);
}

void testFlow0Cars15min(CuTest *tc){
    data testData;
    testData.flowCarCount = 0;
    testData.timeInterval = 15;
    calculateFlow(&testData);
    double expectedFlow = 0.0;
    CuAssertDblEquals(tc, expectedFlow, testData.calculatedFlow,0.000001);
}

void testFlow0Cars60min(CuTest *tc){
    data testData;
    testData.flowCarCount = 0;
    testData.timeInterval = 1;
    calculateFlow(&testData);
    double expectedFlow = 0.0;
    CuAssertDblEquals(tc, expectedFlow, testData.calculatedFlow,0.000001);
}

void testFlow1Cars1min(CuTest *tc){
    data testData;
    testData.flowCarCount = 1;
    testData.timeInterval = 1;
    calculateFlow(&testData);
    double expectedFlow = 1.0;
    CuAssertDblEquals(tc, expectedFlow, testData.calculatedFlow,0.000001);
}

void testFlow1Cars15min(CuTest *tc){
    data testData;
    testData.flowCarCount = 1;
    testData.timeInterval = 15;
    calculateFlow(&testData);
    double expectedFlow = 0.066666;
    CuAssertDblEquals(tc, expectedFlow, testData.calculatedFlow,0.000001);
}

void testFlow1Cars60min(CuTest *tc){
    data testData;
    testData.flowCarCount = 1;
    testData.timeInterval = 60;
    calculateFlow(&testData);
    double expectedFlow = 0.016666;
    CuAssertDblEquals(tc, expectedFlow, testData.calculatedFlow,0.000001);
}

void testFlow150Cars1min(CuTest *tc){
    data testData;
    testData.flowCarCount = 150;
    testData.timeInterval = 1;
    calculateFlow(&testData);
    double expectedFlow = 150.0;
    CuAssertDblEquals(tc, expectedFlow, testData.calculatedFlow,0.000001);
}

void testFlow150Cars15min(CuTest *tc){
    data testData;
    testData.flowCarCount = 150;
    testData.timeInterval = 15;
    calculateFlow(&testData);
    double expectedFlow = 10.0;
    CuAssertDblEquals(tc, expectedFlow, testData.calculatedFlow,0.000001);
}

void testFlow150Cars60min(CuTest *tc){
    data testData;
    testData.flowCarCount = 150;
    testData.timeInterval = 60;
    calculateFlow(&testData);
    double expectedFlow = 2.5;
    CuAssertDblEquals(tc, expectedFlow, testData.calculatedFlow,0.000001);
}

void testAverageSpeed1(CuTest *tc){
    data testData;
    testData.speedMeasurementCount = 5;
    double *testSpeed = (double *) malloc(testData.speedMeasurementCount * sizeof(double));
    testSpeed[0] = 2.0;
    testSpeed[1] = 2.0;
    testSpeed[2] = 2.0;
    testSpeed[3] = 2.0;
    testSpeed[4] = 2.0;
    testData.speedOfCars = testSpeed;
    averageSpeed(&testData);
    double expectedSpeed = 2.0;
    CuAssertDblEquals(tc, expectedSpeed, testData.averageSpeed,0.000001);
}

void testAverageSpeed2(CuTest *tc){
    data testData;
    testData.speedMeasurementCount = 10;
    double *testSpeed = (double *) malloc(testData.speedMeasurementCount * sizeof(double));
    testSpeed[0] = 1.6;
    testSpeed[1] = 1.7;
    testSpeed[2] = 1.8;
    testSpeed[3] = 1.9;
    testSpeed[4] = 2.0;
    testSpeed[5] = 2.0;
    testSpeed[6] = 2.1;
    testSpeed[7] = 2.2;
    testSpeed[8] = 2.3;
    testSpeed[9] = 2.4;
    testData.speedOfCars = testSpeed;
    averageSpeed(&testData);
    double expectedSpeed = 2.0;
    CuAssertDblEquals(tc, expectedSpeed, testData.averageSpeed,0.000001);
}

void testAverageSpeed3(CuTest *tc){
    data testData;
    testData.speedMeasurementCount = 15;
    double *testSpeed = (double *) malloc(testData.speedMeasurementCount * sizeof(double));
    testSpeed[0] = 4.6;
    testSpeed[1] = 1.2;
    testSpeed[2] = 2.3;
    testSpeed[3] = 9.9;
    testSpeed[4] = 2.5;
    testSpeed[5] = 5.0;
    testSpeed[6] = 6.3;
    testSpeed[7] = 7.2;
    testSpeed[8] = 8.4;
    testSpeed[9] = 10.4;
    testSpeed[10] = 1.0;
    testSpeed[11] = 3.6;
    testSpeed[12] = 2.7;
    testSpeed[13] = 4.8;
    testSpeed[14] = 4.1;
    testData.speedOfCars = testSpeed;
    averageSpeed(&testData);
    double expectedSpeed = 4.933333;
    CuAssertDblEquals(tc, expectedSpeed, testData.averageSpeed,0.000001);
}

void testDensity0car1km(CuTest *tc){
    data testData;
    testData.densityCarCount = 0;
    testData.roadLength = 1000;
    calculateDensity(&testData);
    double expectedDensity = 0.0;
    CuAssertDblEquals(tc, expectedDensity, testData.density,0.000001);
}

void testDensity0car4km(CuTest *tc){
    data testData;
    testData.densityCarCount = 0;
    testData.roadLength = 4000;
    calculateDensity(&testData);
    double expectedDensity = 0.0;
    CuAssertDblEquals(tc, expectedDensity, testData.density,0.000001);
}

void testDensity0car7km(CuTest *tc){
    data testData;
    testData.densityCarCount = 0;
    testData.roadLength = 7000;
    calculateDensity(&testData);
    double expectedDensity = 0.0;
    CuAssertDblEquals(tc, expectedDensity, testData.density,0.000001);
}


void testDensity1car1km(CuTest *tc){
    data testData;
    testData.densityCarCount = 1;
    testData.roadLength = 1000;
    calculateDensity(&testData);
    double expectedDensity = 1.0;
    CuAssertDblEquals(tc, expectedDensity, testData.density,0.000001);
}

void testDensity1car4km(CuTest *tc){
    data testData;
    testData.densityCarCount = 1;
    testData.roadLength = 4000;
    calculateDensity(&testData);
    double expectedDensity = 0.25;
    CuAssertDblEquals(tc, expectedDensity, testData.density,0.000001);
}

void testDensity1car7km(CuTest *tc){
    data testData;
    testData.densityCarCount = 1;
    testData.roadLength = 7000;
    calculateDensity(&testData);
    double expectedDensity = 0.142857;
    CuAssertDblEquals(tc, expectedDensity, testData.density,0.000001);
}

void testDensity166car1km(CuTest *tc){
    data testData;
    testData.densityCarCount = 166;
    testData.roadLength = 1000;
    calculateDensity(&testData);
    double expectedDensity = 166.0;
    CuAssertDblEquals(tc, expectedDensity, testData.density,0.000001);
}

void testDensity166car4km(CuTest *tc){
    data testData;
    testData.densityCarCount = 166;
    testData.roadLength = 4000;
    calculateDensity(&testData);
    double expectedDensity = 41.5;
    CuAssertDblEquals(tc, expectedDensity, testData.density,0.000001);
}

void testDensity166car7km(CuTest *tc){
    data testData;
    testData.densityCarCount = 166;
    testData.roadLength = 7000;
    calculateDensity(&testData);
    double expectedDensity = 23.714285;
    CuAssertDblEquals(tc, expectedDensity, testData.density,0.000001);
}

/*Change function to reflect the testcases needed*/
CuSuite* statisticsGetSuite(){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, testFlow0Cars1min);
    SUITE_ADD_TEST(suite, testFlow0Cars15min);
    SUITE_ADD_TEST(suite, testFlow0Cars60min);
    SUITE_ADD_TEST(suite, testFlow1Cars1min);
    SUITE_ADD_TEST(suite, testFlow1Cars15min);
    SUITE_ADD_TEST(suite, testFlow1Cars60min);
    SUITE_ADD_TEST(suite, testFlow150Cars1min);
    SUITE_ADD_TEST(suite, testFlow150Cars15min);
    SUITE_ADD_TEST(suite, testFlow150Cars60min);
    SUITE_ADD_TEST(suite, testAverageSpeed1);
    SUITE_ADD_TEST(suite, testAverageSpeed2);
    SUITE_ADD_TEST(suite, testAverageSpeed3);
    SUITE_ADD_TEST(suite, testDensity0car1km);
    SUITE_ADD_TEST(suite, testDensity0car4km);
    SUITE_ADD_TEST(suite, testDensity0car7km);
    SUITE_ADD_TEST(suite, testDensity1car1km);
    SUITE_ADD_TEST(suite, testDensity1car4km);
    SUITE_ADD_TEST(suite, testDensity1car7km);
    SUITE_ADD_TEST(suite, testDensity166car1km);
    SUITE_ADD_TEST(suite, testDensity166car4km);
    SUITE_ADD_TEST(suite, testDensity166car7km);
  return suite;
}

