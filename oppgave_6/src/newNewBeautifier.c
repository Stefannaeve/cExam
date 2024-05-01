//
// Created by stefannaeve on 4/30/24.
//

#include <stdio.h>
#include <malloc.h>
#include "../include/newNewBeautifier.h"
#include "../include/linkedList.h"
#include <string.h>

#define MAX_STRING_LENGTH 206

void removeEveryConcurrentlyTreeLinesOfSpace(NODE_LIST *psnList);

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
        iLength = strlen(buffer);
        tempChar = (char *) malloc(iLength + 1);
        if (tempChar == NULL) {
            printf("Failed to allocate memory\n");
            return;
        }

        strcpy(tempChar, buffer);

        SENT_NODE psnTemp;
        memset(&psnTemp, 0, sizeof(SENT_NODE));
        psnTemp.line = tempChar;
        psnTemp.size = iLength;

        add(&psnList, &psnTemp);

        free(tempChar);
        memset(buffer, 0, MAX_STRING_LENGTH);

        i++;
    }
    fclose(cFile);

    printAllNodes(&psnList);

    removeEveryConcurrentlyTreeLinesOfSpace(&psnList);

    freeLinkedList(&psnList);

}

void removeEveryConcurrentlyTreeLinesOfSpace(NODE_LIST *psnList) {
    NODE *psnCurrent = psnList->pHead;
    NODE *psnNext;
    NODE *psnPrev = NULL;
    int i = 0;
    while (psnCurrent != NULL) {
        psnNext = psnCurrent->pNextNode;
        if (psnPrev != NULL && psnNext != NULL) {
            // Remove every instance of "   " in psnCurrent->line, and replace it with "t"
            char *psnCurrentLine = psnCurrent->line;
            char psnCurrentLineCopy[MAX_STRING_LENGTH];
        }
        psnPrev = psnCurrent;
        psnCurrent = psnNext;
    }
}