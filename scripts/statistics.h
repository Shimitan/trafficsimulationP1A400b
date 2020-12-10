/* #includes */
#include <stdlib.h>
#include <stdio.h>

/* Constants */
#define AMOUNT_OF_CARS 10
#define TICKS_PER_SECOND 10
#define SECONDS_PER_MINUTE 60
#define AMOUNT_OF_ROADS 40
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
    double maxSpeed;
    /*Flow*/
    int flowCarCount;
    double calculatedFlow;
    double optimalFlow;
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

struct road{
    
    int startID;
    int endID;
    
    double length;
    double speedLimit;
    
    /* 0 = RED, 1 = YELLOW, 2 = GREEN */
    int intersecLightStart;
    int intersecLightEnd;
    
    int maxCars;
    int currCars[100];
    
};

typedef struct data data;
typedef struct road road;

/*Prototypes*/
double* createSpeedArray(int amountOfCars, int ticksPerSecond);
void measureSpeed(double speed, data *dp, int index);
void analyseData(int amountOfRoads, int minutesSimulated, data **minuteData);
void averageSpeed(data *dp);
double mpdsTokmh(double speed);
void countCarFlow(data *dp);
void setUpDataArray(int amountOfRoads, int minutesSimulated, int amountOfCars, int ticksPerSecond, data **minuteData, int speedIndex[], int carsOnRoad[], road roadArr[]);
void printAnalysedData(int amountOfRoads, int minutesSimulated, data **minuteData);
void freeSpeedArrays(int amountOfRoads, int minutesSimulated, data **minuteData);
void calculateFlow(data *dp);
void calculateDensity(data *dp);
void calculateCongestion(data *dp);
data** createDataArray(int amountOfRoads, int minutesSimulated);
int* allocateIntArray(int length);
void calculateLargerIntervals(int amountOfRoads, int minutesSimulated, int interval, data **minuteData, data **largeIntervalData);
void makeOutputFile(int amountOfRoads, int minutesSimulated, data **minuteData, int interval);