#include <stdio.h>
#include "tableReservationList.h"
#include "menuApplication.h"

int main(int argc, char *argv[]){
    int iStatus = 0;
    if (argc != 1){
        printf("%s takes no arguments\n", argv[0]);
        return -1;
    }

    iStatus = menuApplication();

    if (iStatus != 0){
        printf("Error in menuApplication\n");
        return -1;
    }

    return 0;
}