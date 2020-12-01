#include <math.h>
#include "RoadOLD.h"


struct car{

    struct point location;
    double acceleration;
    double reactionTime;
    double speed;
    struct point endGoal;
    struct point currGoal;
    int currentRoadID;
    int dirBool;

};

/* Bevægelse af bil langs vej */
void moveCar(struct car* car, struct road* road){

    if(car->speed < road->speedLimit){
        car->speed += car->acceleration;
    }

    if(car->dirBool == 1){
        car->location.X += car->speed * road->direction.X;
        car->location.Y += car->speed * road->direction.Y;
    }else{
        car->location.X -= car->speed * road->direction.X;
        car->location.Y -= car->speed * road->direction.Y;
    }
}

double distenceToEnd(struct car car, struct road road){
    struct vector vectorToEnd;

    vectorToEnd.X = end.X - location.X;
    vectorToEnd.Y = end.Y - location.Y;

    return sqrt((vectorToEnd.X * vectorToEnd.X)+(vectorToEnd.Y * vectorToEnd.Y));
}