/* #includes */
#include <stdlib.h>
#include <stdio.h>

/* Constants */
/*  */
#define AMOUNT_OF_CARS 100
#define TICKS_PER_SECOND 10
#define SECONDS_PER_MINUTE 60
#define AMOUNT_OF_ROADS 3
#define MINUTES_SIMULATED 60

#define CAR_LENGTH 5
#define MIN_SPACING 1
#define MAX_CARS 100

/*Structs*/
struct data{
    /*Time*/
    int timeStamp;
    int timeInterval;
    /*Speed*/
    int speedMeasurementCount;
    double *speedOfCars; //skal være et array af alle målte hastigheder i løbet af et minut i simulationen
    double averageSpeed;
    /*Flow*/
    int flowCarCount;
    double calculatedFlow;
    double optimalFlow;
    /*Density*/
    int densityCarCount;
    double density;
    // =  max cars / roadLength     max cars = roadlength / (carlength + minSpacing) / roadLength = 1 / (carlength + minSpacing)
    /*Road stuff*/
    int roadID;
    int roadLength;
    int direction;
    
};

typedef struct data data;

/*Prototypes*/
double* createSpeedArray(int amountOfCars, int ticksPerSecond);
void measureSpeed(double speed, data *dp, int index, int dir);
void analyseData(int amountOfRoads, int minutesSimulated, data minuteData[amountOfRoads][minutesSimulated]);
void averageSpeed(data *dp);
double mpdsTokmh(double speed);
void countCarFlow(data *dp);
void setUpDataArray(int amountOfRoads, int minutesSimulated, int amountOfCars, int ticksPerSecond, data minuteData[amountOfRoads][minutesSimulated], int speedIndex[], int carsOnRoad[]);
void printAnalysedData(int amountOfRoads, int minutesSimulated, data minuteData[amountOfRoads][minutesSimulated]);
void freeSpeedArrays(int amountOfRoads, int minutesSimulated, data minuteData[amountOfRoads][minutesSimulated]);
void calculateFlow(data *dp);
void calculateDensity(data *dp);