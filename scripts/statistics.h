/*#includes*/
#include <stdlib.h>
#include <stdio.h>

/*Constants*/
#define AMOUNT_OF_CARS 100
#define TICKS_PER_SECOND 10
#define SECONDS_PER_MINUTE 60
#define AMOUNT_OF_ROADS 100
#define MINUTES_SIMULATED 120

/*Structs*/
struct data{
    int amountOfCars;
    int timeStamp;
    double *speedOfCars; //skal være et array af alle målte hastigheder i løbet af et minut i simulationen
    double averageSpeed;
    double flow;
    double density;
    int roadID;
    int roadLength;
    int timeInterval;
};

typedef struct data data;

/*Prototypes*/
double* createSpeedArray(int amountOfCars, int ticksPerSecond);

