#include <stdio.h>
#include <stdlib.h>
#include "network.h"

void getInput(road roadArr[], int* nodeAmount, int* roadAmount);

int main(void) {
    int i, l, nodeAmount, roadAmount;

    road roadArr[100];
    roadPoints nodeArr[100];
    car car[1000];
    
    for(i = 0; i < 1000; i++){
        car[i].active = 0;
        car[i].pathStep = 0;
        for (l = 0; l < 100; l++) {
            car[i].path[l] = -1;
        }
    }

    for(i = 0; i < 100; i++){
        nodeArr[i].ID = -1;
        roadArr[i].startID = -1;
        roadArr[i].endID = -1;
    }

    for (l = 0; l < 100; l++){
        for(i = 0; i < 100; i++){
            roadArr[l].currCars[i] = -1;
        }
    }

    // printf("newline: %d\n", '\n');
    // printf("BackZero: %d\n", '\0');

    getInput(roadArr, &nodeAmount, &roadAmount);

    printf("nodeAmount: %d\n", nodeAmount);
    printf("roadAmount: %d\n", roadAmount);
    
    for (i = 0; i < 100; i++) {
        printf("roadArr[%d].startID: %d\n", i, roadArr[i].startID);
        printf("roadArr[%d].endID: %d\n", i, roadArr[i].endID);
        printf("roadArr[%d].lenght: %lf\n", i, roadArr[i].length);
        printf("roadArr[%d].speedLimit: %lf\n\n", i, roadArr[i].speedLimit);

    }

    return 0;
}

void getInput(road roadArr[], int* nodeAmount, int* roadAmount) {
    int ch, i = 0, l = 0, k = 0, counter = 0;
    char str[16];
    char strBuff[10000];
    FILE *inputFile;

    inputFile = fopen("xd.txt", "r");

    if (inputFile != NULL) {
        while ((ch = fgetc(inputFile)) != EOF) {
            strBuff[i] = ch;
            str[l] = strBuff[i];
            // printf("strBuff: %c\n", strBuff[i]);
            // printf("str: %c\n", str[l]);
            if (strBuff[i] == '\n') {
                switch (counter){
                case 0:
                    // for (i = 0; i < 16; i++) {
                    //     printf("str[%d]: %c\n", i, str[i]);
                    // }
                    str[l] = '\0';
                    *nodeAmount = atoi(str);
                    // printf("nodeAmount: %d\n", *nodeAmount);
                    counter++;
                    l = -1;
                    break;
                
                case 1:
                    str[l] = '\0';
                    *roadAmount = atoi(str);
                    // printf("roadAmount: %d\n", *roadAmount);
                    counter++;
                    l = -1;
                    break;
                
                default:
                    /*Skriv logic for indlæsning af data til de individuelle veje*/
                    str[l] = '\0';
                    sscanf(str, "%d, %d, %lf, %lf", &roadArr[k].startID, &roadArr[k].endID, &roadArr[k].length, &roadArr[k].speedLimit);
                    // printf("startID: %d\nendId: %d\nlength: %lf\nspeedLimit: %lf\n", roadArr[k].startID, roadArr[k].endID, roadArr[k].length, roadArr[k].speedLimit);
                    k++;
                    l = -1;
                    break;
                }
            }

            i++;
            l++;

        }
            str[l] = '\0';
            sscanf(str, "%d, %d, %lf, %lf", &roadArr[k].startID, &roadArr[k].endID, &roadArr[k].length, &roadArr[k].speedLimit);
            // printf("startID: %d\nendId: %d\nlength: %lf\nspeedLimit: %lf\n", roadArr[k].startID, roadArr[k].endID, roadArr[k].length, roadArr[k].speedLimit);
            k++;
    }

}