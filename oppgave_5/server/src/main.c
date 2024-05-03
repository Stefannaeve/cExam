#include <stdio.h>
#include "../include/server.h"
#include "../include/main.h"

int main(int argc, char *argv[]){
    int iStatus = 0;
    if (argc != 1) {
        printf("%s has no arguments\n", argv[0]);
        return -1;
    }
    iStatus = server();
    if (iStatus != 0) {
        printf("Error: %d\n", iStatus);
        return -1;
    }
    return 0;
}