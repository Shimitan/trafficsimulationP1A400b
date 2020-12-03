/*#includes*/
#include <stdlib.h>
#include <stdio.h>

/*Constants*/
#define AMOUNT_OF_CARS 1000
#define TICKS_PER_SECOND 10
#define SECONDS_PER_MINUTE 60
#define AMOUNT_OF_ROADS 10
#define MINUTES_SIMULATED 30

/*Structs*/
struct data{
    int amountOfCars;
    int timeStamp;
    int speedMeasurementCount;
    double *speedOfCars; //skal være et array af alle målte hastigheder i løbet af et minut i simulationen
    double averageSpeed;
    double flow;
    double density;
    int roadID;
    int roadLength;
    int direction;
    int timeInterval;
};

typedef struct data data;

/*Prototypes*/
double* createSpeedArray(int amountOfCars, int ticksPerSecond);
void measureSpeed(double speed, data *dp, int index, int dir);
void analyseData(data *oneMinuteData);
void averageSpeed(data *dp);
double mpdsTokmh(double speed);