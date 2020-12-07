#include "network.h"
#include <stdio.h>
#include <stdlib.h>

double disToEnd(car car, road road, struct car carArr[]);
void moveCar(car* car, struct car carArr[], road* road, struct road roadArr[], int carNum, int* debugBool, data *dp, int index, int roadAmount, int *carsOnRoadCount);
double breakLength(car car);
void isCarInFront(car car, road road, struct car carArr[], double* carLocation, int* bool);
int cmpfunc (const void * a, const void * b);
double distanceBetweenCars(car car);
void roadOutput(car car[], road road);
double kmhTompds(road* road);
void changeRoad(car* car, road roadArr[], int* debugBool, int roadAmount);
roadPoints* getNodeAmount(int* nodeAmount);
road* getRoadAmount(int* roadAmount);
void getRestOfInput(road roadArr[], int* seed, int* simulationTime);
void pathfinding(car* car, road roadArr[], struct roadPoints roadPointsArr[], int nodeAmount);


void createCar(car* car, int* k, struct road roadArr[], struct roadPoints roadPointsArr[], int* endNodes, int endNodeAmount, int nodeAmount);


int main(void){
    int debugBool = 0;
    int nodeAmount = 0;
    int roadAmount = 0;
    int currTick = 0;
    int endNodeAmount = 0;
    int seed;
    int simulationTime;
    int i = 0;
    int l = 0;
    int active = 0;
    int k = 0;
    int j = 110;
    int ticks = 0, minuteIndex = 0, roadIndex = 0, m;
    int carsOnRoadCount[AMOUNT_OF_ROADS * 2];
    int speedIndex[AMOUNT_OF_ROADS * 2];
    road road;
    //Her står der Struct fordi ellers virkede den ikk... Idk why
    struct road *roadArr;
    roadPoints *nodeArr;
    int *endNodes;
    car car[1000];

    /*Allocates a 2D array for data collection statically*/
    data minuteData[AMOUNT_OF_ROADS * 2][MINUTES_SIMULATED];
    setUpDataArray(AMOUNT_OF_ROADS * 2, MINUTES_SIMULATED, minuteData, speedIndex, carsOnRoadCount);

    nodeArr = getNodeAmount(&nodeAmount);
    roadArr = getRoadAmount(&roadAmount);

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

    for(i = 0; i < roadAmount; i++){
        roadArr[i].startID = -1;
        roadArr[i].endID = -1;

        for(l = 0; l < 100; l++){
            roadArr[i].currCars[l] = -1;
        }
    }
    for (i = 0; i < nodeAmount; i++) {
        nodeArr[i].ID = -1;
    }

    getRestOfInput(roadArr, &seed, &simulationTime);

    /* WAS USED TO DEBUG THE INPUT FUNCTION */    
    printf("nodeAmount: %d\n", nodeAmount);
    printf("roadAmount: %d\n", roadAmount);
    printf("seed: %d\n", seed);
    printf("simulationTime: %d\n", simulationTime);

    
    for (i = 0; i < roadAmount; i++) {
        printf("roadArr[%d].startID: %d\n", i, roadArr[i].startID);
        printf("roadArr[%d].endID: %d\n", i, roadArr[i].endID);
        printf("roadArr[%d].lenght: %lf\n", i, roadArr[i].length);
        printf("roadArr[%d].speedLimit: %lf\n\n", i, roadArr[i].speedLimit);
    }

    // for(i = 0; i < 4; i++){
    //     nodeArr[i].ID = i;
    // }
    // for(i = 0; i < 3; i++){
    //     roadArr[i].length = (i + 1) * 1000;
    //     roadArr[i].speedLimit = (i + 1) * 50;
    //     roadArr[i].speedLimit = kmhTompds(&roadArr[i]);
    //     // roadArr[i].startID = i;
    //     // roadArr[i].endID = i + 1;

    //     //printf("Road[%d].startID: %d\n", i, roadArr[i].startID);
    //     //printf("Road[%d].endID: %d\n", i, roadArr[i].endID);
    //     //printf("Road[%d).length: %lf\n", i, roadArr[i].length);
    // }

    srand(seed);

    for (i = 0; i < nodeAmount; i++) {
        nodeArr[i].ID = i;
    }

    for (i = 0; i < roadAmount; i++) {
        nodeArr[roadArr[i].startID].connections[nodeArr[roadArr[i].startID].numOfConnections] = roadArr[i].endID;
        nodeArr[roadArr[i].startID].numOfConnections += 1;
        nodeArr[roadArr[i].endID].connections[nodeArr[roadArr[i].endID].numOfConnections] = roadArr[i].startID;
        nodeArr[roadArr[i].endID].numOfConnections += 1;
        nodeArr[roadArr[i].startID].ID = roadArr[i].startID;
    }

    for (i = 0; i < nodeAmount; i++) {
        if (nodeArr[i].numOfConnections == 1) {
            endNodeAmount++;
        }
    }

    endNodes = (int*)malloc(endNodeAmount * sizeof(int));

    l = 0;

    for (i = 0; i < nodeAmount; i++) {
        if (nodeArr[i].numOfConnections == 1) {
            endNodes[l] = nodeArr[i].ID;
            //printf("xd[%d]: %d\n", l, endNodes[l]);
            l++;
        }
    }

    /*for (i = 0; i < endNodeAmount; i++) {
        printf("endNode[%d]: %d\n", i, endNodes[i]);
    }*/

    // for (i = 0; i < nodeAmount; i++) {
    //     for (l = 0; l < nodeArr[i].numOfConnections; l++) {
    //         printf("nodeArr[%d].connection[%d]: %d\n", i, l, nodeArr[i].connections[l]);
    //     }
    //     printf("\n");
    // }

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


    // printf("speedIndex = %d, minuteIndex = %d\n", speedIndex, minuteIndex);

    while (currTick < simulationTime) {
        //currTick++;
        j++;
        if (j >= 120 && k < AMOUNT_OF_CARS) {
            //Random midlertidigt fix xd
            // car[k].currNode = car[k].path[0];
            // car[k].endGoal = car[k].path[2];
            // car[k].currGoal = car[k].path[car[k].pathStep + 1];
            //printf("pre car\n");
            createCar(&car[k], &k, roadArr, nodeArr, endNodes, endNodeAmount, nodeAmount);
            //printf("post car\n");
            // for (l = 0; l < 100; l++) {
            //     if ((roadArr[l].startID == car[k].currNode && roadArr[l].endID == car[k].currGoal) || (roadArr[l].endID == car[k].currNode && roadArr[l].startID == car[k].currGoal)) {
            //         createCar(&car[k], &k, roadArr, nodeArr);
            //         // printf("Car[%d].currNode: %d\n", i, car[i].currNode);
            //         // printf("Car[%d].currGoal: %d\n", i, car[i].currGoal);
            //         break;
            //         }
            //     }
            j = 0;
        }

        for(i = 0; i < AMOUNT_OF_CARS; i++){
            // printf("car[%d].currNode: %d\n", i, car[i].currNode);
            // printf("car[%d].currGoal: %d\n", i, car[i].currGoal);
            for (l = 0; l < roadAmount; l++) {
            //printf("CurrCar[0]: %d on road[%d]\n", roadArr[l].currCars[0], l); 
            // printf("Road[%d].startID: %d\n", l, roadArr[l].startID);
            // printf("Road[%d].endID: %d\n", l, roadArr[l].endID);
                if ((roadArr[l].startID == car[i].currNode && roadArr[l].endID == car[i].currGoal) || (roadArr[l].endID == car[i].currNode && roadArr[l].startID == car[i].currGoal)) {
                    //printf("car[%d].id: %d\n", i, car[i].ID);
                    roadIndex = car[i].dirBool == 1 ? l : l + AMOUNT_OF_ROADS;
                    moveCar(&car[i], car, &roadArr[l], roadArr, i, &debugBool, &minuteData[roadIndex][minuteIndex], speedIndex[roadIndex], roadAmount, &carsOnRoadCount[roadIndex]);
                    minuteData[roadIndex][minuteIndex].roadID = l;
                    minuteData[roadIndex][minuteIndex].roadLength = roadArr[l].length;
                    speedIndex[roadIndex]++;
                    if (currTick % (SECONDS_PER_MINUTE * TICKS_PER_SECOND) == 599) {
                    
                    }
                    // printf("CarActive?: %d\n", car[i].active);
                    break;
                }
            }
            /*if(car[i].active == 1){
                printf("Location = %lf, Speed = %lf, ID = %d\n\n", car[i].location, car[i].speed, car[i].ID);
            }*/
        }
        currTick++;
        if (currTick % (SECONDS_PER_MINUTE * TICKS_PER_SECOND) == 0) {
            
            for (m = 0; m < 2 * AMOUNT_OF_ROADS; m++){
                speedIndex[m] = 0;
                minuteData[m][minuteIndex].densityCarCount = carsOnRoadCount[m];
            }
            minuteIndex++;
            // printf("speedIndex = %d, minuteIndex = %d\n", speedIndex, minuteIndex);
        }

    }

    //     if(k > 100){
    //         printf("breakDOWN\n");
    //         break;
    //     }
    // }
    analyseData(AMOUNT_OF_ROADS * 2, MINUTES_SIMULATED, minuteData);
    printAnalysedData(AMOUNT_OF_ROADS * 2, MINUTES_SIMULATED, minuteData);
    freeSpeedArrays(AMOUNT_OF_ROADS * 2, MINUTES_SIMULATED, minuteData);

    free(nodeArr);
    free(roadArr);
    free(endNodes);

    printf("pre return xd\n");
    return 0;
}


void createCar(car* car, int* k, struct road roadArr[], struct roadPoints roadPointsArr[], int* endNodes, int endNodeAmount, int nodeAmount){
    int SENTINAL = 1, i = 0, j, l = 0;

    do{
        car->currNode = endNodes[rand() % endNodeAmount];
        car->endGoal = endNodes[rand() % endNodeAmount];
    }while(car->currNode == car->endGoal);
    

    // if (*k == 0) {
    //     car->currNode = 2;
    //     car->endGoal = 0;        
    // } else {
    //     car->currNode = 3;
    //     car->endGoal = 0;
    // }
    /* MIDLERTIDIG MEME */
    // car->currNode = 0;
    // car->endGoal = 3;

    //printf("pre pathfinding\n");
    pathfinding(car, roadArr, roadPointsArr, nodeAmount);
    //printf("post pathfinding\n");


    /*for (j = 0; j < 100; j++) {
        printf("path[%d] = %d\n", j, car->path[j]);
    }*/

    car->pathStep = 1;
    car->currGoal = car->path[car->pathStep];

    while (SENTINAL) {
        if ((roadArr[l].startID == car->currNode && roadArr[l].endID == car->currGoal) || (roadArr[l].endID == car->currNode && roadArr[l].startID == car->currGoal)) {
            SENTINAL = 0;
        } else {
            l++;
        }
    }

    SENTINAL = 1;

    //printf("currGoal: %d\n", car->currGoal);

    car->acceleration = ((double)(rand() % 7) + 31)/1000;
    //printf("----> ACCELERATION: %lf <------\n", car->acceleration);
    car->speedDeviation = ((rand() % 10) + 1)/100;

    car->speed = 0;
    car->active = 1;
    car->ID = *k;

    if(car->currGoal > car->currNode){
        car->location = 0;
        car->dirBool = 1;
    }else{
        car->location = roadArr[l].length;
        car->dirBool = 0;
    }

    while(SENTINAL){
        if(roadArr[l].currCars[i] == -1){
            roadArr[l].currCars[i] = *k;
            SENTINAL = 0;
        }
        i++;
    }
    *k += 1;
}

/* This funktion read the file called "Input.txt", and configures the roads in roadArr */
roadPoints* getNodeAmount(int* nodeAmount) {
    int ch, i = 0, l = 0, k = 0, debugBool = 0, xd;
    char str[16];
    FILE *inputFile;
    roadPoints* nodeArr;

    inputFile = fopen("Input.txt", "r");

    if (inputFile != NULL) {
        while ((ch = fgetc(inputFile)) != EOF) {
            str[l] = ch;
            // printf("strBuff: %c\n", strBuff[i]);
            // printf("str: %c\n", str[l]);
            if ((str[l] == '\n') && (debugBool == 0)) {
                // for (i = 0; i < 16; i++) {
                //     printf("str[%d]: %d\n", i, str[i]);
                // }
                str[l] = '\0';
                *nodeAmount = atoi(str);
                printf("amount: %d\n", *nodeAmount);
                nodeArr = (roadPoints*)malloc(*nodeAmount * sizeof(roadPoints));
                if (nodeArr == NULL) {
                    printf("Error! It no work D:\n");
                }
                debugBool = 1;
                // printf("nodeAmount: %d\n", *nodeAmount);
                break;
            }
            l++;
        }

    }
    // printf("inputFile adresse: %p\n", inputFile);
    fclose(inputFile);
    // printf("xd: %d\n", xd);
    // printf("inputFile adresse: %p\n", inputFile);
    return nodeArr;
}

road* getRoadAmount(int* roadAmount) {
    int ch, i = 0, l = 0, k = 0, counter = 0;
    char str[16];
    FILE *inputFile;
    road *roadArr;

    inputFile = fopen("Input.txt", "r");

    if (inputFile != NULL) {
        while ((ch = fgetc(inputFile)) != EOF) {
            str[l] = ch;
            // printf("strBuff: %c\n", strBuff[i]);
            // printf("str: %c\n", str[l]);
            if (str[l] == '\n') {
                if (counter == 1) {
                    // for (i = 0; i < 16; i++) {
                    //     printf("str[%d]: %c\n", i, str[i]);
                    // }
                    str[l] = '\0';
                    *roadAmount = atoi(str);
                    roadArr = (road*)malloc(*roadAmount * sizeof(road));
                    if (roadArr == NULL) {
                        printf("Error! It no work D:\n");
                    }
                    // printf("nodeAmount: %d\n", *nodeAmount);
                    return roadArr;
                    break;
                }
                counter++;
                l = -1;
            }
            l++;
        }

    }

    fclose(inputFile);
}

void getRestOfInput(road roadArr[], int* seed, int* simulationTime) {
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
                    counter++;
                    l = -1;
                    break;
                
                case 1:
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
                    *simulationTime = atoi(str)*600;
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
    fclose(inputFile);

}