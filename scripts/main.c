#include "network.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_CARS 110

double disToEnd(car car, road road, struct car carArr[]);
void moveCar(car* car, struct car carArr[], road* road, int carNum);
double breakLength(car car);
void isCarInFront(car car, road road, struct car carArr[], double* carLocation, int* bool);
int cmpfunc (const void * a, const void * b);
double distanceBetweenCars(car car);
void roadOutput(car car[], road road);
double kmhTompds(road road);


void createCar(car* car, road* road, int* k);


int main(void){
    int i = 0;
    int active = 0;
    int k = 0;
    int j = 110;
    road road;
    car car[1000];
    
    for(i = 0; i < 1000; i++){
        car[i].active = 0;
    }
    
    
    road.length = 1000;
    road.speedLimit = 50;

    road.speedLimit = kmhTompds(road);

    srand(398);

    for(i = 0; i < 100; i++){
        road.currCars[i] = -1;
    }

    while(1){
        j++;
        if(j >= 120){
            createCar(&car[k], &road, &k);
            j = 0;
        }
        
        for(i = 0; i < k; i++){
            moveCar(&car[i], car, &road, i);
            if(car[i].active == 1){
                printf("Location = %lf, Speed = %lf, ID = %d\n\n", car[i].location, car[i].speed, car[i].ID);
            }
        }

        if(k > 100){
            printf("breakDOWN\n");
            break;
        }
    }
    return 0;
}


void createCar(car* car, road* road, int* k){
    int SENTINAL = 1, i = 0;

    do{
        car->currNode = rand() % 2;
        car->endGoal = rand() % 2;
    }while(car->currNode == car->endGoal);
    car->currGoal = car->endGoal;



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

