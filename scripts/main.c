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
void getInput(road roadArr[], int* nodeAmount, int* roadAmount, int* seed, int* simulationTime);


void createCar(car* car, road* road, int* k, struct road roadArr[], struct roadPoints roadPointsArr[]);


int main(void){
    int debugBool = 0;
    int nodeAmount = 0;
    int roadAmount = 0;
    int seed;
    int simulationTime;
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

    for (l = 0; l < 100; l++){
        for(i = 0; i < 100; i++){
            roadArr[l].currCars[i] = -1;
        }
    }

    getInput(roadArr, &nodeAmount, &roadAmount, &seed, &simulationTime);

    /* WAS USED TO DEBUG THE INPUT FUNCTION */    
    // printf("nodeAmount: %d\n", nodeAmount);
    // printf("roadAmount: %d\n", roadAmount);
    // printf("seed: %d\n", seed);
    // printf("simulationTime: %d\n", simulationTime);

    
    // for (i = 0; i < 100; i++) {
    //     printf("roadArr[%d].startID: %d\n", i, roadArr[i].startID);
    //     printf("roadArr[%d].endID: %d\n", i, roadArr[i].endID);
    //     printf("roadArr[%d].lenght: %lf\n", i, roadArr[i].length);
    //     printf("roadArr[%d].speedLimit: %lf\n\n", i, roadArr[i].speedLimit);

    // }

    for(i = 0; i < 4; i++){
        nodeArr[i].ID = i;
    }
    for(i = 0; i < 3; i++){
        roadArr[i].length = (i + 1) * 1000;
        roadArr[i].speedLimit = (i + 1) * 50;
        roadArr[i].speedLimit = kmhTompds(&roadArr[i]);
        // roadArr[i].startID = i;
        // roadArr[i].endID = i + 1;

        printf("Road[%d].startID: %d\n", i, roadArr[i].startID);
        printf("Road[%d].endID: %d\n", i, roadArr[i].endID);
        printf("Road[%d).length: %lf\n", i, roadArr[i].length);
    }

    srand(seed);

    nodeArr[0].connections[0] = 1;
    nodeArr[0].numOfConnections = 1;

    nodeArr[1].connections[0] = 0;
    nodeArr[1].connections[1] = 2;
    nodeArr[1].connections[2] = 3;
    nodeArr[1].numOfConnections = 3;

    nodeArr[2].connections[0] = 1;
    nodeArr[2].numOfConnections = 1;

    nodeArr[3].connections[0] = 1;
    nodeArr[3].numOfConnections = 1;

    roadArr[0].startID = 0;
    roadArr[0].endID = 1;

    roadArr[1].startID = 1;
    roadArr[1].endID = 2;

    roadArr[2].startID = 1;
    roadArr[2].endID = 3;

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
    int SENTINAL = 1, i = 0, j;

    // do{
    //     car->currNode = rand() % 2;
    //     car->endGoal = rand() % 2;
    // }while(car->currNode == car->endGoal);
    // car->currGoal = car->endGoal;
    
    car->currNode = 0;
    car->endGoal = 3;

    pathfinding(car, roadArr, roadPointsArr);

    for (j = 0; j < 100; j++) {
        printf("path[%d] = %d\n", j, car->path[j]);
    }

    car->pathStep = 1;
    car->currGoal = car->path[car->pathStep];

    printf("currGoal: %d\n", car->currGoal);

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

/* This funktion read the file called "Input.txt", and configures the roads in roadArr */
void getInput(road roadArr[], int* nodeAmount, int* roadAmount, int* seed, int* simulationTime) {
    int ch, i = 0, l = 0, k = 0, counter = 0;
    char str[16];
    FILE *inputFile;

    inputFile = fopen("Input.txt", "r");

    if (inputFile != NULL) {
        while ((ch = fgetc(inputFile)) != EOF) {
            str[l] = ch;
            // printf("strBuff: %c\n", strBuff[i]);
            // printf("str: %c\n", str[l]);
            if (str[l] == '\n') {
                switch (counter){
                case 0:
                    // for (i = 0; i < 16; i++) {
                    //     printf("str[%d]: %c\n", i, str[i]);
                    // }
                    str[l] = '\0';
                    *nodeAmount = atoi(str);
                    // printf("nodeAmount: %d\n", *nodeAmount);
                    counter++;
                    l = -1;
                    break;
                
                case 1:
                    str[l] = '\0';
                    *roadAmount = atoi(str);
                    counter++;
                    l = -1;
                    break;

                case 2:
                    str[l] = '\0';
                    *seed = atoi(str);
                    counter++;
                    l = -1;
                    break;
            
                case 3:
                    str[l] = '\0';
                    *simulationTime = atoi(str);
                    counter++;
                    l = -1;
                    break;

                default:
                    /*Skriv logic for indlæsning af data til de individuelle veje*/
                    str[l] = '\0';
                    sscanf(str, "%d, %d, %lf, %lf", &roadArr[k].startID, &roadArr[k].endID, &roadArr[k].length, &roadArr[k].speedLimit);
                    roadArr[k].speedLimit = kmhTompds(&roadArr[k]);
                    // printf("startID: %d\nendId: %d\nlength: %lf\nspeedLimit: %lf\n", roadArr[k].startID, roadArr[k].endID, roadArr[k].length, roadArr[k].speedLimit);
                    k++;
                    l = -1;
                    break;
                }
            }

            i++;
            l++;

        }
            str[l] = '\0';
            sscanf(str, "%d, %d, %lf, %lf", &roadArr[k].startID, &roadArr[k].endID, &roadArr[k].length, &roadArr[k].speedLimit);
            roadArr[k].speedLimit = kmhTompds(&roadArr[k]);
            // printf("startID: %d\nendId: %d\nlength: %lf\nspeedLimit: %lf\n", roadArr[k].startID, roadArr[k].endID, roadArr[k].length, roadArr[k].speedLimit);
            k++;
    }

}