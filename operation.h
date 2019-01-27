#include<iostream>
#include<cstring>
#include"whiteChapelMap.h"
using namespace std;

int numCarriage, numLight;
int moveCounter;
int path[maxMoveCount][maxPath][numPositions];
int pathLength[maxMoveCount][maxPath];
bool enablePath[maxMoveCount][maxPath];
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
        if (!enablePath[moveCounter-1][prevPath]) return;
        pathLength[moveCounter][totalPath[moveCounter]] = pathLength[moveCounter-1][prevPath] + 1;
        for (int i = 0; i < pathLength[moveCounter-1][prevPath]; i++) {
            path[moveCounter][totalPath[moveCounter]][i] = path[moveCounter-1][prevPath][i];
        }
    }
    enablePath[moveCounter][totalPath[moveCounter]] = true;
    path[moveCounter][totalPath[moveCounter]][pathLength[moveCounter][totalPath[moveCounter]]-1] = currentPosition;
    totalPath[moveCounter]++;
}

void startKilling(int curTurn) {
    moveCounter = 1;
    memset(totalPath,0, sizeof(totalPath));
    int deathPosition;
    printf("Please enter the death position: ");
    scanf("%d", &deathPosition);
    addNewPath(moveCounter, -1, deathPosition);

    if (curTurn == 3) {
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

void findAndAddNextPosition(int startPosition, int curPath, int g[][numPositions + 1], int l[]) {
    for (int i = 0; i < l[startPosition]; i++) {
        addNewPath(moveCounter, curPath, g[startPosition][i]);
    }
}

int getLastPositionFromPath(int move, int curPath) {
    return path[move][curPath][pathLength[move][curPath] - 1];
}

void findAndAddNewPossiblePaths(int g[][numPositions + 1], int l[]) {
    for (int i=0; i < totalPath[moveCounter - 1]; i++) {
        if (enablePath[moveCounter-1][i]) {
            int startPosition = getLastPositionFromPath(moveCounter - 1, i);
            findAndAddNextPosition(startPosition, i, g, l);
        }
    }
}

void escapeThroughPath(int steps) {
    for (int step = 0; step < steps; step++) {
        moveCounter++;
        findAndAddNewPossiblePaths(position, numOutPosition);
    }
}

void escapeThroughZone() {
    moveCounter++;
    findAndAddNewPossiblePaths(zone, numOutZone);
}

void useWalking() {
    escapeThroughPath(1);
}

void useCarriage() {
    numCarriage--;
    escapeThroughPath(2);
}

void useLight() {
    numLight--;
    escapeThroughZone();
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

void printPossiblePaths(const char* outFile) {
    freopen(outFile, "w", stdout);
    printf("====================================================================\n");
    printf("The list of possible paths for Jack: \n");
    int cnt = 0;
    for (int i=0;i<totalPath[moveCounter];i++) {
        if (!enablePath[moveCounter][i]) continue;
        cnt++;
        for (int j=0;j<pathLength[moveCounter][i]; j++) {
            printf("%d ", path[moveCounter][i][j]);
        }
        printf("\n")
    }
    printf("Total %d path(s).\n", cnt);
    printf("====================================================================\n");
    freopen(console, "w", stdout);
}

void filterPath(int markedPosition, bool isCorrect) {
    for (int i = 0; i < totalPath[moveCounter]; i++) {
        bool isEnable = !isCorrect;
        for (int j = 0; j < pathLength[moveCounter][i]; j++)
            if (path[moveCounter][i][j] == markedPosition) {
                isEnable = isCorrect;
                break;
            }
        enablePath[moveCounter][i] = isEnable;
    }
}

void handleMaybeAnswer() {
    // TODO
    // Assume nothing to do at present
}

void ask() {
    int x, answer;
    while (true) {
        printf("Enter a position you wanna ask (or enter 0 to quit): ");
        scanf("%d", &x);
        if (x<=0 || x>numPositions) break;
        printf("Is this position correct? (1. Yes, 2. No, 3. Maybe): ");
        scanf("%d", answer);
        switch (answer) {
            case 1:
                filterPath(x, true);
                break;
            case 2:
                filterPath(x, false);
                break;
            case 3:
                handleMaybeAnswer();
                break;
        }
        printPossiblePaths(escapingJackPathsFile);
        printPossiblePaths(console);
    }

}

void seize() {
    // Seems nothing to do at present
}

void movingCap() {
    printPossiblePaths(escapingJackPathsFile);
    printPossiblePaths(console);
    for (int curCap=1;curCap<=numCap;curCap++) {
        int action;
        printf("Hello, Sir #%d! What do you want to do? (1. Ask 2. Seize 3. Quit): ", curCap);
        scanf("%d", &action);
        switch (action) {
            case 1:
                ask();
                break;
            case 2:
                seize();
                break;
            case 3:
                break;
        }
    }
}

bool isBackHome() {
    int x;
    do {
        printf("Is Jack back home? (1. Yes, 2. No): ");
        scanf("%d", &x);
    } while (x!=1 && x!=2);
    
    return x==1;
}

void printEstimatedJackHome(int turn, const char* outFile) {
    freopen(outFile, "w", stdout);
    printf("Jack's Home List:");
    for (int i = 1; i <= numPositions; i++) 
        if (estimatedJackHome[i] == turn) 
            printf(" %d", i);
    printf("\n");
    freopen(console, "w", stdout);
}

void estimateJackHome(int turn) {
    for (int i=0;i<totalPath[moveCounter];i++) {
        int lastPosition = getLastPositionFromPath(moveCounter, i);
        if (estimatedJackHome[lastPosition] == turn - 1) estimatedJackHome[lastPosition] = turn;
    }

    printEstimatedJackHome(turn, console);
    printEstimatedJackHome(turn, estimatedHomeFile);
}

void printDisablePath() {
    // TODO
}

void logInvestigation() {
    printf("What do you want to do? (0. quit 1. print disable path): ");
    int option;
    scanf("%d", &option);
    switch (option) {
        case 0:
            break;
        case 1:
            printDisablePath();
            break;
    }
}

void chase(int turn) {
    while (moveCounter < maxMoveCount) {
        escapingJack();
        if (isBackHome()) {
            estimateJackHome(turn);
            break;
        }
        movingCap();
        logInvestigation();
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
        
        printf("%d turn end.", turn);
    }
}