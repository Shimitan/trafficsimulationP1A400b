#include "network.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_CARS 110
/*
double disToEnd(car car, road road, struct car carArr[]);
void moveCar(car* car, struct car carArr[], road* road, struct road roadArr[], int carNum, int* debugBool);
double breakLength(car car);
void isCarInFront(car car, road road, struct car carArr[], double* carLocation, int* bool);
int cmpfunc (const void * a, const void * b);
double distanceBetweenCars(car car);
void roadOutput(car car[], road road);
double kmhTompds(road* road);
void changeRoad(car* car, road roadArr[], int* debugBool);
*/

void createCar(car* car, road* road, int* k, struct road roadArr[], struct roadPoints roadPointsArr[]);


int main(void){
    int debugBool = 0;
    int i = 0;
    int l = 0;
    int active = 0;
    int k = 0;
    int j = 110;
    int speedIndex = 0, ticks = 0, minuteIndex = 0;
    road road;
    //Her står der Struct fordi ellers virkede den ikk... Idk why
    struct road roadArr[100];
    roadPoints nodeArr[100];
    car car[1000];

    /*Allocates a 2D array for data collection statically*/
    data minuteData[AMOUNT_OF_ROADS][MINUTES_SIMULATED];
    for (i = 0; i < AMOUNT_OF_ROADS; i++){
        for (k = 0; k < MINUTES_SIMULATED; k++){
            minuteData[i][k].speedOfCars = createSpeedArray(AMOUNT_OF_CARS, TICKS_PER_SECOND);
            if (minuteData[i][k].speedOfCars == NULL){
                printf("Error allocating memory for [%d][%d]\n", i, k);
                exit(EXIT_FAILURE);
            } else {
                minuteData[i][k].speedMeasurementCount = 0;
            }
        }
    }



    for(i = 0; i < 1000; i++){
        car[i].active = 0;
        car[i].pathStep = 0;
        for (l = 0; l < 100; l++) {
            car[i].path[l] = -1;
        }
    }

    // for(i = 0; i < 1000; i++){
    //     for (l = 0; l < 3; l++) {
    //         car[i].path[l] = l;
    //     }
    // }

    for(i = 0; i < 100; i++){
        nodeArr[i].ID = -1;
        roadArr[i].startID = -1;
        roadArr[i].endID = -1;
    }
    for(i = 0; i < 3; i++){
        nodeArr[i].ID = i;
    }
    for(i = 0; i < 2; i++){
        roadArr[i].length = (i + 1) * 1000;
        roadArr[i].speedLimit = (i + 1) * 50;
        roadArr[i].speedLimit = kmhTompds(&roadArr[i]);
        roadArr[i].startID = i;
        roadArr[i].endID = i + 1;

        //printf("Road[%d].startID: %d\n", i, roadArr[i].startID);
        //printf("Road[%d].endID: %d\n", i, roadArr[i].endID);
        //printf("Road[%d).length: %lf\n", i, roadArr[i].length);
    }

    srand(420);

    for (l = 0; l < 100; l++){
        for(i = 0; i < 100; i++){
            roadArr[l].currCars[i] = -1;
        }
    }

    nodeArr[0].connections[0] = 1;
    nodeArr[0].numOfConnections = 1;

    nodeArr[1].connections[0] = 0;
    nodeArr[1].connections[1] = 2;
    nodeArr[1].numOfConnections = 2;

    nodeArr[2].connections[0] = 1;
    nodeArr[2].numOfConnections = 1;

    // while(1){
    //     j++;
    //     if(j >= 120){
    //         createCar(&car[k], &road, &k);
    //         j = 0;
    //     }
        
    //     for(i = 0; i < k; i++){
    //         moveCar(&car[i], car, &road, i);
    //         if(car[i].active == 1){
    //             printf("Location = %lf, Speed = %lf, ID = %d\n\n", car[i].location, car[i].speed, car[i].ID);
    //         }
    //     }

    for(i = 0; i < 1; i++){
        //Random midlertidigt fix xd
        car[i].currNode = car[i].path[0];
        car[i].endGoal = car[i].path[2];
        car[i].currGoal = car[i].path[car[i].pathStep + 1];

        for (l = 0; l < 100; l++) {
            if ((roadArr[l].startID == car[i].currNode && roadArr[l].endID == car[i].currGoal) || (roadArr[l].endID == car[i].currNode && roadArr[l].startID == car[i].currGoal)) {
                createCar(&car[i], &roadArr[l], &k, roadArr, nodeArr);
                // printf("Car[%d].currNode: %d\n", i, car[i].currNode);
                // printf("Car[%d].currGoal: %d\n", i, car[i].currGoal);
                break;
            }
        }
    }


    printf("speedIndex = %d, minuteIndex = %d\n", speedIndex, minuteIndex);
    while (debugBool == 0) {
        for(i = 0; i < 1; i++){
            for (l = 0; l < 100; l++) {
                if ((roadArr[l].startID == car[i].currNode && roadArr[l].endID == car[i].currGoal) || (roadArr[l].endID == car[i].currNode && roadArr[l].startID == car[i].currGoal)) {
                    moveCar(&car[i], car, &roadArr[l], roadArr, i, &debugBool, &minuteData[l][minuteIndex], speedIndex);
                    minuteData[l][minuteIndex].roadID = l;
                    speedIndex++;
                    //printf("CarActive?: %d\n", car[i].active);
                    break;
                }
            }
            if(car[i].active == 1){
                //printf("Location = %lf, Speed = %lf, ID = %d\n\n", car[i].location, car[i].speed, car[i].ID);
            }
        }
        ticks++;
        if (ticks % (SECONDS_PER_MINUTE * TICKS_PER_SECOND) == 0) {
            minuteIndex++;
            speedIndex = 0;
            printf("speedIndex = %d, minuteIndex = %d\n", speedIndex, minuteIndex);
        }
    }

    //     if(k > 100){
    //         printf("breakDOWN\n");
    //         break;
    //     }
    // }
    for (l = 0; l < 2; l++) {
        for (i = 0; i < MINUTES_SIMULATED; i++){
            printf("Ticks with car on road %d for minute %2d: %d\n", l, i, minuteData[l][i].speedMeasurementCount);
        }
    }


    for (i = 0; i < AMOUNT_OF_ROADS; i++){
        for (k = 0; k < MINUTES_SIMULATED; k++){
            free(minuteData[i][k].speedOfCars);
            minuteData[i][k].speedOfCars = NULL;
        }
    }

    return 0;
}


void createCar(car* car, road* road, int* k, struct road roadArr[], struct roadPoints roadPointsArr[]){
    int SENTINAL = 1, i = 0, j;

    // do{
    //     car->currNode = rand() % 2;
    //     car->endGoal = rand() % 2;
    // }while(car->currNode == car->endGoal);
    // car->currGoal = car->endGoal;
    
    car->currNode = 0;
    car->endGoal = 2;

    pathfinding(car, roadArr, roadPointsArr);

    for (j = 0; j < 100; j++) {
        printf("path[%d] = %d\n", j, car->path[j]);
    }

    car->pathStep = 1;
    car->currGoal = car->path[car->pathStep];

    printf("currGoal: %d\n", car->currGoal);

    car->acceleration = ((double)(rand() % 7) + 31)/1000;
    printf("----> ACCELERATION: %lf <------\n", car->acceleration);
    car->speedDeviation = ((rand() % 10) + 1)/100;

    car->speed = 0;
    car->active = 1;
    car->ID = *k;

    if(car->currGoal > car->currNode){
        car->location = 0;
        car->dirBool = 1;
    }else{
        car->location = road->length;
        car->dirBool = 0;
    }



    while(SENTINAL){
        if(road->currCars[i] == -1){
            road->currCars[i] = *k;
            SENTINAL = 0;
        }
        i++;
    }
    *k += 1;
}

