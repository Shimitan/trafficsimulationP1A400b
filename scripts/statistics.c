#include "statistics.h"

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
 */

/* Sets up the minuteData 2D array */
void setUpDataArray(int amountOfRoads, int minutesSimulated, int amountOfCars, int ticksPerSecond, data minuteData[amountOfRoads][minutesSimulated], int speedIndex[], int carsOnRoad[]){
    int i, l;
    for (i = 0; i < amountOfRoads * 2; i++){
        for (l = 0; l < minutesSimulated; l++){
            minuteData[i][l].speedOfCars = createSpeedArray(amountOfCars, ticksPerSecond);
            if (minuteData[i][l].speedOfCars == NULL){
                printf("Error allocating memory for [%d][%d]\n", i, l);
                exit(EXIT_FAILURE);
            }
            minuteData[i][l].timeInterval = 1;
            minuteData[i][l].speedMeasurementCount = 0;
            minuteData[i][l].flowCarCount = 0;
            minuteData[i][l].densityCarCount = 0;
            
            
        }
        speedIndex[i] = 0;
        carsOnRoad[i] = 0;
    }
    /* Gud hader menneskeheden så det her skal stå der for at lortet virker i [0][0]
     * Din familie vil ikke være i live til næste jul hvis du fjerner det her
     *      - Varme julehilsner <3,
     *        Mette Frederiksen, 07-12-2020*/
    minuteData[0][0].timeInterval = 1;
}

/*Allocates an array to store the speed for each car for each tick on a given road*/
double* createSpeedArray(int amountOfCars, int ticksPerSecond){
    double *speedArray = (double *) calloc(SECONDS_PER_MINUTE * amountOfCars * ticksPerSecond,  sizeof(double));
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
                calculateCongestion(&minuteData[l][i]);
            }
        }
    }
}

void measureSpeed(double speed, data *dp, int index, int dir, double maxSpeed){
    dp->speedOfCars[index] = speed;
    dp->speedMeasurementCount++;
    dp->direction = dir;
    dp->maxSpeed = mpdsTokmh(maxSpeed);
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

void calculateCongestion(data *dp){
    dp->congestion = (int) ((1 - (dp->averageSpeed/dp->maxSpeed)) * 100);
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

void calculateBiggerIntervals(int amountOfRoads, int minutesSimulated, data minuteData[amountOfRoads][minutesSimulated]){

}




/* Prints the analysed data */
void printAnalysedData(int amountOfRoads, int minutesSimulated, data minuteData[amountOfRoads][minutesSimulated]){
    int i, l;
    for (l = 0; l < amountOfRoads; l++) {
        for (i = 0; i < minutesSimulated; i++){
            if (minuteData[l][i].speedMeasurementCount > 0){
                printf("Ticks with car on road %2d dir %1d for minute %3d: %4d ", minuteData[l][i].roadID, minuteData[l][i].direction, minuteData[l][i].timeStamp, minuteData[l][i].speedMeasurementCount);
                printf("with average speed %05.2lf km/h,", minuteData[l][i].averageSpeed);
                printf(" flow %03.2lf cars/min, density %04.2f cars/km and %3d%% congestion\n",  minuteData[l][i].calculatedFlow, minuteData[l][i].density, minuteData[l][i].congestion);
            }
        }
        printf("\n");
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


