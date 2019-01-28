#include"operation.h"
using namespace std;

int main() {
    readMap();
    printf(verifyMap()?"White Chapel's map is okay!\n":"White Chapel's map has error(s).\n");
    launchGame();

    system("pause");
    return 0;
}