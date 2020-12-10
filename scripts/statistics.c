#include "statistics.h"

/* Sets up the minuteData 2D array */
void setUpDataArray(int amountOfRoads, int minutesSimulated, int amountOfCars, int ticksPerSecond, data **minuteData, int speedIndex[], int carsOnRoad[], road roadArr[]){
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
            if (i < amountOfRoads / 2){
                minuteData[i][l].roadID = i;
                minuteData[i][l].direction = 1;
                minuteData[i][l].maxSpeed = mpdsTokmh(roadArr[i].speedLimit);
                minuteData[i][l].roadLength = roadArr[i].length;
            } else {
                minuteData[i][l].roadID = (i - amountOfRoads / 2);
                minuteData[i][l].direction = 0;
                minuteData[i][l].maxSpeed = mpdsTokmh(roadArr[i - amountOfRoads / 2].speedLimit);
                minuteData[i][l].roadLength = roadArr[i - amountOfRoads / 2].length;
            } 
        }
        speedIndex[i] = 0;
        carsOnRoad[i] = 0;
    }
    /* This is neccesary to make the first observation work, because the program mistakenly sets the first time interval to 0 */
    minuteData[0][0].timeInterval = 1;
}

/*Dynamically allocates an array to store the speed for each car for each tick on a given road*/
double* createSpeedArray(int amountOfCars, int ticksPerSecond){
    double *speedArray = (double *) malloc(SECONDS_PER_MINUTE * amountOfCars * ticksPerSecond * sizeof(double));

    if (speedArray == NULL){
        printf("Error allocating int array\n");
        exit(EXIT_FAILURE);
    }
    return speedArray;
}

/*Dynamically allocates a 2D-array of type data*/
data** createDataArray(int amountOfRoads, int minutesSimulated){
    int i;
    data **dat;

    dat = calloc(amountOfRoads, sizeof(data *));
    if (dat == NULL){
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < amountOfRoads; i++){
        dat[i] = calloc(minutesSimulated, sizeof(data));
        if (dat[i] == NULL){
            printf("Error allocating memory\n");
            exit(EXIT_FAILURE);
        }
    }
    return dat;
}

/* Dynamically allocates an int array */
int* allocateIntArray(int length){
    int *i = calloc(length, sizeof(int));

    if (i == NULL){
        printf("Error allocating int array\n");
        exit(EXIT_FAILURE);
    }
    return i;
}

/* Analyses the 1 minute data, 15 minute data and 60 minute data */
void analyseData(int amountOfRoads, int minutesSimulated, data **minuteData, int seed){
    int i, l, quarterHourIntervals, hourIntervals;
    char fileName[107] = "Output", intString[100]; 
    data **quarterHourData, **hourData;
    FILE *fp;

    quarterHourIntervals = minutesSimulated / 15;
    if (minutesSimulated % 15 != 0){
        quarterHourIntervals += 1;
    }
    hourIntervals = minutesSimulated / 60;
    if (minutesSimulated % 60 != 0){
        hourIntervals += 1;
    }
    
    for (l = 0; l < amountOfRoads; l++){
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
    
    /* File is created and emptied */
    sprintf(intString, "%d.txt", seed);
    strcat(fileName, intString);
    printf("%s\n", fileName);
    fp = fopen(fileName, "w");
    fclose(fp);
    /* File is opened and data is inputted */
    makeOutputFile(amountOfRoads, minutesSimulated, minuteData, 1, fileName);
    makeOutputFile(amountOfRoads, quarterHourIntervals, quarterHourData, 15, fileName);
    makeOutputFile(amountOfRoads, hourIntervals, hourData, 60, fileName);

    free(quarterHourData);
    free(hourData);
}

/*Measures the speed of a car*/
void measureSpeed(double speed, data *dp, int index){
    dp->speedOfCars[index] = speed;
    dp->speedMeasurementCount++;
}

/*Calculates the average speed for a given road*/
void averageSpeed(data *dp){
    int i;
    double average = 0.0;

    if (dp->speedMeasurementCount > 0){
        for (i = 0; i < dp->speedMeasurementCount; i++){
            average += dp->speedOfCars[i];
        }
        average = (double) average/dp->speedMeasurementCount;
    }
    dp->averageSpeed = mpdsTokmh(average);
}

/* Calculates the density */
void calculateDensity(data *dp){
    dp->density = (double) (dp->densityCarCount * 1000) / (dp->roadLength) ;
}

/* Calculates the congestion by finding the relative difference between the average speed and max speed */
void calculateCongestion(data *dp){
    dp->congestion = (int) ((1 - (dp->averageSpeed/dp->maxSpeed)) * 100);
    if (dp->congestion < 0){
        dp->congestion = 0;
    }
}

/* Counts the flow */
void countCarFlow(data *dp){
    dp->flowCarCount++;
}

/* Calculates the flow for a given data point */
void calculateFlow(data *dp){
    dp->calculatedFlow = (double) dp->flowCarCount / dp->timeInterval;
}

/* Converts meters per deci second to kmh */
double mpdsTokmh(double speed){
    return speed * 10 * 3.6;
}

/* Analyses the data and calculates the average in the intervals */
void calculateLargerIntervals(int amountOfRoads, int minutesSimulated, int interval, data **minuteData, data **largeIntervalData){
    int i, l, count, index;

    for (l = 0; l < amountOfRoads; l++){
        index = 0;
        count = 0;
        for (i = 0; i < minutesSimulated; i++){
            if (count == 0){
                largeIntervalData[l][index].timeStamp = minuteData[l][i].timeStamp;
            }
            if (largeIntervalData[l][index].speedMeasurementCount == 0 && minuteData[l][i].averageSpeed != 0){
                largeIntervalData[l][index].roadID = minuteData[l][i].roadID;
                largeIntervalData[l][index].roadLength = minuteData[l][i].roadLength;
                largeIntervalData[l][index].direction = minuteData[l][i].direction;
                largeIntervalData[l][index].maxSpeed = minuteData[l][i].maxSpeed;
                largeIntervalData[l][index].averageSpeed = minuteData[l][i].averageSpeed;
                largeIntervalData[l][index].flowCarCount = minuteData[l][i].flowCarCount;
                largeIntervalData[l][index].densityCarCount = minuteData[l][i].densityCarCount;
                largeIntervalData[l][index].speedMeasurementCount++;
            } else if (largeIntervalData[l][index].speedMeasurementCount != 0 && minuteData[l][i].averageSpeed != 0){
                largeIntervalData[l][index].speedMeasurementCount++;
                largeIntervalData[l][index].averageSpeed += minuteData[l][i].averageSpeed;
                largeIntervalData[l][index].flowCarCount += minuteData[l][i].flowCarCount;
                largeIntervalData[l][index].densityCarCount += minuteData[l][i].densityCarCount;
            }
            largeIntervalData[l][index].timeInterval = count + 1;
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
            if (largeIntervalData[l][i].timeInterval != 0){
                largeIntervalData[l][i].averageSpeed = (double) largeIntervalData[l][i].averageSpeed / largeIntervalData[l][i].speedMeasurementCount;
                largeIntervalData[l][i].densityCarCount = (double) largeIntervalData[l][i].densityCarCount / largeIntervalData[l][i].timeInterval;
                calculateFlow(&largeIntervalData[l][i]);
                calculateDensity(&largeIntervalData[l][i]);
                calculateCongestion(&largeIntervalData[l][i]);
            }
        }
    }
}



/* DETTE SKAL FJERNES I DEN ENDELIGE UDGAVE, DA VI IKKE SKAL BRUGE ET TERMINALPRINT */
/* Prints the analysed data */
void printAnalysedData(int amountOfRoads, int minutesSimulated, data **minuteData){
    int i, l, print = 0;

    for (l = 0; l < amountOfRoads; l++){
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
void makeOutputFile(int amountOfRoads, int minutesSimulated, data **minuteData, int interval, char *fileName){
    int i, l, print = 0;
    FILE *fp;

    fp = fopen(fileName, "ab");
    
    switch (interval){
    case 1:
        fprintf(fp, "-------------------------------------------------->  1-minute intervals <-------------------------------------------------\n\n");
        for (l = 0; l < amountOfRoads; l++){
            for (i = 0; i < minutesSimulated; i++){
                if (minuteData[l][i].speedMeasurementCount > 0){
                    fprintf(fp, "RoadID:\t%2d\tDirection:\t%1d\tMinute:\t%3d\t", minuteData[l][i].roadID, minuteData[l][i].direction, minuteData[l][i].timeStamp);
                    fprintf(fp, "Avg speed:\t%3.0lf\tkm/h\tMax speed:\t%3.0lf\tkm/h\t", minuteData[l][i].averageSpeed, minuteData[l][i].maxSpeed);
                    fprintf(fp, "Flow:\t%5.2f\tcars/min\tDensity:\t%5.2f\tcars/km\tCongestion:\t%3d\t%%\n",  minuteData[l][i].calculatedFlow, minuteData[l][i].density, minuteData[l][i].congestion);
                }
                
            }
        }
        break;
    
    case 15:
        fprintf(fp, "\n\n-------------------------------------------------->  15-minute intervals <-------------------------------------------------\n\n");
        for (l = 0; l < amountOfRoads; l++){
            for (i = 0; i < minutesSimulated; i++){
                if (minuteData[l][i].speedMeasurementCount > 0){
                    fprintf(fp, "RoadID:\t%2d\tDirection:\t%1d\tMinute:\t%3d\tMinutes with activity:\t%3d\t", minuteData[l][i].roadID, minuteData[l][i].direction, minuteData[l][i].timeStamp, minuteData[l][i].speedMeasurementCount);
                    fprintf(fp, "Avg speed:\t%3.0lf\tkm/h\tMax speed:\t%3.0lf\tkm/h\t", minuteData[l][i].averageSpeed, minuteData[l][i].maxSpeed);
                    fprintf(fp, "Flow:\t%5.2f\tcars/min\tDensity:\t%5.2f\tcars/km\tCongestion:\t%3d\t%%\n",  minuteData[l][i].calculatedFlow, minuteData[l][i].density, minuteData[l][i].congestion);
                }
                
            }
        }
        break;

    case 60:
        fprintf(fp, "\n\n-------------------------------------------------->  60-minute intervals <-------------------------------------------------\n\n");
        for (l = 0; l < amountOfRoads; l++){
            for (i = 0; i < minutesSimulated; i++){
                if (minuteData[l][i].speedMeasurementCount > 0){
                    fprintf(fp, "RoadID:\t%2d\tDirection:\t%1d\tMinute:\t%3d\tMinutes with activity:\t%3d\t", minuteData[l][i].roadID, minuteData[l][i].direction, minuteData[l][i].timeStamp, minuteData[l][i].speedMeasurementCount);
                    fprintf(fp, "Avg speed:\t%3.0lf\tkm/h\tMax speed:\t%3.0lf\tkm/h\t", minuteData[l][i].averageSpeed, minuteData[l][i].maxSpeed);
                    fprintf(fp, "Flow:\t%5.2f\tcars/min\tDensity:\t%5.2f\tcars/km\tCongestion:\t%3d\t%%\n",  minuteData[l][i].calculatedFlow, minuteData[l][i].density, minuteData[l][i].congestion);
                }
                
            }
        }
        break;
    
    default:
        break;
    }
    fclose(fp);
}

/* Frees the memory allocated for the speed arrays */
void freeSpeedArrays(int amountOfRoads, int minutesSimulated, data **minuteData){
    int i, l;
    
    for (i = 0; i < amountOfRoads; i++){
        for (l = 0; l < minutesSimulated; l++){
            free(minuteData[i][l].speedOfCars);
            minuteData[i][l].speedOfCars = NULL;
        }
    }
}
