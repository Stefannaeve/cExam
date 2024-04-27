#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "tableReservationList.h"
#include "orderList.h"
#include "menuApplication.h"

int main(int argc, char *argv[]){
    int status = 0;
    if (argc != 1){
        printf("%s takes no arguments\n", argv[0]);
        return -1;
    }

    status = menuApplication();

    if (status != 0){
        printf("Error in menuApplication\n");
        return -1;
    }

    return 0;
}