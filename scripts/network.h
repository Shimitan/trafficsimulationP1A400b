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
typedef struct road road;
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

/* Finds distance to whatever is in front of the car, either being a node or another car */
/* DO NOT FUCK WITH THE PRINTS IN THE IF-CHAIN AT THE BOTTEM... LITTERALLY DOES NOT WORK WITHOUT THEM! */
double disToEnd(car car, road road, struct car carArr[]){
    int carInFront;
    double aheadLocation;
    isCarInFront(car, road, carArr, &aheadLocation, &carInFront);
    if(carInFront == 1){
        if(car.dirBool == 1){
            return (aheadLocation - (CAR_LENGTH + distanceBetweenCars(car))) - car.location;
        }else{
            return car.location - (aheadLocation + (CAR_LENGTH + distanceBetweenCars(car)));
        }
    }else{
        if(car.dirBool == 1){
            if(road.intersecLightEnd == 2) {
                printf("LightEnd: %d\n", road.intersecLightEnd);
                return road.length - car.location;
            } else {
                printf("LightEnd: %d\n", road.intersecLightEnd);
                return road.length - car.location - 2;
            }
        }else{
            if(road.intersecLightStart == 2) {
                printf("LightStart: %d\n", road.intersecLightStart);
                return road.length - (road.length - car.location);
            } else {
                printf("LightStart: %d\n", road.intersecLightStart);
                return road.length - (road.length - car.location) - 5;
            }
        }
    }
}

/* distance the car should keep to the car in front based on current speed */
double distanceBetweenCars(car car){
    double kmt = car.speed * 10;
    kmt *= 3.6;

    return (kmt/4) + 1;
}

/* Finds out wether there is a car in front of the current car */
void isCarInFront(car car, road road, struct car carArr[], double* carLocation, int* bool){

    int i = 0, k = 0, j;
    double locations[100];
    int temp, SENTINAL = 1;
    
    /* Makes array of locations of cars on the road */
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

    /* finds if there is a car in front */
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

/* Finds the breaking distance of the current car */
double breakLength(car car){
    double dist = 0;
    double i;
    
    for(i = car.speed; i > 0; i -= car.acceleration){
        dist += i;
    }

    return dist;
}

void moveCar(car* car, struct car carArr[], road* road, struct road roadArr[], int carNum, int* debugBool, data *dp, int *index, int roadAmount, int *carsOnRoadCount){ /*Tilføj parametre*/
    double distanceToEnd;
    if(car->active == 1){
        if ((car->location == 0 && car->dirBool == 1) || (car->location == road->length && car->dirBool == 0)){
            *carsOnRoadCount += 1;
        }
        
        distanceToEnd = disToEnd(*car, *road, carArr);
        car->breakLength = breakLength(*car);

        /* Accelerates or deccelerates the car */
        if(car->speed < (road->speedLimit + car->speedDeviation) && car->breakLength < distanceToEnd){
            car->speed += car->acceleration;
        }else if(car->breakLength >= distanceToEnd && car->speed > 0){
            car->speed -= car->acceleration;
            if(car->speed < 0){
                car->speed = 0;
            }
        }

        /* Moves the car */
        if(car->dirBool == 1){
            car->location += car->speed;
        }else{
            car->location -= car->speed;
        }

        /* Gem farten til databehandling */
        if (car->dirBool == 1 && (road->length - car->location) > (findAccelerationLength(car, road) + 5 * (CAR_LENGTH + MIN_SPACING)) && car->location > findAccelerationLength(car, road)){
            measureSpeed(car->speed, dp, *index);
            *index += 1;
        } else if (car->dirBool == 0 && (road->length - car->location) > findAccelerationLength(car, road) && car->location > (findAccelerationLength(car, road) + 5 * (CAR_LENGTH + MIN_SPACING))){
            measureSpeed(car->speed, dp, *index);
            *index += 1;
        }
        
        /* Gem antal biler der har passeret midtpunktet af vejen */
        if (car->dirBool == 1 && car->location >= (road->length/2) && (car->location - car->speed) < (road->length/2)){
            countCarFlow(dp);
        } else if (car->dirBool == 0 && car->location <= (road->length/2) && (car->location + car->speed) > (road->length/2)) {
            countCarFlow(dp);
        }


        /* stops the car and changes road if needed */
        if(car->location >= road->length && car->dirBool == 1){
            car->speed = 0;
            car->active = 0;
            *carsOnRoadCount -= 1;

            /* DETTE FUNKTIONS KALD VIRKER IKKE... JEG ANDER IKK HVORFOR SÅ JEG HAR NU KONSTATERET AT DET IKKE ER MIT PROBLEM LIGE NU LMAO!*/
            /*Det virker nu... Jeg gav mine roads currCar en værdi på -1 for at ressette den. Har INGEN ide om hvorfor det nogensinde ville fucke noget op...*/
            /*JEG STEMMER FOR AT PUSHARRAY SKAL DRÆBES!! DEN ER CURSED AF!!!!*/
            /* LAV EN FUCKING STRUCT BUFFER*/
            /* Ladies and gentlebobs! It works! Who knew, that the problem was createCar... */
            /* Den virker ikk igen...................... */
            
            pushArray(*car, road);
            
            car->location = road->length; /* endpoint om det er bag en anden bil eller i et kryds */

            car->currNode = road->endID;
            car->pathStep++;

            changeRoad(car, roadArr, debugBool, roadAmount);

        }else if(car->location <= 0 && car->dirBool == 0){
            car->speed = 0;
            car->active = 0;
            *carsOnRoadCount -= 1;
            pushArray(*car, road);

            car->location = 0;

            car->currNode = road->startID;
            car->pathStep++;

            changeRoad(car, roadArr, debugBool, roadAmount);
        }
    }
}

/* Ugly physics calculation */
double findAccelerationLength(car *car, road *road){
    double distance;
    distance = (double) 0.5 * (road->speedLimit * road->speedLimit)/car->acceleration;
    return distance;
}


/* compare function for qsort */
int cmpfunc (const void * a, const void * b){
    return ( *(double*)a - *(double*)b );
}

/* pushes the array of cars on the road to avoid gaps */
/* den her hader jeg stadig væk */
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
}

/* converts input to the correct unit */
double kmhTompds(road* road){
    return road->speedLimit = (road->speedLimit/3.6)/10;
}

/* Pathfinding for the car */
void pathfinding(car* car, road roadArr[], struct roadPoints roadPointsArr[], int nodeAmount){
    int i, j, k, elements = 1, SENTINAL = 1;
    /* Not tested holds the ID of nodes to be tested */
    int notTested[100];
    int currentNode;
    int tempArray[100];
    double distance;
    for(i = 0; i < 100; i++){
        notTested[i] = -1;
    }

    notTested[0] = car->currNode;

    /* setup */
    /* nu fixed */
    for(i = 0; i < nodeAmount; i++){
        if(i != notTested[0]){
            roadPointsArr[i].local = INFINITY;
            roadPointsArr[i].parent = NULL;
        }else{
            roadPointsArr[i].local = 0;
            roadPointsArr[i].parent = NULL;
        }
    }

    /* While loop stops when array is empty */
    while(notTested[0] != -1){
        distance = 0;

        /* Removes node currently being tested from the array */
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
        
                
        for(i = 0; i < roadPointsArr[currentNode].numOfConnections; i++){

            /* finds distance from current node to neigbour node */
            j = 0;
            while(distance == 0){
                if((roadArr[j].startID == currentNode && roadArr[j].endID == roadPointsArr[currentNode].connections[i]) || (roadArr[j].startID == roadPointsArr[currentNode].connections[i] && roadArr[j].endID == currentNode)){
                    distance = roadArr[j].length;
                }
                j++;
            }   
            /* Enters the if-statement if there is a shorter route to it than alrady found, and then adds it to the array of nodes to be tested */
            if(roadPointsArr[currentNode].local + distance < roadPointsArr[roadPointsArr[currentNode].connections[i]].local){
                notTested[elements] = roadPointsArr[roadPointsArr[currentNode].connections[i]].ID;
                elements++;
                roadPointsArr[roadPointsArr[currentNode].connections[i]].parent = &roadPointsArr[currentNode];

                roadPointsArr[roadPointsArr[currentNode].connections[i]].local = roadPointsArr[currentNode].local + distance;
            }
        }
    }

    if(roadPointsArr[car->endGoal].parent == NULL){
        printf("No path for %d\n", car->ID);
    }else{
        currentNode = car->endGoal;
        i = 0;
        /* makes array for the path in opposite direction */
        while(currentNode != car->currNode){
            tempArray[i] = currentNode;

            currentNode = roadPointsArr[currentNode].parent->ID;

            i++;
        }

        /* flips the array */
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

/* facilitates the change in road when car reaches a node */
void changeRoad(car* car, road roadArr[], int* debugBool, int roadAmount){
    int l, i;
    road road;

    if (car->path[car->pathStep] != -1){
        car->active = 1;
        car->currGoal = car->path[car->pathStep];
        for (l = 0; l < roadAmount; l++) {
            if ((roadArr[l].startID == car->currNode && roadArr[l].endID == car->currGoal) || (roadArr[l].endID == car->currNode && roadArr[l].startID == car->currGoal)) {
                road = roadArr[l];
                for (i = 0; i < 100; i++) {
                    if (roadArr[l].currCars[i] == -1) {
                        roadArr[l].currCars[i] = car->ID;
                        break;
                    }
                }
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
        *debugBool = 1;
    }
}

void changeLight(road roadArr[], roadPoints* node, int roadAmount) {
    int intersecMax = 740;
    int i, j = 0;
    int connectedRoadIndex[4];

    for (i = 0; i < roadAmount; i++) {
        if (roadArr[i].startID == node->ID || roadArr[i].endID == node->ID) {
            connectedRoadIndex[j] = i;
            j++;
        }
    }

    if (node->intersecTimer >= intersecMax) {
        node->intersecTimer = 0;
    }

    switch (node->intersecTimer) {
    /* IN CASE OF RED/RED */
    case 0:
        for (i = 0; i < node->numOfConnections; i++) {
            if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                roadArr[connectedRoadIndex[i]].intersecLightStart = 0;
            } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                roadArr[connectedRoadIndex[i]].intersecLightEnd = 0;
            }
        }
        break;

    /* IN CASE OF RED/YELLOW_START */
    case 10:
        for (i = 0; i < node->numOfConnections; i++) {
            if (i == 0 || i == 2) {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 0;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 0;
                }
            } else {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 1;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 1;
                }
            }
        }
        break;

    /* IN CASE OF RED/GREEN */
    case 30:
        for (i = 0; i < node->numOfConnections; i++) {
            if (i == 0 || i == 2) {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 0;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 0;
                }
            } else {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 2;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 2;
                }
            }
        }
        break;

    /* IN CASE OF RED/YELLOW_STOP */
    case 330:
        for (i = 0; i < node->numOfConnections; i++) {
            if (i == 0 || i == 2) {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 0;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 0;
                }
            } else {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 1;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 1;
                }
            }
        }
        break;

    /* IN CASE OF RED/RED */
    case 370:
        for (i = 0; i < node->numOfConnections; i++) {
            if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                roadArr[connectedRoadIndex[i]].intersecLightStart = 0;
            } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                roadArr[connectedRoadIndex[i]].intersecLightEnd = 0;
            }
        }
        break;

    /* IN CASE OF YELLOW_START/RED */
    case 380:
        for (i = 0; i < node->numOfConnections; i++) {
            if (i == 0 || i == 2) {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 1;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 1;
                }
            } else {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 0;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 0;
                }
            }
        }
        break;

    /* IN CASE OF GREEN/RED */
    case 400:
        for (i = 0; i < node->numOfConnections; i++) {
            if (i == 0 || i == 2) {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 2;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 2;
                }
            } else {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 0;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 0;
                }
            }
        }
        break;

    /* IN CASE OF YELLOW_STOP/RED */
    case 700:
        for (i = 0; i < node->numOfConnections; i++) {
            if (i == 0 || i == 2) {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 1;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 1;
                }
            } else {
                if (roadArr[connectedRoadIndex[i]].startID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightStart = 0;
                } else if (roadArr[connectedRoadIndex[i]].endID == node->ID) {
                    roadArr[connectedRoadIndex[i]].intersecLightEnd = 0;
                }
            }
        }
        break;

    default:
        /* DEFAULT CASE */
        break;
    }

    node->intersecTimer += 1;
}