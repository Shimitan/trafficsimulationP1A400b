#include "statistics.h"

/*Skal koden køre et forudbestemt stykke tid eller er det op til brugeren?*/

/* 1000 målinger / sekund
 * 60 * 1000 målinger / minut
 * 60000 * 4 bytes
 * 240000 bytes
 * 240 kB / minuts data
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
 *
 * */

double* createSpeedArray(int amountOfCars, int ticksPerSecond){
    double *speedArray = (double *) malloc(SECONDS_PER_MINUTE * amountOfCars * ticksPerSecond);
    return speedArray;
}







