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
void setUpDataArray(int amountOfRoads, int minutesSimulated, int amountOfCars, int ticksPerSecond, data **minuteData, int speedIndex[], int carsOnRoad[]/*, road roadArr[]*/){
    int i, l;
    for (i = 0; i < amountOfRoads; i++){
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
            minuteData[i][l].timeStamp = l;
            if (i < amountOfRoads / 2) {
                minuteData[i][l].roadID = i;
                minuteData[i][l].direction = 1;
                //minuteData[i][l].maxSpeed = mpdsTokmh(roadArr[i].speedLimit);
                //minuteData[i][l].roadLength = roadArr[i].length;
            } else {
                minuteData[i][l].roadID = (i - amountOfRoads / 2);
                minuteData[i][l].direction = 0;
                //minuteData[i][l].maxSpeed = mpdsTokmh(roadArr[i - amountOfRoads / 2].speedLimit);
                //minuteData[i][l].roadLength = roadArr[i - amountOfRoads / 2].length;
            }
            
            
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
    double *speedArray = (double *) malloc(SECONDS_PER_MINUTE * amountOfCars * ticksPerSecond * sizeof(double));
    if (speedArray == NULL){
        printf("Error allocating int array\n");
        exit(EXIT_FAILURE);
    }
    return speedArray;
}

/*Allocates a 2D-array of type data*/
data** createDataArray(int amountOfRoads, int minutesSimulated){
    int i;
    data **dat;
    dat = calloc(amountOfRoads, sizeof(data *));
    if (dat == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < amountOfRoads; i++){
        dat[i] = calloc(minutesSimulated, sizeof(data));
        if (dat[i] == NULL) {
            printf("Error allocating memory\n");
            exit(EXIT_FAILURE);
        }
    }
    return dat;
}

int* allocateIntArray(int length){
    int *i = calloc(length, sizeof(int));
    if (i == NULL){
        printf("Error allocating int array\n");
        exit(EXIT_FAILURE);
    }
    return i;
}

void analyseData(int amountOfRoads, int minutesSimulated, data **minuteData){
    int i, l, quarterHourIntervals, hourIntervals;
    data **quarterHourData, **hourData;
    quarterHourIntervals = minutesSimulated / 15;
    if (minutesSimulated % 15 != 0){
        quarterHourIntervals += 1;
    }
    hourIntervals = minutesSimulated / 60;
    if (minutesSimulated % 60 != 0){
        hourIntervals += 1;
    }
    
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
    quarterHourData = createDataArray(amountOfRoads, quarterHourIntervals);
    hourData = createDataArray(amountOfRoads, hourIntervals);
    calculateLargerIntervals(amountOfRoads, minutesSimulated, 15, minuteData, quarterHourData);
    calculateLargerIntervals(amountOfRoads, minutesSimulated, 60, minuteData, hourData);
    printf("-------------------------------->  1 minute intervals <--------------------------------\n\n");
    printAnalysedData(amountOfRoads, minutesSimulated, minuteData);
    printf("--------------------------------> 15 minute intervals <--------------------------------\n\n");
    printAnalysedData(amountOfRoads, quarterHourIntervals, quarterHourData);
    printf("--------------------------------> 60 minute intervals <--------------------------------\n\n");
    printAnalysedData(amountOfRoads, hourIntervals, hourData);
    
    
    free(quarterHourData);
    free(hourData);
}

void measureSpeed(double speed, data *dp, int index){
    dp->speedOfCars[index] = speed;
    dp->speedMeasurementCount++;
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

void calculateLargerIntervals(int amountOfRoads, int minutesSimulated, int interval, data **minuteData, data **largeIntervalData){
    int i, l, count, index;
    
    for (l = 0; l < amountOfRoads; l++){
        index = 0;
        count = 0;
        for (i = 0; i < minutesSimulated; i++) {
            if (count == 0) {
                largeIntervalData[l][index].timeStamp = minuteData[l][i].timeStamp;
                largeIntervalData[l][index].roadID = minuteData[l][i].roadID;
                largeIntervalData[l][index].roadLength = minuteData[l][i].roadLength;
                largeIntervalData[l][index].direction = minuteData[l][i].direction;
                largeIntervalData[l][index].maxSpeed = minuteData[l][i].maxSpeed;
                largeIntervalData[l][index].averageSpeed = minuteData[l][i].averageSpeed;
                largeIntervalData[l][index].flowCarCount = minuteData[l][i].flowCarCount;
                largeIntervalData[l][index].densityCarCount = minuteData[l][i].densityCarCount;
                largeIntervalData[l][index].speedMeasurementCount = minuteData[l][i].speedMeasurementCount;
                largeIntervalData[l][index].timeInterval = count + 1;
            } else {
                largeIntervalData[l][index].timeInterval = count + 1;
                largeIntervalData[l][index].speedMeasurementCount += minuteData[l][i].speedMeasurementCount;
                largeIntervalData[l][index].averageSpeed += minuteData[l][i].averageSpeed;
                largeIntervalData[l][index].flowCarCount += minuteData[l][i].flowCarCount;
                largeIntervalData[l][index].densityCarCount += minuteData[l][i].densityCarCount;
            }
            if (count == (interval - 1) && i != (minutesSimulated - 1)){
                index++;
                count = 0;
            } else {
                count++;
            }
        }
    }
    
    for (l = 0; l < amountOfRoads; l++){
        for (i = 0; i <= index; i++){
            largeIntervalData[l][i].averageSpeed = largeIntervalData[l][i].averageSpeed / largeIntervalData[l][i].timeInterval;
            largeIntervalData[l][i].densityCarCount = largeIntervalData[l][i].densityCarCount / largeIntervalData[l][i].timeInterval;
            calculateFlow(&largeIntervalData[l][i]);
            calculateDensity(&largeIntervalData[l][i]);
            calculateCongestion(&largeIntervalData[l][i]);
        }
    }
}




/* Prints the analysed data */
void printAnalysedData(int amountOfRoads, int minutesSimulated, data **minuteData){
    int i, l, print = 0;
    for (l = 0; l < amountOfRoads; l++) {
        for (i = 0; i < minutesSimulated; i++){
            if (minuteData[l][i].speedMeasurementCount > 0){
                printf("Ticks with car on road %2d dir %1d for minute %3d: %4d ", minuteData[l][i].roadID, minuteData[l][i].direction, minuteData[l][i].timeStamp, minuteData[l][i].speedMeasurementCount);
                printf("with average speed %05.2lf km/h,", minuteData[l][i].averageSpeed);
                printf(" flow %03.2lf cars/min, density %04.2f cars/km and %3d%% congestion\n",  minuteData[l][i].calculatedFlow, minuteData[l][i].density, minuteData[l][i].congestion);
                print = 1;
            }
        }
        if (print){
            printf("\n");
            print = 0;
        }
    }
}

/* Write data to file */


/* Frees the allocated memory  */
void freeSpeedArrays(int amountOfRoads, int minutesSimulated, data **minuteData){
    int i, l;
    for (i = 0; i < amountOfRoads; i++){
        for (l = 0; l < minutesSimulated; l++){
            free(minuteData[i][l].speedOfCars);
            minuteData[i][l].speedOfCars = NULL;
        }
    }
}
