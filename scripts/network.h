#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"

#define INFINITY 2147483647

/* UwUbuntu */
/* OwObonto */

struct roadPoints{
    int ID;

    int numOfConnections;
    int connections[4];

    int intersecTimer;

    struct roadPoints* parent;
    double local;

};

struct car{

    double acceleration;
    double speed;
    double location;
    double breakLength;
    double speedDeviation;

    int currNode;
    int currGoal;
    int endGoal;
    int dirBool; /* 1 is positive direction. 0 is negative direction */
    int active;
    int ID;

    int path[100];
    int pathStep;
};

typedef struct car car;
typedef struct roadPoints roadPoints;

double disToEnd(car car, road road, struct car carArr[]);
void moveCar(car* car, struct car carArr[], road* road, struct road roadArr[], int carNum, int* debugBool, data *dp, int *index, int roadAmount, int *carsOnRoadCount);
double breakLength(car car);
void isCarInFront(car car, road road, struct car carArr[], double* carLocation, int* bool);
int cmpfunc (const void * a, const void * b);
double distanceBetweenCars(car car);
void pushArray(car car, road *road);
void roadOutput(car car[], road road);
double kmhTompds(road* road);
void changeRoad(car* car, road roadArr[], int* debugBool, int roadAmount);
void pathfinding(car* car, road roadArr[], struct roadPoints roadPointsArr[], int nodeAmount);
void changeLight(road roadArr[], roadPoints* node, int roadAmount);
double findAccelerationLength(car *car, road *road);