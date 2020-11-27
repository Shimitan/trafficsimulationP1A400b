#include <stdio.h>
#include <stdlib.h>

#define INFINITY 2147483647

//UwUbuntu

typedef struct car car;
typedef struct road road;
typedef struct roadPoints roadPoints;

struct roadPoints{
    int ID;

    int connections;

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
void moveCar(car* car, struct car carArr[], road* road, struct road roadArr[], int carNum, int* debugBool);
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
    printf("aheadLocation: %lf carInFront: %d\n", aheadLocation, carInFront);
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
        if(temp == -1){
            SENTINAL = 0;
        }else if(carArr[i].dirBool == car.dirBool){
            locations[k] = carArr[temp].location;
            k++;
            i++;
        }else{
            i++;
        }
    }

    qsort(locations, k, sizeof(double), cmpfunc);

    for(j = 0; j < k; j++){
        printf("%lf ", locations[j]);
    }
    printf("\n");
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

void moveCar(car* car, struct car carArr[], road* road, struct road roadArr[], int carNum, int* debugBool){
    int l;

    if(car->active == 1){
        
        double distanceToEnd = disToEnd(*car, *road, carArr);
        car->breakLength = breakLength(*car);
        printf("BreakLength: %lf\n", car->breakLength);

        /* Ændre bilens fart */
        if(car->speed < (road->speedLimit + car->speedDeviation) && car->breakLength < distanceToEnd){
            car->speed += car->acceleration;
        }else if(car->breakLength >= distanceToEnd && car->speed > 0){
            car->speed -= car->acceleration;
            if(car->speed < 0){
                car->speed = 0;
            }
        }

        /* Ændre bilens position */
        if(car->dirBool == 1){
            car->location += car->speed;
        }else{
            car->location -= car->speed;
        }

        /* Snapper bilen når den stopper */
        if(car->location >= road->length && car->dirBool == 1){
            car->speed = 0;
            car->active = 0;
            // printf("CurrentRoadStart: %d\n", road->startID);
            // printf("CurrentRoadEnd: %d\n", road->endID);
            printf("Road[%d].startID: %d\n", 1, roadArr[1].startID);
            printf("Road[%d].endID: %d\n", 1, roadArr[1].endID);

            /* DETTE FUNKTIONS KALD VIRKER IKKE... JEG ANDER IKK HVORFOR SÅ JEG HAR NU KONSTATERET AT DET IKKE ER MIT PROBLEM LIGE NU LMAO!*/
            pushArray(*car, road);
            
            printf("Road[%d].startID: %d\n", 1, roadArr[1].startID);
            printf("Road[%d].endID: %d\n", 1, roadArr[1].endID);
            // printf("CurrentRoadStart: %d\n", road->startID);
            // printf("CurrentRoadEnd: %d\n", road->endID);
            
            car->location = road->length; /* endpoint om det er bag en anden bil eller i et kryds */

            car->currNode = road->endID;
            car->pathStep++;

            printf("Car stopped %d\n", carNum);

            changeRoad(car, roadArr, debugBool);
        }else if(car->location <= 0 && car->dirBool == 0){
            car->speed = 0;
            car->active = 0;
            pushArray(*car, road);
            car->location = 0;

            car->currNode = road->startID;
            car->pathStep++;

            printf("Car stopped %d\n", carNum);

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
        printf("currCar[%d]: %d\n", i, road->currCars[i]);
    }
}

double kmhTompds(road* road){
    return road->speedLimit = (road->speedLimit/3.6)/10;
}

void pathfinding(car* car, road roadArr[], struct roadPoints roadPointsArr[]){
    int i;
    int currentNode = car->currNode;

    //100 skal være antal nodes
    for(i = 0; i < 100; i++){
        if(!(i == currentNode)){

            



        }
    }
    
    for(i = 0; i < roadPointsArr[currentNode].connections; i++){

    }





}

void roadOutput(car car[], road road){




}

void changeRoad(car* car, road roadArr[], int* debugBool){
    int l;
    road road;

    if (car->path[car->pathStep + 1] != -1){
        car->active = 1;
        car->currGoal = car->path[car->pathStep + 1];
        for (l = 0; l < 100; l++) {
            if ((roadArr[l].startID == car->currNode && roadArr[l].endID == car->currGoal) || (roadArr[l].endID == car->currNode && roadArr[l].startID == car->currGoal)) {
                road = roadArr[l];
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
        printf("Endgoal Reached!\n");
        *debugBool = 1;
    }
}