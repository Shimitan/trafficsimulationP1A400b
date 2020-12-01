#include <math.h>

struct point{
    double X;
    double Y;
};

struct vector{
    double X;
    double Y;
};

struct road{

    double speedLimit;
    int lanes;

    struct point start;
    struct point end;
    
    struct vector direction;

};

/* Finder enhedsvektor til en vej som bruges til bevægelse af biler */
struct vector findDirection(struct point start, struct point end){
    struct vector vectorOut;
    struct vector vectorRoad;

    vectorRoad.X = end.X - start.X;
    vectorRoad.Y = end.Y - start.Y;

    vectorOut.X = (vectorRoad.X/sqrt((vectorRoad.X * vectorRoad.X) + (vectorRoad.Y * vectorRoad.Y)));
    vectorOut.Y = (vectorRoad.Y/sqrt((vectorRoad.X * vectorRoad.X) + (vectorRoad.Y * vectorRoad.Y)));

    return vectorOut;
}
