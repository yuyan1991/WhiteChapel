#include<iostream>
#include<cstring>
#include"constants.h"
using namespace std;

int numCarriage, numLight;
int moveCounter;
int path[maxMoveCount][maxPath][numPositions];
int pathLength[maxMoveCount][maxPath];
int totalPath[maxMoveCount];
int estimatedJackHome[numPositions + 1];

void stuffForJack(int turn) {
    switch (turn) {
        case 1:
            numCarriage = 3;
            numLight = 2;
            break;
        case 2:
            numCarriage = 2;
            numLight = 2;
            break;
        case 3:
            numCarriage = 2;
            numLight = 1;
            break;
        case 4:
            numCarriage = 1;
            numLight = 1;
            break;
    }
}

void addNewPath(int moveCounter, int prevPath, int currentPosition) {
    if (moveCounter == 1) {
        pathLength[moveCounter][totalPath[moveCounter]] = 1;
    } else {
        pathLength[moveCounter][totalPath[moveCounter]] = pathLength[moveCounter-1][prevPath] + 1;
        for (int i = 0; i < pathLength[moveCounter-1][prevPath]; i++) {
            path[moveCounter][totalPath[moveCounter]][i] = path[moveCounter-1][prevPath][i];
        }
    }
    path[moveCounter][totalPath[moveCounter]][pathLength[moveCounter][totalPath[moveCounter]]-1] = currentPosition;
    totalPath[moveCounter]++;
}

void startKilling(int turn) {
    moveCounter = 1;
    memset(totalPath,0, sizeof(totalPath));
    int deathPosition;
    printf("Please enter the death position: ");
    scanf("%d", &deathPosition);
    addNewPath(moveCounter, -1, deathPosition);

    if (turn == 3) {
        printf("Please enter another death position: ");
        scanf("%d", &deathPosition);
        addNewPath(moveCounter, -1, deathPosition);
    }
}

bool validateJackEscapeWay(int way) {
    bool success;
    switch (way) {
        case 1:
            success = true;
            break;
        case 2:
            success = numCarriage > 0;
            break;
        case 3:
            success = numLight > 0;
            break;
    }
    return success;
}

void escapePath(int steps) {
    for (int step = 0; step < steps; step++) {

    }
}

void escapeZone() {

}

void useWalking() {
    escapePath(1);
}

void useCarriage() {
    numCarriage--;
    escapePath(2);
}

void useLight() {
    numLight--;
    escapeZone();
}

void escapingJack() {
    int way;
    do {
        printf("Which way would Jack pick to escape? (1. walking 2. carriage 3. light): ");
        scanf("%d", &way);
    } while (!validateJackEscapeWay(way));

    switch (way) {
        case 1:
            useWalking();
            break;
        case 2:
            useCarriage();
            break;
        case 3:
            useLight();
            break;
    }

}

void movingCap() {

}

bool isBackHome() {
    return false;
}

void estimateJackHome(int moveCounter) {
    // TODO: update estimatedJackHome

    printf("Jack's Home List:");
    for (int i = 1; i <= numPositions; i++) 
        if (estimatedJackHome[i] == moveCounter) 
            printf(" %d", i);
    printf("\n");
}

void chase(int turn) {
    while (moveCounter < maxMoveCount) {
        escapingJack();
        if (isBackHome()) {
            estimateJackHome(moveCounter);
            break;
        }
        movingCap();
        printf("Carriage: %d left, Light: %d left.\n", numCarriage, numLight);
    }
}

void initialize() {
    memset(estimatedJackHome, 0, sizeof(estimatedJackHome));
}

void launchGame() {
    initialize();
    for (int turn=1; turn<=totalTurn; turn++) {
        printf("%d turn begin:", turn);
        stuffForJack(turn);
        startKilling(turn);
        chase(turn);
        
        printf("%d turn end.", turn)
    }

}