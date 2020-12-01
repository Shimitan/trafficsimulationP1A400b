#include <stdio.h>
#include <unistd.h>


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
    int currCars;

};

struct car{

    double acceleration;
    double speed;
    double location;
    double breakLength;

    int currGoal;
    int endGoal;
    int dirBool; /* 1 er positiv retning. 0 er negativ retning */

};

double breakLength(struct car car);
void moveCar(struct car* car, struct road road);
double disToEnd(struct car car, struct road road);

/* lav om til: afstand til hvad end der er foran. om det er kryds eller anden bil */
double disToEnd(struct car car, struct road road){
    if(car.dirBool == 1){
        return road.length - car.location;
    }else{
        return road.length - (road.length - car.location);
    }
}

void moveCar(struct car* car, struct road road){
    double distanceToEnd = disToEnd(*car, road);
    car->breakLength = breakLength(*car);

    if(car->speed < road.speedLimit && car->breakLength < distanceToEnd){
        car->speed += car->acceleration;
    }else if(car->breakLength >= distanceToEnd){
        car->speed -= car->acceleration;
    }



    if(car->dirBool == 1){
        car->location += car->speed;
    }else{
        car->location -= car->speed;
    }

    if(car->speed <= 0){
        car->speed = 0;
        if(car->dirBool == 1){
            car->location = road.length; /* endpoint om det er bag en anden bil eller i et kryds */
        }else{
            car->location = 0;
        }
    }
    printf("speed = %lf\nlocation = %lf\n", car->speed, car->location);
}

double breakLength(struct car car){
    double dist = 0;
    double i;
    
    for(i = car.speed; i > 0; i -= car.acceleration){
        dist += i;
    }

    return dist;
}


int main(void){
    struct road road;
    struct car car;
    int i = 0;

    car.acceleration = 0.034;
    car.location = 0;
    car.speed = 0;
    car.dirBool = 1;

    road.length = 1000;
    road.speedLimit = 1.3889;

    

    while(1){
        moveCar(&car, road);

        i++;
        
        printf("%d\n", i);
        if(car.location == 1000){
            break;
        }
        
    }

    
    return 0;
}