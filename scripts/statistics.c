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

double* createSpeedArray(int amountOfCars, int ticksPerSecond){
    double *speedArray = (double *) malloc(SECONDS_PER_MINUTE * amountOfCars * ticksPerSecond * sizeof(double));
    return speedArray;
}

void analyseData(data oneMinuteData, ){

}





