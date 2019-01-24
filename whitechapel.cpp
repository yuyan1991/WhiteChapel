#include<iostream>
#include<cstring>
#include<algorithm>
#include"readmap.h"
using namespace std;

int main() {
    readMap();
    printf(verifyMap()?"White Chapel's map is okay!\n":"White Chapel's map has error(s).\n");
    return 0;
}