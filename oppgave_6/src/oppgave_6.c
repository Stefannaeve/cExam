#include <stdio.h>

#include "../include/oppgave_6.h"

int oppgave_6() {
    int a = 0;
   char *b = "Hello, World!";
    while (a < 10) {
        printf("%d\n", a+1);
        a++;
    }
   return 0;
}