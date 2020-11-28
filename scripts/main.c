#include "network.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_CARS 110

double disToEnd(car car, road road, struct car carArr[]);
void moveCar(car* car, struct car carArr[], road* road, struct road roadArr[], int carNum, int* debugBool);
double breakLength(car car);
void isCarInFront(car car, road road, struct car carArr[], double* carLocation, int* bool);
int cmpfunc (const void * a, const void * b);
double distanceBetweenCars(car car);
void roadOutput(car car[], road road);
double kmhTompds(road* road);
void changeRoad(car* car, road roadArr[], int* debugBool);


void createCar(car* car, road* road, int* k, struct road roadArr[], struct roadPoints roadPointsArr[]);


int main(void){
    int debugBool = 0;
    int i = 0;
    int l = 0;
    int active = 0;
    int k = 0;
    int j = 110;
    road road;
    //Her står der Struct fordi ellers virkede den ikk... Idk why
    struct road roadArr[100];
    roadPoints nodeArr[100];
    car car[1000];
    
    for(i = 0; i < 1000; i++){
        car[i].active = 0;
        car[i].pathStep = 0;
        for (l = 0; l < 100; l++) {
            car[i].path[l] = -1;
        }
    }

    for(i = 0; i < 1000; i++){
        for (l = 0; l < 3; l++) {
            car[i].path[l] = l;
        }
    }

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

        printf("Road[%d].startID: %d\n", i, roadArr[i].startID);
        printf("Road[%d].endID: %d\n", i, roadArr[i].endID);
    }

    

    srand(398);

    for (l = 0; l < 100; l++){
        for(i = 0; i < 100; i++){
            roadArr[l].currCars[i] = -1;
    }
    }

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



    while (debugBool == 0) {
        for(i = 0; i < 1; i++){
            for (l = 0; l < 100; l++) {
                // printf("\nRoad[%d].startID: %d\n", l, roadArr[l].startID);
                // printf("Road[%d].endID: %d\n", l, roadArr[l].endID);
                // printf("Road[%d].startID: %d\n", l + 1, roadArr[l + 1].startID);
                // printf("Road[%d].endID: %d\n", l + 1, roadArr[l + 1].endID);

                if ((roadArr[l].startID == car[i].currNode && roadArr[l].endID == car[i].currGoal) || (roadArr[l].endID == car[i].currNode && roadArr[l].startID == car[i].currGoal)) {
                    // printf("hihi xd\n");
                    moveCar(&car[i], car, &roadArr[l], roadArr, i, &debugBool);
                    printf("CarActive?: %d\n", car[i].active);
                    break;
                }
            }
            if(car[i].active == 1){
                printf("Location = %lf, Speed = %lf, ID = %d\n\n", car[i].location, car[i].speed, car[i].ID);
            }
        }
    }

    //     if(k > 100){
    //         printf("breakDOWN\n");
    //         break;
    //     }
    // }

    return 0;
}


void createCar(car* car, road* road, int* k, struct road roadArr[], struct roadPoints roadPointsArr[]){
    int SENTINAL = 1, i = 0;

    // do{
    //     car->currNode = rand() % 2;
    //     car->endGoal = rand() % 2;
    // }while(car->currNode == car->endGoal);
    // car->currGoal = car->endGoal;

    car->currNode = car->path[0];
    car->endGoal = car->path[2];

    car->currGoal = car->path[car->pathStep + 1];

    car->acceleration = 0.034;
    car->speedDeviation = 0;


    car->acceleration = ((double)(rand() % 7) + 31)/1000;
    printf("----> ACCELERATION: %lf <------", car->acceleration);
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

