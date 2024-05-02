//
// Created by stefannaeve on 4/30/24.
//

#include <stdio.h>
#include <malloc.h>
#include "../include/beautifier.h"
#include "../include/linkedList.h"
#include <string.h>
#include <errno.h>

#define MAX_STRING_LENGTH 206

int removeEveryConcurrentlyTreeLinesOfSpace(NODE_LIST *psnList);

int changeAllCharVariableNamesToHungerianNotation(NODE_LIST *list);

int changeWhileLoopsToForLoops(NODE_LIST *list);

int checkIfLineHasComment(char *currentLine, int size);

int changeWhileToFor(NODE *psnCurrent, int positionOfWhile, int nodePosition, NODE_LIST *list);

int findCondition(int *placementOfFor, int *nodeWithWhilePosition, int *nodePosition, NODE_LIST *list, char *strForCondition, int *foundWhile);

int findInitialization(NODE_LIST *list, int nodeWithWhilePosition, int nodePosition, char *strForInitialization);

int findIncrement(NODE_LIST *list, int *nodePosition, char *strForIncrement);

int beautify(char *filename) {
    char strBuffer[MAX_STRING_LENGTH];
    char *pszTempChar;

    int iLength = 0;
    int i = 0;
    int status = 0;

    NODE_LIST psnList = {NULL, NULL, 0};

    FILE *fdCFile = fopen(filename, "r");
    if (fdCFile == NULL) {
        printf("Could not open fdCFile %s - Error message: %s\n", filename, strerror(errno));
        return -1;
    }


    while (fgets(strBuffer, MAX_STRING_LENGTH, fdCFile) != NULL) {
        SENT_NODE psnTemp;
        iLength = strlen(strBuffer);

        pszTempChar = (char *) malloc(iLength + 1);
        if (pszTempChar == NULL) {
            printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
            return - 1;
        }

        strcpy(pszTempChar, strBuffer);

        pszTempChar[iLength] = '\0';

        memset(&psnTemp, 0, sizeof(SENT_NODE));
        psnTemp.line = pszTempChar;
        psnTemp.size = iLength;

        add(&psnList, &psnTemp);

        free(pszTempChar);
        memset(strBuffer, 0, MAX_STRING_LENGTH);

        i++;
    }
    fclose(fdCFile);


    status = removeEveryConcurrentlyTreeLinesOfSpace(&psnList);

    if (status != 0) {
        printf("Error in beautifier...\n");
        return -1;
    }

    printAllNodes(&psnList);

    status = changeAllCharVariableNamesToHungerianNotation(&psnList);

    if (status != 0) {
        printf("Error in beautifier...\n");
        return -1;
    }

    printAllNodes(&psnList);

    status = changeWhileLoopsToForLoops(&psnList);

    if (status != 0) {
        printf("Error in beautifier...\n");
        return -1;
    }

    printAllNodes(&psnList);


    FILE *beautifiedFile = fopen("src/beautified_oppgave6_test.c", "w");


    if (beautifiedFile == NULL) {
        printf("Could not open beautifiedFile - Error message: %s\n", strerror(errno));
        return -1;
    }

    NODE *psnCurrent = psnList.pHead;
    while (psnCurrent != NULL) {
        fprintf(beautifiedFile, "%s", psnCurrent->line);
        psnCurrent = psnCurrent->pNextNode;
    }

    fclose(beautifiedFile);

    freeLinkedList(&psnList);

    return 0;

}

int changeWhileLoopsToForLoops(NODE_LIST *list) {
    int nodeWithWhilePosition = 0;
    int nodePosition = 0;
    int foundWhile = 0;
    int placementOfFor = 0;
    int status = 0;

    int lengthInitialization = 0;
    int lengthCondition = 0;
    int lengthIncrement = 0;

    int positionOfFirstParenthesis = 0;
    int positionOfSecondParenthesis = 0;

    char strForInitialization[MAX_STRING_LENGTH] = {0};
    char strForCondition[MAX_STRING_LENGTH] = {0};
    char strForIncrement[MAX_STRING_LENGTH] = {0};


    status = findCondition(&placementOfFor, &nodeWithWhilePosition, &nodePosition, list, strForCondition, &foundWhile);
    if (status != 0) {
        printf("Error in findCondition...\n");
        return -1;
    }

    if (foundWhile) {
        status = findInitialization(list, nodeWithWhilePosition, nodePosition, strForInitialization);
    }
    if (status != 0) {
        printf("Error in findInitialization...\n");
        return -1;
    }

    // INCREMENT
    if (foundWhile) {
        status = findIncrement(list, &nodePosition, strForIncrement);
    }
    if (status != 0) {
        printf("Error in findIncrement...\n");
        return -1;
    }

    lengthInitialization = strlen(strForInitialization);
    lengthCondition = strlen(strForCondition);
    lengthIncrement = strlen(strForIncrement);

    // REPLACEMENT
    if (foundWhile){
        NODE *psnCurrent = list->pHead;
        for (int i = 0; i < nodeWithWhilePosition - 1; ++i) {
            psnCurrent = psnCurrent->pNextNode;
        }
        for (int i = placementOfFor; i < psnCurrent->size; ++i) {
            if (psnCurrent->line[i] == '('){
                positionOfFirstParenthesis = i;
            }
            if (psnCurrent->line[i] == ')' && positionOfFirstParenthesis != 0){
                positionOfSecondParenthesis = i;
                break;
            }
        }
            char *temp = (char *) malloc(positionOfFirstParenthesis + lengthInitialization + lengthCondition + lengthIncrement + psnCurrent->size - positionOfSecondParenthesis + 1);
        if (temp == NULL) {
            printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
            return - 1;
        }
        memset(temp, 0, positionOfFirstParenthesis + lengthInitialization + lengthCondition + lengthIncrement + psnCurrent->size - positionOfSecondParenthesis + 1);
        for (int i = 0; i < positionOfFirstParenthesis + 1; ++i) {
            temp[i] = psnCurrent->line[i];
        }
        for (int i = 0; i < lengthInitialization; ++i) {
            temp[positionOfFirstParenthesis + 1 + i] = strForInitialization[i];
        }
        for (int i = 0; i < lengthCondition; ++i) {
            temp[positionOfFirstParenthesis + lengthInitialization + 1 + i] = strForCondition[i];
        }
        for (int i = 0; i < lengthIncrement; ++i) {
            temp[positionOfFirstParenthesis + lengthInitialization + lengthCondition + 1 + i] = strForIncrement[i];
        }
        for (int i = 0; i < psnCurrent->size - positionOfSecondParenthesis; ++i) {
            temp[positionOfFirstParenthesis + lengthInitialization + lengthCondition + lengthIncrement + 1 + i] = psnCurrent->line[positionOfSecondParenthesis + i];
        }
        temp[positionOfFirstParenthesis + lengthInitialization + lengthCondition + lengthIncrement + psnCurrent->size - positionOfSecondParenthesis + 1] = '\0';
        psnCurrent->size = psnCurrent->size + lengthInitialization + lengthCondition + lengthIncrement;

        SENT_NODE psnTemp;
        memset(&psnTemp, 0, sizeof(SENT_NODE));
        psnTemp.line = temp;
        psnTemp.size = psnCurrent->size;

        status = deleteSpecificNode(list, nodeWithWhilePosition - 1);
        if (status != 0) {
            printf("Error in changeWhileLoopsToForLoops...\n");
        } else {
            status = addAtIndex(list, &psnTemp, nodeWithWhilePosition - 1);
        }

        free(temp);
    }

    return 0;
}

int findIncrement(NODE_LIST *list, int *nodePosition, char *strForIncrement){
    NODE *psnCurrent = list->pHead;
    NODE *psnPrev = NULL;

    int foundEndOfWhile = 0;
    int positionOfWhileEnd = 0;
    int iStatus = 0;

    int iCommentPosition = 0;
    for (int i = 0; i < *nodePosition + 1; ++i) {
        psnPrev = psnCurrent;
        psnCurrent = psnCurrent->pNextNode;
    }
    printf("For body: %s\n", psnCurrent->line);
    while (psnCurrent != NULL) {
        iCommentPosition = checkIfLineHasComment(psnCurrent->line, psnCurrent->size);

        for (int j = 0; j < (iCommentPosition == 0 ? psnCurrent->size : iCommentPosition); j++) {
            if (psnCurrent->line[j] == '}') {
                positionOfWhileEnd = *nodePosition;
                foundEndOfWhile = 1;
                break;
            }
        }
        if (foundEndOfWhile) {
            break;
        }
        psnPrev = psnCurrent;
        *nodePosition = *nodePosition + 1;
        psnCurrent = psnCurrent->pNextNode;
    }
    if (foundEndOfWhile) {
        int i = 0;
        for (i = 0; i < psnPrev->size; ++i) {
            if (('a' <= psnPrev->line[i] && psnPrev->line[i] <= 'z') ||
                ('A' <= psnPrev->line[i] && psnPrev->line[i] <= 'Z')) {
                break;
            }
        }
        int currentLength = i;
        strForIncrement[0] = ' ';
        for (int j = 0; j < psnPrev->size - currentLength; j++) {
            if (psnPrev->line[i] == ';') {
                break;
            }
            strForIncrement[j + 1] = psnPrev->line[i];
            i++;
        }
        iStatus = deleteSpecificNode(list, positionOfWhileEnd);
        if (iStatus != 0) {
            printf("Error in findIncrement...\n");
            return -1;
        }
        printf("For increment: %s\n", strForIncrement);
    }
    return 0;
}

int findInitialization(NODE_LIST *list, int nodeWithWhilePosition, int nodePosition, char *strForInitialization){
    int iStatus = 0;
    int i = 0;

    NODE *psnCurrent = list->pHead;
    for (i = 0; i < nodePosition - 1; ++i) {
        psnCurrent = psnCurrent->pNextNode;
    }
    for (i = 0; i < psnCurrent->size; ++i) {
        if (('a' <= psnCurrent->line[i] && psnCurrent->line[i] <= 'z') ||
            ('A' <= psnCurrent->line[i] && psnCurrent->line[i] <= 'Z')) {
            break;
        }
    }
    int currentLength = i;
    for (int j = 0; j < psnCurrent->size - currentLength; j++) {
        strForInitialization[j] = psnCurrent->line[i];
        if (psnCurrent->line[i] == ';') {
            break;
        }
        i++;
    }
    iStatus = deleteSpecificNode(list, nodeWithWhilePosition - 1);

    if (iStatus != 0) {
        printf("Error in findInitialization...\n");
        return -1;
    }

    printf("For initialization: %s\n", strForInitialization);
    return 0;
}

int findCondition(int *placementOfFor, int *nodeWithWhilePosition, int *nodePosition, NODE_LIST *list, char *strForCondition, int *foundWhile) {
    NODE *psnCurrent = list->pHead;
    char *pszWhile = "while";
    int iSizeOfWhile = 5;
    int iSizeOfFor = 3;
    int iCommentPosition = 0;
    int status = 0;

    while (psnCurrent != NULL) {
        iCommentPosition = 0;
        char *currentLine = psnCurrent->line;

        // Check if line contains comment
        iCommentPosition = checkIfLineHasComment(currentLine, psnCurrent->size);

        // Loop though string in node until comment or end of line
        for (int j = 0; j < (iCommentPosition == 0 ? psnCurrent->size : iCommentPosition); j++) {
            // Check if the line contains "while", if it does, change position to possible start of variable
            if (strncmp(&currentLine[j], pszWhile, iSizeOfWhile) == 0) {
                status = changeWhileToFor(psnCurrent, j, *nodePosition, list);
                if (status != 0) {
                    printf("Error in changeWhileToFor...\n");
                    return -1;
                }
                *placementOfFor = j;
                *nodeWithWhilePosition = *nodePosition;
                *foundWhile = 1;
                j += iSizeOfFor;
                // Exclude spaces
                while (currentLine[j] == ' ') {
                    if (currentLine[j] == '(') {
                        break;
                    }
                    j++;
                }
                // Check if the line contains "(", if it does, change position to possible start of variable
                strForCondition[0] = ' ';
                if (currentLine[j] == '(') {
                    j++;
                    int k = 0;
                    while (currentLine[j] != ')') {
                        strForCondition[k + 1] = currentLine[j];
                        k++;
                        j++;
                    }
                    strForCondition[k] = ';';
                }
                printf("For condition 2: %s\n", strForCondition);
                break;
            }
        }
        if (*foundWhile) {
            break;
        }
        *nodePosition = *nodePosition + 1;
        psnCurrent = psnCurrent->pNextNode;
    }
    return 0;
}

int changeWhileToFor(NODE *psnCurrent, int positionOfWhile, int nodePosition, NODE_LIST *list) {
    char *strFor = "for";
    int iSizeOfFor = 3;
    int iSizeOfWhile = 5;
    int iStatus = 0;

    char *temp = (char *) malloc(psnCurrent->size + iSizeOfFor - iSizeOfWhile);
    if (temp == NULL) {
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return - 1;
    }
    for (int k = 0; k < positionOfWhile; ++k) {
        temp[k] = psnCurrent->line[k];
    }
    for (int k = 0; k < iSizeOfFor; ++k) {
        temp[positionOfWhile + k] = strFor[k];
    }
    for (int k = positionOfWhile + iSizeOfFor; k < psnCurrent->size; k++) {
        temp[k] = psnCurrent->line[k + iSizeOfWhile - iSizeOfFor];
    }
    psnCurrent->size = psnCurrent->size - iSizeOfWhile + iSizeOfFor;

    SENT_NODE psnTemp;
    memset(&psnTemp, 0, sizeof(SENT_NODE));
    psnTemp.line = temp;
    psnTemp.size = psnCurrent->size;

    iStatus = deleteSpecificNode(list, nodePosition);

    if (iStatus != 0) {

    } else {
        iStatus = addAtIndex(list, &psnTemp, nodePosition);
    }
    free(temp);

    if (iStatus != 0) {
        printf("Error in changeWhileToFor...\n");
        return -1;
    }
    return 0;
}

int checkIfLineHasComment(char *currentLine, int size) {
    for (int j = 0; j < size; j++) {
        if (currentLine[j] == '/' && currentLine[j + 1] == '/') {
            return j;
        }
    }
    return 0;
}


int changeAllCharVariableNamesToHungerianNotation(NODE_LIST *list) {
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
    int iStatus = 0;

    int iLengthOfOldVariable = 0;
    int iLengthOfNewVariable = 0;
    int iLengthOfNewLine = 0;
    int iCurrentLengthOfNewVariable = 0;
    int iRemainingSize = 0;

    int iCommentPosition = 0;

    memset(strOldVariable, 0, MAX_STRING_LENGTH);
    memset(strUpgradeOldVariable, 0, MAX_STRING_LENGTH);
    memset(strNewLine, 0, MAX_STRING_LENGTH);
    memset(strNewVariable, 0, MAX_STRING_LENGTH);
    // Loop through all lines in the list to find char, and check if they are using hungerian notation
    while (psnCurrent != NULL) {
        char *currentLine = psnCurrent->line;
        iCommentPosition = 0;
        // Loop though string in node
        iCommentPosition = checkIfLineHasComment(currentLine, psnCurrent->size);

        // Loop though string in node until comment or end of line
        for (int j = 0; j < (iCommentPosition == 0 ? psnCurrent->size : iCommentPosition); j++) {
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
                strNewVariable[strlen(pszVariableStart)] = '\0';
                iCurrentLengthOfNewVariable = strlen(strNewVariable);
                iRemainingSize = MAX_STRING_LENGTH - iCurrentLengthOfNewVariable;
                strncat(strNewVariable, strOldVariable, iRemainingSize);
                strNewVariable[strlen(strNewVariable)] = '\0';

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
                    return - 1;
                }
                strncpy(pszTempString, strNewLine, iLengthOfNewLine);
                pszTempString[iLengthOfNewLine] = '\0';

                SENT_NODE psnTemp;
                memset(&psnTemp, 0, sizeof(SENT_NODE));
                psnTemp.line = pszTempString;
                psnTemp.size = iLengthOfNewLine;

                iStatus = deleteSpecificNode(list, iListPosition);

                if (iStatus != 0) {
                    printf("Error in changeAllCharVariableNamesToHungerianNotation...\n");
                    return -1;
                }

                iStatus = addAtIndex(list, &psnTemp, iListPosition);

                if (iStatus != 0) {
                    printf("Error in changeAllCharVariableNamesToHungerianNotation...\n");
                    return -1;
                }

                free(pszTempString);

                printf("Old variable: %s\n", strOldVariable);
                printf("New variable: %s\n", strNewVariable);
                printf("\n");

            }
        }
        iListPosition++;
        psnCurrent = psnCurrent->pNextNode;
    }
    return 0;
}

int removeEveryConcurrentlyTreeLinesOfSpace(NODE_LIST *psnList) {
    NODE *psnCurrent = psnList->pHead;
    int currentNode = 0;
    int status = 0;

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
        char *temp = (char *) malloc(newSize + 1);
        if (temp == NULL) {
            printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
            return - 1;
        }
        memset(temp, 0, newSize + 1);
        for (int j = 0; j < newSize; j++) {
            temp[j] = psnCurrentLine[j];
        }
        temp[newSize] = '\0';

        SENT_NODE psnTemp;
        memset(&psnTemp, 0, sizeof(SENT_NODE));
        psnTemp.line = temp;
        psnTemp.size = newSize;

        status = deleteSpecificNode(psnList, currentNode);

        if (status != 0) {
            printf("Error in removeEveryConcurrentlyTreeLinesOfSpace...\n");
            return -1;
        }

        status = addAtIndex(psnList, &psnTemp, currentNode);

        if (status != 0) {
            printf("Error in removeEveryConcurrentlyTreeLinesOfSpace...\n");
            return -1;
        }

        free(temp);

        psnCurrent = psnCurrent->pNextNode;
        currentNode++;
    }
    return 0;
}