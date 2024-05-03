#include <stdio.h>
#include "../include/client.h"

int main(int argc, char *argv[]) {
    int iStatus = 0;

    iStatus = client(argc, argv);
    if (iStatus != 0) {
        printf("Error: %d\n", iStatus);
    }

    return 0;
}