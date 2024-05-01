//
// Created by stefannaeve on 4/30/24.
//

#include <stdio.h>
#include <malloc.h>
#include "../include/newNewBeautifier.h"
#include "../include/linkedList.h"
#include <string.h>
#include <errno.h>

#define MAX_STRING_LENGTH 206

void removeEveryConcurrentlyTreeLinesOfSpace(NODE_LIST *psnList);

void changeAllCharVariableNamesToHungerianNotation(NODE_LIST *list);

void changeWhileLoopsToForLoops(NODE_LIST *list);

int checkIfLineHasComment(char *currentLine, int size);

void beautify(char *filename) {
    char strBuffer[MAX_STRING_LENGTH];
    char *pszTempChar;

    int iLength = 0;
    int i = 0;

    NODE_LIST psnList = {NULL, NULL, 0};

    FILE *fdCFile = fopen(filename, "r");
    if (fdCFile == NULL) {
        printf("Could not open fdCFile %s\n", filename);
        return;
    }


    while (fgets(strBuffer, MAX_STRING_LENGTH, fdCFile) != NULL) {
        SENT_NODE psnTemp;
        iLength = strlen(strBuffer);

        pszTempChar = (char *) malloc(iLength + 1);
        if (pszTempChar == NULL) {
            printf("Failed to allocate memory\n");
            return;
        }

        strcpy(pszTempChar, strBuffer);

        memset(&psnTemp, 0, sizeof(SENT_NODE));
        psnTemp.line = pszTempChar;
        psnTemp.size = iLength;

        add(&psnList, &psnTemp);

        free(pszTempChar);
        memset(strBuffer, 0, MAX_STRING_LENGTH);

        i++;
    }
    fclose(fdCFile);

    removeEveryConcurrentlyTreeLinesOfSpace(&psnList);

    printAllNodes(&psnList);

    changeAllCharVariableNamesToHungerianNotation(&psnList);

    printAllNodes(&psnList);

    changeWhileLoopsToForLoops(&psnList);

    freeLinkedList(&psnList);

}

void changeWhileLoopsToForLoops(NODE_LIST *list) {
    char *pszWhile = "while";
    int iSizeOfWhile = 5;

    NODE *psnCurrent = list->pHead;

    while (psnCurrent != NULL) {
        char *currentLine = psnCurrent->line;
        // Loop though string in node
        for (int j = 0; j < psnCurrent->size; j++) {
            // Check if the line contains "char ", if it does, change position to possible start of variable
            if (strncmp(&currentLine[j], pszWhile, iSizeOfWhile) == 0) {

            }
        }
    }
}

int checkIfLineHasComment(char *currentLine, int size) {
    for (int j = 0; j < size; j++) {
        if (currentLine[j] == '/' && currentLine[j + 1] == '/') {
            return j;
        }
    }
    return 0;
}


void changeAllCharVariableNamesToHungerianNotation(NODE_LIST *list) {
    NODE *psnCurrent = list->pHead;
    char *pszVariableStart = "*psz";
    char strOldVariable[MAX_STRING_LENGTH];
    char strUpgradeOldVariable[MAX_STRING_LENGTH];
    char strNewLine[MAX_STRING_LENGTH];
    char strNewVariable[MAX_STRING_LENGTH];
    char *pszTempString;

    int i = 0;
    int iListPosition = 0;
    int iLinePositionForVariable = 0;
    int iSizeOfCharOffset = 5;

    int iLengthOfOldVariable = 0;
    int iLengthOfNewVariable = 0;
    int iLengthOfNewLine = 0;

    int iCommentPosition = 0;

    int iComment = 0;

    memset(strOldVariable, 0, MAX_STRING_LENGTH);
    memset(strUpgradeOldVariable, 0, MAX_STRING_LENGTH);
    memset(strNewLine, 0, MAX_STRING_LENGTH);
    memset(strNewVariable, 0, MAX_STRING_LENGTH);
    // Loop through all lines in the list to find char, and check if they are using hungerian notation
    while (psnCurrent != NULL) {
        iComment = 0;
        char *currentLine = psnCurrent->line;
        // Loop though string in node
        iCommentPosition = checkIfLineHasComment(currentLine, psnCurrent->size);

        if (iCommentPosition != 0) {
            iComment = 1;
        }

        for (int j = 0; j < (iComment == 0 ? psnCurrent->size : iCommentPosition); j++) {
            // Check if the line contains "char ", if it does, change position to possible start of variable
            if (strncmp(&currentLine[j], "char ", iSizeOfCharOffset) == 0) {
                j += iSizeOfCharOffset;
                iLinePositionForVariable = j;
                // Exclude spaces
                while (currentLine[j] == ' ') {
                    j++;
                }
                i = 0;
                // Find the end of the variable
                while (currentLine[j] != ' ' && currentLine[j] != ';') {
                    strOldVariable[i++] = currentLine[j++];
                }
                strOldVariable[i] = '\0';

                iLengthOfOldVariable = strlen(strOldVariable);

                // Check if variable starts with *, if so remove it
                if (strOldVariable[0] == '*') {
                    for (int k = 0; k < iLengthOfOldVariable; ++k) {
                        strOldVariable[k] = strOldVariable[k + 1];
                    }
                }

                // Check if first letter is lowercase, if so change it to uppercase
                if ('a' <= strOldVariable[0] && strOldVariable[0] <= 'z') {
                    strOldVariable[0] = strOldVariable[0] - 32;
                }

                memset(strNewVariable, 0, MAX_STRING_LENGTH);

                // Add *psz to start of variable, and add old variable with changed first letter to uppercase if needed
                strncpy(strNewVariable, pszVariableStart, strlen(pszVariableStart));
                strncat(strNewVariable, strOldVariable, MAX_STRING_LENGTH);

                iLengthOfNewVariable = strlen(strNewVariable);

                // Make new line out of old code before new variable, add new variable, fill in old code before variable
                for (int k = 0; k < iLinePositionForVariable; ++k) {
                    strNewLine[k] = currentLine[k];
                }
                for (int k = 0; k < iLengthOfNewVariable; ++k) {
                    strNewLine[iLinePositionForVariable + k] = strNewVariable[k];
                }
                int oldPositionAfterVariable = iLinePositionForVariable + iLengthOfOldVariable + 1;
                for (int k = iLinePositionForVariable + iLengthOfNewVariable;
                     k < psnCurrent->size + (iLengthOfNewVariable -
                                             iLengthOfOldVariable); ++k) {
                    strNewLine[k] = currentLine[oldPositionAfterVariable++];
                }

                iLengthOfNewLine = strlen(strNewLine);

                pszTempString = (char *) malloc(iLengthOfNewLine + 1);
                if (pszTempString == NULL) {
                    printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
                    return;
                }
                strncpy(pszTempString, strNewLine, iLengthOfNewLine);
                pszTempString[iLengthOfNewLine] = '\0';

                SENT_NODE psnTemp;
                memset(&psnTemp, 0, sizeof(SENT_NODE));
                psnTemp.line = pszTempString;
                psnTemp.size = iLengthOfNewLine;

                deleteSpecificNode(list, iListPosition);
                addAtIndex(list, &psnTemp, iListPosition);

                free(pszTempString);

                printf("Old variable: %s\n", strOldVariable);
                printf("New variable: %s\n", strNewVariable);
                printf("\n");

            }
        }
        iListPosition++;
        psnCurrent = psnCurrent->pNextNode;
    }
}

void removeEveryConcurrentlyTreeLinesOfSpace(NODE_LIST *psnList) {
    NODE *psnCurrent = psnList->pHead;
    while (psnCurrent != NULL) {
        char *psnCurrentLine = psnCurrent->line;
        int newSize = 0;
        for (int j = 0; j < psnCurrent->size; j++) {
            // Check if there are three spaces in a row, if so replace with \t
            if (j < psnCurrent->size - 2 && psnCurrentLine[j] == ' ' && psnCurrentLine[j + 1] == ' ' &&
                psnCurrentLine[j + 2] == ' ') {
                psnCurrentLine[newSize++] = '\t';
                // Skip two spaces
                j += 2;
            } else {
                psnCurrentLine[newSize++] = psnCurrentLine[j];
            }
        }
        psnCurrentLine[newSize] = '\0';
        psnCurrent->size = newSize;
        psnCurrent = psnCurrent->pNextNode;
    }
}