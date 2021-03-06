#include "network.h"
#include <stdio.h>
#include <stdlib.h>

double disToEnd(car car, road road, struct car carArr[]);
void moveCar(car* car, struct car carArr[], road* road, struct road roadArr[], int carNum, int* debugBool, data *dp, int *index, int roadAmount, int *carsOnRoadCount);
double breakLength(car car);
void isCarInFront(car car, road road, struct car carArr[], double* carLocation, int* bool);
int cmpfunc (const void * a, const void * b);
double distanceBetweenCars(car car);
void roadOutput(car car[], road road);
double kmhTompds(road* road);
void changeRoad(car* car, road roadArr[], int* debugBool, int roadAmount);
roadPoints* getNodeAmount(int* nodeAmount);
road* getRoadAmount(int* roadAmount);
void getRestOfInput(road roadArr[], int* seed, int* simulationTime, int* carAmount);
void pathfinding(car* car, road roadArr[], struct roadPoints roadPointsArr[], int nodeAmount);
void changeLight(road roadArr[], roadPoints* node, int roadAmount);
void createCar(car* car, int* k, struct road roadArr[], struct roadPoints roadPointsArr[], int* endNodes, int endNodeAmount, int nodeAmount);

int main(void){
    int debugBool = 0;
    int nodeAmount = 0;
    int roadAmount = 0;
    int carAmount = 0;
    int currTick = 0;
    int endNodeAmount = 0;
    int intersectionAmount = 0;
    int seed;
    int simulationTime;
    int i = 0;
    int l = 0;
    int n = 0;
    int k = 0;
    int j = 110;
    int ticks = 0, minuteIndex = 0, roadIndex = 0, m, minutesSimulated;
    int *carsOnRoadCount;
    int *speedIndex;
    int spawnRate;
    road road;
    struct road *roadArr;
    roadPoints *nodeArr;
    int *endNodes;
    int *intersections;
    car *car;
    
    nodeArr = getNodeAmount(&nodeAmount);
    roadArr = getRoadAmount(&roadAmount);

    /* Resets variables */
    for(i = 0; i < roadAmount; i++){
        roadArr[i].startID = -1;
        roadArr[i].endID = -1;
        roadArr[i].intersecLightStart = 2;
        roadArr[i].intersecLightEnd = 2;

        for(l = 0; l < 1000; l++){
            roadArr[i].currCars[l] = -1;
        }
    }
    for (i = 0; i < nodeAmount; i++) {
        nodeArr[i].ID = -1;
    }

    getRestOfInput(roadArr, &seed, &simulationTime, &carAmount);
    car = (struct car*)malloc(carAmount * sizeof(struct car));

    /* Resets variables */
    for(i = 0; i < carAmount; i++){
        car[i].active = 0;
        car[i].pathStep = 0;
        for (l = 0; l < 100; l++) {
            car[i].path[l] = -1;
        }
    }    
    
    /*Allocates a 2D array for data collection dynamically*/
    carsOnRoadCount = allocateIntArray(roadAmount * 2);
    speedIndex = allocateIntArray(roadAmount * 2);
    minutesSimulated = simulationTime / (TICKS_PER_SECOND * SECONDS_PER_MINUTE);
    data **minuteData = allocateDataArray(roadAmount * 2, minutesSimulated);
    setUpDataArray(roadAmount * 2, minutesSimulated, carAmount, TICKS_PER_SECOND, minuteData, speedIndex, carsOnRoadCount, roadArr);

    srand(seed);


    for (i = 0; i < nodeAmount; i++) {
        nodeArr[i].ID = i;
    }
    /* Gives values to nodes(ID, connections, numOfConnections) */
    for (i = 0; i < roadAmount; i++) {
        nodeArr[roadArr[i].startID].connections[nodeArr[roadArr[i].startID].numOfConnections] = roadArr[i].endID;
        nodeArr[roadArr[i].startID].numOfConnections += 1;
        nodeArr[roadArr[i].endID].connections[nodeArr[roadArr[i].endID].numOfConnections] = roadArr[i].startID;
        nodeArr[roadArr[i].endID].numOfConnections += 1;
        nodeArr[roadArr[i].startID].ID = roadArr[i].startID;
        nodeArr[roadArr[i].endID].ID = roadArr[i].endID;
    }

    for (i = 0; i < nodeAmount; i++) {
        if (nodeArr[i].numOfConnections == 1) {
            endNodeAmount++;
        } else {
            intersectionAmount++;
        }
    }

    endNodes = (int*)malloc(endNodeAmount * sizeof(int));
    intersections = (int*)malloc(intersectionAmount * sizeof(int));

    l = 0;
    n = 0;

    /* Assigns endnodes */
    for (i = 0; i < nodeAmount; i++) {
        if (nodeArr[i].numOfConnections == 1) {
            endNodes[l] = nodeArr[i].ID;
            l++;
        } else {
            intersections[n] = nodeArr[i].ID;
            n++;
        }
    }

    spawnRate = (simulationTime / 1.25) / (carAmount/1.25);

    while (currTick < simulationTime) {

        /* Changes spawnrate of cars */
        if(currTick >= simulationTime/1.25){
            spawnRate = (simulationTime/1.50) / (carAmount/1.6);
        }else if(currTick >= simulationTime/1.75){
            spawnRate = (simulationTime/1.25) / (carAmount/1.15);
        }

        /* Spawns cars */
        j++;
        if (j >= spawnRate && k < carAmount) {
            createCar(&car[k], &k, roadArr, nodeArr, endNodes, endNodeAmount, nodeAmount);
            j = 0;
        }

        /* Changes light at intersections */
        for (i = 0; i < intersectionAmount; i++) {
            changeLight(roadArr, &nodeArr[intersections[i]], roadAmount);
        }

        /* runs movecar with correct parameters */
        for(i = 0; i < carAmount; i++){
            for (l = 0; l < roadAmount; l++) {
                if ((roadArr[l].startID == car[i].currNode && roadArr[l].endID == car[i].currGoal) || (roadArr[l].endID == car[i].currNode && roadArr[l].startID == car[i].currGoal)) {
                    roadIndex = car[i].dirBool == 1 ? l : l + roadAmount;
                    moveCar(&car[i], car, &roadArr[l], roadArr, i, &debugBool, &minuteData[roadIndex][minuteIndex], &speedIndex[roadIndex], roadAmount, &carsOnRoadCount[roadIndex]);
                    break;
                }
            }
        }
        currTick++;
        if (currTick % (SECONDS_PER_MINUTE * TICKS_PER_SECOND) == 0) {
            
            for (m = 0; m < 2 * roadAmount; m++){
                speedIndex[m] = 0;
                minuteData[m][minuteIndex].densityCarCount = carsOnRoadCount[m];
            }
            minuteIndex++;
        }

    }
    printf("CurrTick = %d\n", currTick);

    analyseData(roadAmount * 2, minutesSimulated, minuteData, seed);
    
    /* deallocates memory */
    freeSpeedArrays(roadAmount * 2, minutesSimulated, minuteData);
    free(minuteData);
    free(speedIndex);
    free(carsOnRoadCount);
    free(nodeArr);
    free(roadArr);
    free(endNodes);
    free(car);
    
    printf("Simulated ended!\n");
    return 0;
}


void createCar(car* car, int* k, struct road roadArr[], struct roadPoints roadPointsArr[], int* endNodes, int endNodeAmount, int nodeAmount){
    int SENTINAL = 1, i = 0, l = 0;

    /* Gives random start and endnode to car */
    do{
        car->currNode = endNodes[rand() % endNodeAmount];
        car->endGoal = endNodes[rand() % endNodeAmount];
    }while(car->currNode == car->endGoal);
    printf("pre pathfinding\n");
    pathfinding(car, roadArr, roadPointsArr, nodeAmount);
    printf("post pathfinding\n");

    car->pathStep = 1;
    car->currGoal = car->path[car->pathStep];

    /* Finds road car will start on */
    while (SENTINAL) {
        if ((roadArr[l].startID == car->currNode && roadArr[l].endID == car->currGoal) || (roadArr[l].endID == car->currNode && roadArr[l].startID == car->currGoal)) {
            SENTINAL = 0;
        } else {
            l++;
        }
    }

    SENTINAL = 1;

    car->acceleration = ((double)(rand() % 7) + 31)/1000;
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
    int ch, l = 0, lineRead = 0;
    char str[16];
    FILE *inputFile;
    roadPoints* nodeArr;

    inputFile = fopen("Input.txt", "r");

    if (inputFile != NULL) {
        while ((ch = fgetc(inputFile)) != EOF) {
            str[l] = ch;
            if ((str[l] == '\n') && (lineRead == 0)) {
                str[l] = '\0';
                *nodeAmount = atoi(str);
                nodeArr = (roadPoints*)malloc(*nodeAmount * sizeof(roadPoints));
                if (nodeArr == NULL) {
                    printf("Error! It no work D:\n");
                }
                lineRead = 1;
                break;
            }
            l++;
        }

    }
    fclose(inputFile);
    return nodeArr;
}

road* getRoadAmount(int* roadAmount) {
    int ch, l = 0, counter = 0, lineRead = 0;
    char str[16];
    FILE *inputFile;
    road *roadArr;

    inputFile = fopen("Input.txt", "r");

    if (inputFile != NULL) {
        while ((ch = fgetc(inputFile)) != EOF) {
            str[l] = ch;
            if ((str[l] == '\n') && (lineRead == 0)) {
                if (counter == 1) {
                    str[l] = '\0';
                    *roadAmount = atoi(str);
                    roadArr = (road*)malloc(*roadAmount * sizeof(road));
                    if (roadArr == NULL) {
                        printf("Error! It no work D:\n");
                    }
                    lineRead = 1;
                    break;
                }
                counter++;
                l = -1;
            }
            l++;
        }

    }

    fclose(inputFile);
    return roadArr;
}

void getRestOfInput(road roadArr[], int* seed, int* simulationTime, int* carAmount) {
    int ch, i = 0, l = 0, k = 0, counter = 0;
    char str[16];
    FILE *inputFile;

    inputFile = fopen("Input.txt", "r");

    if (inputFile != NULL) {
        while ((ch = fgetc(inputFile)) != EOF) {
            str[l] = ch;
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
                    *carAmount = atoi(str);
                    counter++;
                    l = -1;
                    break;

                case 3:
                    str[l] = '\0';
                    *seed = atoi(str);
                    counter++;
                    l = -1;
                    break;
            
                case 4:
                    str[l] = '\0';
                    *simulationTime = atoi(str)*600;
                    counter++;
                    l = -1;
                    break;

                default:
                    str[l] = '\0';
                    sscanf(str, "%d, %d, %lf, %lf", &roadArr[k].startID, &roadArr[k].endID, &roadArr[k].length, &roadArr[k].speedLimit);
                    roadArr[k].speedLimit = kmhTompds(&roadArr[k]);
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
            k++;
    }
    fclose(inputFile);
}