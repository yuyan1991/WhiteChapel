#include<iostream>
#include<cstring>
#include"constants.h"
using namespace std;

bool graph[numPositions + 1][numPositions + 1];
int position[numPositions + 1][numPositions + 1];
int zone[numPositions + 1][numPositions + 1];
int numOutPosition[numPositions + 1];
int numOutZone[numPositions + 1];

void read(int cur, int g[][numPositions + 1], int l[], int endMark) {
    int x;
    while (true) {
        scanf("%d", &x);
        // printf("%d ",x);
        if (x == endMark) break;
        // if (x<0) printf("error: %d\n", cur);
        g[cur][l[cur]++] = x;
        if (endMark == -1) graph[cur][x] = true;
    }
}

void readPositions(int cur) {
    read(cur, position, numOutPosition, -1);
}

void readZones(int cur) {
    read(cur, zone, numOutZone, -2);
}

void readMap() {
    freopen(mapFile, "r", stdin);
    memset(graph, 0, sizeof(graph));
    for (int cur=1;cur<=numPositions;cur++) {
        readPositions(cur);
        readZones(cur);
        // printf("\n");
    }
    fclose(stdin);
    freopen(console, "r", stdin);
}

bool verifyMap() {
    bool success = true;
    for (int x = 1; x <= numPositions; x++) {
        for (int y = x; y<= numPositions; y++) {
            if (graph[x][y] != graph[y][x]) {
                printf("Please check (%d, %d)\n", x, y);
                cout<<graph[x][y]<<"\t"<<graph[y][x]<<endl;
                success = false;
            }
        }
    }
    return success;
}