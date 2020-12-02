#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"

#define INFINITY 2147483647

//UwUbuntu

typedef struct car car;
typedef struct road road;
typedef struct roadPoints roadPoints;

struct roadPoints{
    int ID;

    int numOfConnections;
    int connections[4];

    /* pathfinding ting */
    int visited;
    struct roadPoints* parent;
    double local;

};

struct road{

    int startID;
    int endID;

    double length;
    double speedLimit;

    int maxCars;
    int currCars[100];

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
    int dirBool; /* 1 er positiv retning. 0 er negativ retning */
    int active;
    int ID;

    // skal være antal af nodes i guess
    int path[100];
    int pathStep;
};

double disToEnd(car car, road road, struct car carArr[]);
void moveCar(car* car, struct car carArr[], road* road, struct road roadArr[], int carNum, int* debugBool, data *dp, int index);
double breakLength(car car);
void isCarInFront(car car, road road, struct car carArr[], double* carLocation, int* bool);
int cmpfunc (const void * a, const void * b);
double distanceBetweenCars(car car);
void pushArray(car car, road *road);
void roadOutput(car car[], road road);
double kmhTompds(road* road);
void changeRoad(car* car, road roadArr[], int* debugBool);

/* lav om til: afstand til hvad end der er foran. om det er kryds eller anden bil */
double disToEnd(car car, road road, struct car carArr[]){
    int carInFront;
    double aheadLocation;
    isCarInFront(car, road, carArr, &aheadLocation, &carInFront);
    //printf("aheadLocation: %lf carInFront: %d\n", aheadLocation, carInFront);
    if(carInFront == 1){
        if(car.dirBool == 1){
            return (aheadLocation - (5 + distanceBetweenCars(car))) - car.location;
        }else{
            return car.location - (aheadLocation + (5 + distanceBetweenCars(car)));
        }
    }else{
        if(car.dirBool == 1){
            return road.length - car.location;
        }else{
            return road.length - (road.length - car.location);
        }
    }
}

/* afstand bilen bør holde til bilen foran baseret på hastighed */
double distanceBetweenCars(car car){
    double kmt = car.speed * 10;
    kmt *= 3.6;

    return (kmt/4) + 1;
}

void isCarInFront(car car, road road, struct car carArr[], double* carLocation, int* bool){

    int i = 0, k = 0, j;
    double locations[100];
    int temp, SENTINAL = 1;
    

    while(SENTINAL){
        temp = road.currCars[i];
        if (temp == -1){
            SENTINAL = 0;
        } else if (carArr[i].dirBool == car.dirBool){
            locations[k] = carArr[temp].location;
            k++;
            i++;
        }else{
            i++;
        }
    }

    qsort(locations, k, sizeof(double), cmpfunc);

    for(j = 0; j < k; j++){
        //printf("%lf ", locations[j]);
    }
    //printf("\n");
    if(car.dirBool == 1){
        for(j = 0; j < k-1; j++){
            if(car.location == locations[j]){
                *bool = 1;
                *carLocation = locations[j+1];
                break;
            }else{
                *bool = 0;
            }
        }
    }else{
        for(j = k; j > 0; j--){
            if(car.location == locations[j]){
                *bool = 1;
                *carLocation = locations[j-1];
                break;
            }else{
                *bool = 0;
            }
        }
    }
}

double breakLength(car car){
    double dist = 0;
    double i;
    
    for(i = car.speed; i > 0; i -= car.acceleration){
        dist += i;
    }

    return dist;
}

void moveCar(car* car, struct car carArr[], road* road, struct road roadArr[], int carNum, int* debugBool, data *dp, int index){ /*Tilføj parametre*/
    int l, startBuffer, endBuffer, lengthBuffer;

    if(car->active == 1){
        
        double distanceToEnd = disToEnd(*car, *road, carArr);
        car->breakLength = breakLength(*car);
        //printf("BreakLength: %lf\n", car->breakLength);

        /* Ændrer bilens fart */
        if(car->speed < (road->speedLimit + car->speedDeviation) && car->breakLength < distanceToEnd){
            car->speed += car->acceleration;
        }else if(car->breakLength >= distanceToEnd && car->speed > 0){
            car->speed -= car->acceleration;
            if(car->speed < 0){
                car->speed = 0;
            }
        }

        /* Ændrer bilens position */
        if(car->dirBool == 1){
            car->location += car->speed;
        }else{
            car->location -= car->speed;
        }
        /* Gem farten til databehandling
         * TODO: Lav et if statement så der ikke måles fart tæt på krydsene*/
        measureSpeed(car->speed, dp, index);

        /* Snapper bilen når den stopper */
        if(car->location >= road->length && car->dirBool == 1){
            car->speed = 0;
            car->active = 0;
            // printf("CurrentRoadStart: %d\n", road->startID);
            // printf("CurrentRoadEnd: %d\n", road->endID);
            //printf("Road[%d].startID: %d\n", car->path[car->pathStep], roadArr[car->path[car->pathStep]].startID);
            startBuffer = roadArr[car->path[car->pathStep]].startID;
            //printf("Road[%d].endID: %d\n", car->path[car->pathStep], roadArr[car->path[car->pathStep]].endID);
            endBuffer = roadArr[car->path[car->pathStep]].endID;
            //printf("Road[%d].length: %lf\n", car->path[car->pathStep],roadArr[car->path[car->pathStep]].length);
            lengthBuffer = roadArr[car->path[car->pathStep]].length;




            /* DETTE FUNKTIONS KALD VIRKER IKKE... JEG ANDER IKK HVORFOR SÅ JEG HAR NU KONSTATERET AT DET IKKE ER MIT PROBLEM LIGE NU LMAO!*/
            /*Det virker nu... Jeg gav mine roads currCar en værdi på -1 for at ressette den. Har INGEN ide om hvorfor det nogensinde ville fucke noget op...*/
            /*JEG STEMMER FOR AT PUSHARRAY SKAL DRÆBES!! DEN ER CURSED AF!!!!*/
            /* LAV EN FUCKING STRUCT BUFFER*/
            //pushArray(*car, road);

            roadArr[car->path[car->pathStep]].startID = startBuffer;
            //printf("Road[%d].startID: %d\n", car->path[car->pathStep], roadArr[car->path[car->pathStep]].startID);
            roadArr[car->path[car->pathStep]].endID = endBuffer;
            //printf("Road[%d].endID: %d\n", car->path[car->pathStep], roadArr[car->path[car->pathStep]].endID);
            roadArr[car->path[car->pathStep]].length = lengthBuffer;
            //printf("Road[%d].length: %lf\n", car->path[car->pathStep],roadArr[car->path[car->pathStep]].length);
            // printf("CurrentRoadStart: %d\n", road->startID);
            // printf("CurrentRoadEnd: %d\n", road->endID);
            
            car->location = road->length; /* endpoint om det er bag en anden bil eller i et kryds */

            car->currNode = road->endID;
            car->pathStep++;

            //printf("Car stopped %d\n", carNum);

            changeRoad(car, roadArr, debugBool);
        }else if(car->location <= 0 && car->dirBool == 0){
            car->speed = 0;
            car->active = 0;
            //pushArray(*car, road);
            car->location = 0;

            car->currNode = road->startID;
            car->pathStep++;

            //printf("Car stopped %d\n", carNum);

            changeRoad(car, roadArr, debugBool);
        }
    }
}

int cmpfunc (const void * a, const void * b){
    return ( *(double*)a - *(double*)b );
}

void pushArray(car car, road *road){
    int i = 0, SENTINAL = 1;

    while(SENTINAL){
        if(road->currCars[i] == car.ID){
            SENTINAL = 0;
        }else{
            i++;
        }
    }

    SENTINAL = 1;

    while(SENTINAL){
        road->currCars[i] = road->currCars[i + 1];
        if (road->currCars[i + 1] == -1){
            SENTINAL = 0;
        }
        i++;
    }

    /*DEBUG PRINTER ARRAYET XD*/
    for (i = 0; i < 100; i++) {
        //printf("currCar[%d]: %d\n", i, road->currCars[i]);
    }
}

double kmhTompds(road* road){
    return road->speedLimit = (road->speedLimit/3.6)/10;
}

void pathfinding(car* car, road roadArr[], struct roadPoints roadPointsArr[]){
    int i, j, k, elements = 1, SENTINAL = 1;
    int notTested[100];
    int currentNode;
    int tempArray[100];
    double distance;
    for(i = 0; i < 100; i++){
        notTested[i] = -1;
    }

    notTested[0] = car->currNode;

    //setup
    //100 skal være antal nodes
    for(i = 0; i < 100; i++){
        if(i != notTested[0]){
            roadPointsArr[i].local = INFINITY;
            roadPointsArr[i].visited = 0;
            roadPointsArr[i].parent = NULL;
        }else{
            roadPointsArr[i].local = 0;
            roadPointsArr[i].visited = 1;
            roadPointsArr[i].parent = NULL;
        }
    }

    while(notTested[0] != -1){
        distance = 0;

        if(notTested[0] != -1){
            currentNode = notTested[0];
            elements--;

            i = 0;
            SENTINAL = 1;
            while(SENTINAL){
                notTested[i] = notTested[i + 1];
                if(notTested[i + 1] == -1){
                    SENTINAL = 0;
                }
                i++;
            }
        }

        for(i = 0; i < roadPointsArr[currentNode].numOfConnections; i++){
            if(roadPointsArr[currentNode].local < roadPointsArr[roadPointsArr[currentNode].connections[i]].local){
                notTested[elements] = roadPointsArr[roadPointsArr[currentNode].connections[i]].ID;
                elements++;
                roadPointsArr[roadPointsArr[currentNode].connections[i]].parent = &roadPointsArr[currentNode];

                j = 0;
                while(distance == 0){
                    if((roadArr[j].startID == currentNode && roadArr[j].endID == roadPointsArr[currentNode].connections[i]) || (roadArr[j].startID == roadPointsArr[currentNode].connections[i] && roadArr[j].endID == currentNode)){
                        distance = roadArr[j].length;
                    }
                    j++;
                }

                roadPointsArr[roadPointsArr[currentNode].connections[i]].local = roadPointsArr[currentNode].local + distance;
            }
        }
    }

    if(roadPointsArr[car->endGoal].parent == NULL){
        //printf("No path for %d\n", car->ID);
    }else{
        currentNode = car->endGoal;
        i = 0;
        while(currentNode != car->currNode){
            tempArray[i] = currentNode;

            currentNode = roadPointsArr[currentNode].parent->ID;

            i++;
        }

        // for (j = 0; j < 100; j++) {
        //     printf("tempArray[%d] = %d\n", j, tempArray[j]);
        // }

        tempArray[i] = currentNode;
        k = 0;
        for(j = i; j >= 0; j--){
            car->path[k] = tempArray[j];
            k++;
        }
    }
}

void roadOutput(car car[], road road){




}

void changeRoad(car* car, road roadArr[], int* debugBool){
    int l;
    double roadLength;
    road road;

    if (car->path[car->pathStep] != -1){
        car->active = 1;
        // car->currNode = car->currGoal;
        car->currGoal = car->path[car->pathStep];
        for (l = 0; l < 100; l++) {
            if ((roadArr[l].startID == car->currNode && roadArr[l].endID == car->currGoal) || (roadArr[l].endID == car->currNode && roadArr[l].startID == car->currGoal)) {
                road = roadArr[l];
                //printf("l: %d\n", l);
                break;
            }
        }
        
        if(car->currGoal > car->currNode){
            car->location = 0;
            car->dirBool = 1;
        }else{
            car->location = road.length;
            car->dirBool = 0;
        }

    }else{
        //printf("Endgoal Reached!\n");
        *debugBool = 1;
    }
    /*
    printf("road.lenght: %lf\n", road.length);
    printf("road.startID: %d\n", road.startID);
    printf("road.endID: %d\n", road.endID);
*/
}