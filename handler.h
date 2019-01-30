#include"whiteChapelMap.h"
#include<algorithm>
using namespace std;

struct PositionData {
    int position;
    int occurrenceCount;
};
int numCarriage, numLight;
int moveCounter;
int path[maxMoveCount][maxPath][numSteps];
int pathLength[maxMoveCount][maxPath];
bool enablePath[maxMoveCount][maxPath];
int totalPath[maxMoveCount];
int validTotalPath[maxMoveCount];
int occurrenceCount[numPositions + 1];
int lastOccurrenceCount[numPositions + 1];
int mustAppearPositions[numPositions];
int mustAppearPositionsCount;
int estimatedJackHome[numPositions + 1];
PositionData positionsMax[numPositions + 1];
PositionData positionsLastMax[numPositions + 1];

int getLastPositionFromPath(int move, int curPath) {
    return path[move][curPath][pathLength[move][curPath] - 1];
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

void findAndAddNewPossiblePaths(int g[][numPositions + 1], int l[]) {
    for (int i=0; i < totalPath[moveCounter - 1]; i++) {
        if (enablePath[moveCounter-1][i]) {
            int startPosition = getLastPositionFromPath(moveCounter - 1, i);
            findAndAddNextPosition(startPosition, i, g, l);
        }
    }
}

void calculateValidTotalPath() {
    validTotalPath[moveCounter] = 0;
    for (int i=0;i<totalPath[moveCounter];i++) {
        if (!enablePath[moveCounter][i]) continue;
        validTotalPath[moveCounter]++;
    }
}

void escapeThroughPath(int steps) {
    for (int step = 0; step < steps; step++) {
        moveCounter++;
        findAndAddNewPossiblePaths(position, numOutPosition);
        calculateValidTotalPath();
    }
}

void escapeThroughZone() {
    moveCounter++;
    findAndAddNewPossiblePaths(zone, numOutZone);
    calculateValidTotalPath();
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

void printPossiblePaths(const char* outFile) {
    freopen(outFile, "w", stdout);
    printf("====================================================================\n");
    printf("The list of possible paths for Jack: \n");

    for (int i=0;i<totalPath[moveCounter];i++) {
        if (!enablePath[moveCounter][i]) continue;
        for (int j=0;j<pathLength[moveCounter][i]; j++) {
            printf("%d ", path[moveCounter][i][j]);
        }
        printf("\n");
    }
    printf("Total %d path(s).\n", validTotalPath[moveCounter]);
    printf("====================================================================\n");
    freopen(console, "w", stdout);
}

void calculateOccurrenceCount() {
    memset(occurrenceCount, 0, sizeof(occurrenceCount));
    int vis[numPositions + 1];
    for (int i = 0; i < totalPath[moveCounter]; i++) {
        if (!enablePath[moveCounter][i]) continue;
        memset(vis, 0, sizeof(vis));
        for (int j = 0; j < pathLength[moveCounter][i]; j++) {
            int position = path[moveCounter][i][j];
            vis[position]++;
        }
        for (int k = 1; k <= numPositions; k++) occurrenceCount[k] += (vis[k]>0?1:0);
    }
}

void calculateLastOccurrenceCount() {
    memset(lastOccurrenceCount, 0, sizeof(lastOccurrenceCount));
    int vis[numPositions + 1];
    for (int i = 0; i < totalPath[moveCounter]; i++) {
        if (!enablePath[moveCounter][i]) continue;
        int position = getLastPositionFromPath(moveCounter, i);
        lastOccurrenceCount[position]++;
    }
}

void checkMustAppearPositions() {
    mustAppearPositionsCount = 0;
    for (int i = 1; i <= numPositions; i++) {
        if (occurrenceCount[i] == validTotalPath[moveCounter]) {
            mustAppearPositions[mustAppearPositionsCount++] = i;
        }
    }
}

void printMustAppearPositions(const char* outFile) {
    freopen(outFile, "w", stdout);
    printf("====================================================================\n");
    printf("Must appear positions list:");
    for (int i = 0; i < mustAppearPositionsCount; i++)
        printf(" %d", mustAppearPositions[i]);
    printf("\n");
    printf("====================================================================\n");
    freopen(console, "w", stdout);
}

void checkAndPrintMustAppearPositions() {
    checkMustAppearPositions();
    printMustAppearPositions(mustAppearPositionsFile);
    printMustAppearPositions(console);
}

void filterPath(int markedPosition, bool isCorrect) {
    for (int i = 0; i < totalPath[moveCounter]; i++) {
        if (!enablePath[moveCounter][i]) continue;
        bool isEnable = !isCorrect;
        for (int j = 0; j < pathLength[moveCounter][i]; j++)
            if (path[moveCounter][i][j] == markedPosition) {
                isEnable = isCorrect;
                break;
            }
        enablePath[moveCounter][i] = isEnable;
    }
    calculateValidTotalPath();
}

void handleMaybeAnswer() {
    // TODO
    // Assume nothing to do at present
}

bool positionsOccurrenceOrderByDesc(PositionData p1, PositionData p2) {
    return p1.occurrenceCount > p2.occurrenceCount;
}

void printMaxOccurrencePositions(const char *outFile) {
    freopen(outFile, "w", stdout);
    printf("====================================================================\n");
    printf("Max Occurrence List:\n");
    for (int i = 1; i <= numPositions; i++) {
        if (positionsMax[i].occurrenceCount > 0 && occurrenceCount[positionsMax[i].position] < validTotalPath[moveCounter]) {
            printf("%d occurs: %d\n", positionsMax[i].position, positionsMax[i].occurrenceCount);
        }
    }
    printf("====================================================================\n");
    freopen(console, "w", stdout);
}

void checkMaxOccurrencePositions() {
    for (int i = 1; i <= numPositions; i++) {
        positionsMax[i].position = i;
        positionsMax[i].occurrenceCount = occurrenceCount[i];
    }
    sort(positionsMax + 1, positionsMax + numPositions + 1, positionsOccurrenceOrderByDesc);
    printMaxOccurrencePositions(maxOccurrencePositionsFile);
    printMaxOccurrencePositions(console);
}

void printMaxLastOccurrencePositions(const char *outFile) {
    freopen(outFile, "w", stdout);
    printf("====================================================================\n");
    printf("Max Last Occurrence List:\n");
    for (int i = 1; i <= numPositions; i++) {
        if (positionsLastMax[i].occurrenceCount > 0 && occurrenceCount[positionsLastMax[i].position] < validTotalPath[moveCounter]) {
            printf("%d occurs: %d\n", positionsLastMax[i].position, positionsLastMax[i].occurrenceCount);
        }
    }
    printf("====================================================================\n");
    freopen(console, "w", stdout);
}

void checkMaxLastOccurrencePositions() {
    for (int i = 1; i <= numPositions; i++) {
        positionsLastMax[i].position = i;
        positionsLastMax[i].occurrenceCount = lastOccurrenceCount[i];
    }
    sort(positionsLastMax + 1, positionsLastMax + numPositions + 1, positionsOccurrenceOrderByDesc);
    printMaxLastOccurrencePositions(maxLastOccurrencePositionsFile);
    printMaxLastOccurrencePositions(console);
}

void massageStatistics() {
    calculateOccurrenceCount();
    calculateLastOccurrenceCount();
    checkAndPrintMustAppearPositions();
    checkMaxOccurrencePositions();
    checkMaxLastOccurrencePositions();
}

void filterPathContainSeizePositionInLastOccurrence(int position) {
    for (int i = 0; i < totalPath[moveCounter]; i++) {
        if (!enablePath[moveCounter][i]) continue;
        int lastOccurrence = getLastPositionFromPath(moveCounter, i);
        enablePath[moveCounter][i] = (lastOccurrence != position);
    }
    calculateValidTotalPath();
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
        if (!enablePath[moveCounter][i]) continue;
        int lastPosition = getLastPositionFromPath(moveCounter, i);
        if (estimatedJackHome[lastPosition] == turn - 1) estimatedJackHome[lastPosition] = turn;
    }

    printEstimatedJackHome(turn, console);
    printEstimatedJackHome(turn, estimatedHomeFile);
}

void logInvestigation() {
    printf("What do you want to do? (0. quit): ");
    int option;
    scanf("%d", &option);
    switch (option) {
        case 0:
            break;
    }
}

void initialize() {
    memset(estimatedJackHome, 0, sizeof(estimatedJackHome));
}