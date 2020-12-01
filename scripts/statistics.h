#define AMOUNT_OF_CARS 100
#define TICKS_PER_SECOND 10

struct data{
    int amountOfCars;
    int timeStamp;
    double *speedOfCars; //skal være et array af alle målte hastigheder i løbet af et minut i simulationen
    double averageSpeed;
    int roadID;
    int roadLength;
    int timeInterval;
};

typedef struct data data;

