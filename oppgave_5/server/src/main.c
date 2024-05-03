#include <stdio.h>
#include "../include/server.h"
#include "../include/main.h"

int main(int argc, char *argv[]){
    int iStatus = 0;

    iStatus = server(argc, argv);
    if (iStatus != 0) {
        printf("Error: %d\n", iStatus);
        return -1;
    }
    return 0;
}