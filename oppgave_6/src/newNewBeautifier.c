//
// Created by stefannaeve on 4/30/24.
//

#include <stdio.h>
#include <malloc.h>
#include "../include/newNewBeautifier.h"
#include "../include/linkedList.h"
#include <string.h>

#define MAX_STRING_LENGTH 206

void beautify(char *filename) {
    char buffer[MAX_STRING_LENGTH];
    int iLength = 0;
    char *tempChar;

    FILE *cFile = fopen(filename, "r");
    if (cFile == NULL) {
        printf("Could not open cFile %s\n", filename);
        return;
    }

    NODE_LIST psnList = {NULL, NULL, 0};

    int i = 0;
    while (fgets(buffer, MAX_STRING_LENGTH, cFile) != NULL) {
        int iLength = strlen(buffer);
        tempChar = (char *) malloc(iLength + 1);
        if (tempChar == NULL) {
            printf("Failed to allocate memory\n");
            return;
        }

        strcpy(tempChar, buffer);

        SENT_NODE psnTemp = {tempChar};

        add(&psnList, &psnTemp);

        free(tempChar);
        memset(buffer, 0, MAX_STRING_LENGTH);

        i++;
    }
    fclose(cFile);

    orderPrintAllNodes(&psnList);



}