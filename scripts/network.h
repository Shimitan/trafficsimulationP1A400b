#include <stdio.h>
#include <stdlib.h>

//UwUbuntu

typedef struct car car;
typedef struct road road;

struct roadPoints{
    int ID;
    int connections;
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

    int currNode;
    int currGoal;
    int endGoal;
    int dirBool; /* 1 er positiv retning. 0 er negativ retning */
    int active;
    int ID;
};

double disToEnd(car car, road road, struct car carArr[]);
void moveCar(car* car, struct car carArr[], road* road, int carNum);
double breakLength(car car);
void isCarInFront(car car, road road, struct car carArr[], double* carLocation, int* bool);
int cmpfunc (const void * a, const void * b);
double distanceBetweenCars(car car);
void pushArray(car car, road *road);

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

void moveCar(car* car, struct car carArr[], road* road, int carNum){
    if(car->active == 1){
        
        double distanceToEnd = disToEnd(*car, *road, carArr);
        car->breakLength = breakLength(*car);
        printf("BreakLength: %lf\n", car->breakLength);

        /* Ændre bilens fart */
        if(car->speed < road->speedLimit && car->breakLength < distanceToEnd){
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
            pushArray(*car, road);
            
            car->location = road->length; /* endpoint om det er bag en anden bil eller i et kryds */
            
            printf("Car stopped %d\n", carNum);
        }else if(car->location <= 0 && car->dirBool == 0){
            car->speed = 0;
            car->active = 0;
            pushArray(*car, road);
            car->location = 0;
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