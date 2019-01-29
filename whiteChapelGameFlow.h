#include"handler.h"
using namespace std;

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

void startKilling(int curTurn) {
    moveCounter = 1;
    memset(totalPath,0, sizeof(totalPath));
    memset(occurrenceCount, 0, sizeof(occurrenceCount));
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

void ask() {
    int x, answer;
    while (true) {
        printf("Enter a position you wanna ask (or enter 0 to quit): ");
        scanf("%d", &x);
        if (x<=0 || x>numPositions) break;
        printf("Is this position correct? (1. Yes, 2. No, 3. Maybe): ");
        scanf("%d", &answer);
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
        massageStatistics();
    }

}

void seize() {
    int x, answer;
    printf("Enter a position you wanna seize: ");
    scanf("%d", &x);
    printf("Is this position correct? (1. Yes, 2. No): ");
    scanf("%d", &answer);
    switch (answer) {
        case 1:
            printf("Congratulations!\n");
            system("pause");
            exit(0);
            break;
        case 2:
            filterPathContainSeizePositionInLastOccurrence(x);
            break;
    }
    printPossiblePaths(escapingJackPathsFile);
    massageStatistics();
}

void movingCap() {
    printPossiblePaths(escapingJackPathsFile);
    massageStatistics();

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

void launchGame() {
    initialize();
    for (int turn=1; turn<=totalTurn; turn++) {
        printf("%d turn begin:\n", turn);
        stuffForJack(turn);
        startKilling(turn);
        chase(turn);
        
        printf("%d turn end.\n", turn);
    }
}