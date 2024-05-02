#include <stdio.h>
#include "../include/beautifier.h"

int main(int argc, char *argv[]) {
    int status = 0;

    if (argc > 1) {
        printf("%s takes no arguments\n", argv[0]);
        return -1;
    }

    status = beautify("src/oppgave6_test.c");
    if (status != 0) {
        printf("Error: Shutting down...\n");
        return -1;
    }
    return 0;
}
