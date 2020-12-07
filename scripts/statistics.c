#include "statistics.h"

/*Skal koden køre et forudbestemt stykke tid eller er det op til brugeren?*/

/* 1000 målinger / sekund
 * 60 * 1000 målinger / minut
 * 60000 * 8 bytes
 * 480000 bytes
 * 480 kB / minuts data
 *
 * */

/* Data for 1 minut
 * Data for hvert kvarter
 *      - Baseret på 15 1 minuts målinger
 *
 * Data for hver time
 *      - Baseret på 4 15 minutters målinger
 *
 * Data for hvert døgn? Maybe(TM)
 *      - Jeg har ingen ide om hvorvidt det er en god ide
 * */

/* Sets up the minuteData 2D array */
void setUpDataArray(int amountOfRoads, int minutesSimulated, data minuteData[amountOfRoads][minutesSimulated], int speedIndex[], int carsOnRoad[]){
    int i, l;
    for (i = 0; i < amountOfRoads * 2; i++){
        for (l = 0; l < minutesSimulated; l++){
            minuteData[i][l].speedOfCars = createSpeedArray(AMOUNT_OF_CARS, TICKS_PER_SECOND);
            if (minuteData[i][l].speedOfCars == NULL){
                printf("Error allocating memory for [%d][%d]\n", i, l);
                exit(EXIT_FAILURE);
            } else {
                minuteData[i][l].speedMeasurementCount = 0;
                minuteData[i][l].flowCarCount = 0;
                minuteData[i][l].densityCarCount = 0;
                minuteData[i][l].timeInterval = 1;
            }
        }
        speedIndex[i] = 0;
        carsOnRoad[i] = 0;
    }
}

/*Allocates an array to store the speed for each car for each tick on a given road*/
double* createSpeedArray(int amountOfCars, int ticksPerSecond){
    double *speedArray = (double *) malloc(SECONDS_PER_MINUTE * amountOfCars * ticksPerSecond * sizeof(double));
    return speedArray;
}

void analyseData(int amountOfRoads, int minutesSimulated, data minuteData[amountOfRoads][minutesSimulated]){
    int i, l;
    for (l = 0; l < amountOfRoads; l++) {
        for (i = 0; i < minutesSimulated; i++){
            if (minuteData[l][i].speedMeasurementCount > 0){
                averageSpeed(&minuteData[l][i]);
                calculateFlow(&minuteData[l][i]);
                calculateDensity(&minuteData[l][i]);
            }
        }
    }
}

void measureSpeed(double speed, data *dp, int index, int dir){
    dp->speedOfCars[index] = speed;
    dp->speedMeasurementCount++;
    dp->direction = dir;
}

void averageSpeed(data *dp){
    int i;
    double average = 0.0;
    if (dp->speedMeasurementCount > 0){
        for (i = 0; i < dp->speedMeasurementCount; i++){
            average += dp->speedOfCars[i];
        }
        average = average/dp->speedMeasurementCount;
    }
    dp->averageSpeed = mpdsTokmh(average);
}


void calculateDensity(data *dp){
    dp->density = (double) (dp->densityCarCount * 1000) / (dp->roadLength) ;
}

void countCarFlow(data *dp){
    dp->flowCarCount++;
}

void calculateFlow(data *dp){
    dp->calculatedFlow = (double) dp->flowCarCount / dp->timeInterval;
}

double mpdsTokmh(double speed){
    return speed * 10 * 3.6;
}

/* Prints the analysed data */
void printAnalysedData(int amountOfRoads, int minutesSimulated, data minuteData[amountOfRoads][minutesSimulated]){
    int i, l;
    for (l = 0; l < amountOfRoads; l++) {
        for (i = 0; i < minutesSimulated; i++){
            if (minuteData[l][i].speedMeasurementCount > 0){
                printf("Ticks with car on road %4d for minute %3d: %3d ", l, i, minuteData[l][i].speedMeasurementCount);
                printf("with average speed %05.2lf km/h, flow %03.2lf cars/min and density %3.2lf\n", minuteData[l][i].averageSpeed, minuteData[l][i].calculatedFlow, minuteData[l][i].density);
            }
        }
    }
}

/* Frees the allocated memory  */
void freeSpeedArrays(int amountOfRoads, int minutesSimulated, data minuteData[amountOfRoads][minutesSimulated]){
    int i, l;
    for (i = 0; i < amountOfRoads; i++){
        for (l = 0; l < minutesSimulated; l++){
            free(minuteData[i][l].speedOfCars);
            minuteData[i][l].speedOfCars = NULL;
        }
    }
}





/* Write data to file */


