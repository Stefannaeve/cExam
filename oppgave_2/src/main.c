#include <stdio.h>
#include "program.h"

#include "main.h"

int main(int argc, char *argv[]) {
    int iStatus = 0;
    if (argc != 1) {
        printf("%s takes no arguments\n", argv[0]);
        return -1;
    }

    iStatus = program();
    if (iStatus != 0) {
        printf("Program failed with status %d\n", iStatus);
        return -1;
    }
    return 0;
}
