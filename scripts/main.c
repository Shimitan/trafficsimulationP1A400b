#include "network.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_CARS 110

double disToEnd(car car, road road, struct car carArr[]);
void moveCar(car* car, struct car carArr[], road* road, int carNum);
double breakLength(car car);
void isCarInFront(car car, road road, struct car carArr[], double* carLocation, int* bool);
int cmpfunc (const void * a, const void * b);


void createCar(car* car, road road, int* k);


int main(void){
    int i = 0;
    int active = 0;
    int k = 0;
    int j = 110;
    road road;
    car car[100];
    // car car[NUM_OF_CARS];
    // for(i = 0; i < NUM_OF_CARS; i++){
    //     car[i].active = 0;
    // }
    /*
    car.acceleration = 0.034;
    car.location = 0;
    car.speed = 0;
    car.dirBool = 1;
    */
    road.length = 1000;
    road.speedLimit = 1.3889;

    for(i = 0; i < 100; i++){
        road.currCars[i] = -1;
    }

    car[0].location = 0;
    car[0].dirBool = 1;
    car[0].speed = 0;
    car[0].acceleration = 0.034;
    car[0].ID = 0;
    car[0].active = 0;
    car[0].currGoal = road.length;
    road.currCars[0] = 0;

    car[1].location = 0;
    car[1].dirBool = 1;
    car[1].speed = 0;
    car[1].acceleration = 0.05;
    car[1].ID = 1;
    car[1].active = 0;
    car[1].currGoal = road.length;
    

    car[0].active = 1;
    while(1){

        k++;

        if(k > 10 && active != 1){
            car[1].active = 1;
            active = 1;
            road.currCars[1] = 1;
        }


        for(i = 0; i < 2; i++){
            moveCar(&car[i], car, &road, i);
            if(car[i].active == 1){
                printf("Location = %lf, Speed = %lf, ID = %d\n\n", car[i].location, car[i].speed, car[i].ID);
            }
        }

        if(car[0].active == 0 && car[1].active == 0){
            break;
        }
    }
    
    // while(1){
    //     j++;
    //     if(j >= 120){
    //         createCar(&car[k], road, &k);
    //         j = 0;
    //     }
        
    //     for(i = 0; i < k; i++){
    //         moveCar(&car[i], car, &road, i);
    //     }

    //     if(k > 100){
    //         break;
    //     }
    // }
    return 0;
}


void createCar(car* car, road road, int* k){
    int SENTINAL = 1, i = 0;

    car->acceleration = 0.034;
    car->location = 0;
    car->speed = 0;
    car->dirBool = 1;
    car->active = 1;
    car->ID = *k;

    while(SENTINAL){

        if(road.currCars[i] == -1){
            road.currCars[i] = *k;
            car->arrayIndex = i;
        }
    }
    *k += 1;
}

