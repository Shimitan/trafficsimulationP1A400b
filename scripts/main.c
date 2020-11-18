#include "roadOLD.h"
#include "carOLD.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int makeCar(double acceleration);

int main(void){
   int done = 0;

    /* road setup */ 
    struct road road1;
    road1.start.X = 0;
    road1.start.Y = 0;
    road1.end.X = 100;
    road1.end.Y = 0;
    road1.speedLimit = 50;
    road1.lanes = 1;

    /* car setup */ 
    struct car *carP, car1;
    car1.location.X = 0;
    car1.location.Y = 0;
    (*carP).acceleration = 1;
    car1.speed = 0;

    makeCar((*carP).acceleration);
    /*
    while(!done){

        if(car1.speed < road1.speedLimit){
            car1.speed += car1.acceleration;
        }

        car1.locationX += car1.speed;

        printf("speed = %lf, location = %lf\n", car1.speed, car1.locationX);


        if(car1.locationX > road1.endX){
            done = 1;
        }
    }
    printf("done\n"); */
}

int makeCar(double acceleration){

    printf("%lf\n", acceleration);
    

}