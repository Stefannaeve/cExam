#include "../include/beautifier.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <malloc.h>

int beautify(char *filename) {
    char strBuffer[MAX_STRING_LENGTH];
    char *pszTempChar;

    int iLength = 0;
    int i = 0;
    int iStatus = 0;

    NODE_LIST snList = {NULL, NULL, 0};
    SENT_NODE snTemp = {NULL, 0};
    NODE *psnCurrent = NULL;

    FILE *pfdBeautifiedFile;
    FILE *pfdCFile = fopen(filename, "r");
    if (pfdCFile == NULL) {
        printf("Could not open pfdCFile %s - Error message: %s\n", filename, strerror(errno));
        return -1;
    }

    // Read file and add each line to a linked list
    while (fgets(strBuffer, MAX_STRING_LENGTH, pfdCFile) != NULL) {
        iLength = strlen(strBuffer);

        pszTempChar = (char *) malloc(iLength + 1);
        if (pszTempChar == NULL) {
            iStatus = -1;
            printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
            break;
        }
        memset(pszTempChar, 0, iLength + 1);

        pszTempChar[iLength] = '\0';
        strncpy(pszTempChar, strBuffer, iLength);
        pszTempChar[iLength] = '\0';

        memset(&snTemp, 0, sizeof(SENT_NODE));
        snTemp.line = pszTempChar;
        snTemp.size = iLength;

        add(&snList, &snTemp);

        free(pszTempChar);
        memset(strBuffer, 0, MAX_STRING_LENGTH);

        i++;
    }

    if (iStatus != 0) {

    } else {
        iStatus = fclose(pfdCFile);
        if (iStatus != 0) {
            printf("Could not close pfdCFile - Error message: %s\n", strerror(errno));
        } else {

            // Remove every three spaces in a row and replace with \t
            iStatus = removeEveryConcurrentlyTreeLinesOfSpace(&snList);

            if (iStatus != 0) {
                printf("Error in Removing spaces for tabs...\n");
            } else {

                printAllNodes(&snList);

                // Change all char variable names to hungerian notation
                iStatus = changeAllCharVariableNamesToHungerianNotation(&snList);

                if (iStatus != 0) {
                    printf("Error in Hungerian notation changer...\n");
                } else {

                    // Change while loop name and condition into a for loop
                    iStatus = changeWhileLoopsToForLoops(&snList);

                    if (iStatus != 0) {
                        printf("Error in changing while loops...\n");
                    } else {

                        printAllNodes(&snList);
                        // Open file to write beautified code to
                        pfdBeautifiedFile = fopen("src/beautified_oppgave6_test.c", "w");


                        if (pfdBeautifiedFile == NULL) {
                            printf("Could not open pfdBeautifiedFile - Error message: %s\n", strerror(errno));
                        } else {

                            psnCurrent = snList.pHead;
                            while (psnCurrent != NULL) {
                                fprintf(pfdBeautifiedFile, "%s", psnCurrent->line);
                                psnCurrent = psnCurrent->pNextNode;
                            }

                            iStatus = fclose(pfdBeautifiedFile);
                            if (iStatus != 0) {
                                printf("Could not close pfdBeautifiedFile - Error message: %s\n", strerror(errno));
                            }
                        }
                    }
                }
            }
        }
    }

    freeLinkedList(&snList);

    if (iStatus != 0) {
        return -1;
    }

    return 0;
}

int changeWhileLoopsToForLoops(NODE_LIST *list) {
    int iNodeWithWhilePosition = 0;
    int iNodePosition = 0;
    int iFoundWhile = 0;
    int iPlacementOfFor = 0;
    int iStatus = 0;
    int sizeOfList = list->size;

    int iLengthInitialization = 0;
    int iLengthCondition = 0;
    int iLengthIncrement = 0;

    int iPositionOfFirstParenthesis = 0;
    int iPositionOfSecondParenthesis = 0;

    char strForInitialization[MAX_STRING_LENGTH] = {0};
    char strForCondition[MAX_STRING_LENGTH] = {0};
    char strForIncrement[MAX_STRING_LENGTH] = {0};

    char *pszTemp = NULL;

    NODE *psnCurrent = NULL;
    SENT_NODE psnTemp = {NULL, 0};

    for (int m = 0; m < sizeOfList; ++m) {

        iPlacementOfFor = 0;
        iNodeWithWhilePosition = 0;
        iNodePosition = 0;
        iFoundWhile = 0;
        memset(strForInitialization, 0, MAX_STRING_LENGTH);
        memset(strForCondition, 0, MAX_STRING_LENGTH);
        memset(strForIncrement, 0, MAX_STRING_LENGTH);

        // Find the condition for the for loop
        iStatus = findCondition(&iPlacementOfFor, &iNodeWithWhilePosition, &iNodePosition, list, strForCondition,
                                &iFoundWhile);
        if (iStatus != 0) {
            printf("Error in findCondition...\n");
            return -1;
        }

        // findCondition will return 0 if it finds a while loop
        // Find the initialization part of the for loop
        if (iFoundWhile) {
            iStatus = findInitialization(list, iNodeWithWhilePosition, iNodePosition, strForInitialization);
        }
        if (iStatus != 0) {
            printf("Error in findInitialization...\n");
            return -1;
        }

        // Find the increment part of the for loop
        if (iFoundWhile) {
            iStatus = findIncrement(list, &iNodePosition, strForIncrement);
        }
        if (iStatus != 0) {
            printf("Error in findIncrement...\n");
            return -1;
        }

        // Find length of the strings
        iLengthInitialization = strlen(strForInitialization);
        iLengthCondition = strlen(strForCondition);
        iLengthIncrement = strlen(strForIncrement);

        // Replace the while loop with a for loop
        if (iFoundWhile) {
            psnCurrent = list->pHead;
            // Find the node with the while loop
            for (int i = 0; i < iNodeWithWhilePosition - 1; ++i) {
                psnCurrent = psnCurrent->pNextNode;
            }
            // start at the position of the "for", and increment until we find the first and second parenthesis
            for (int i = iPlacementOfFor; i < psnCurrent->size; ++i) {
                if (psnCurrent->line[i] == '(') {
                    iPositionOfFirstParenthesis = i;
                }
                if (psnCurrent->line[i] == ')' && iPositionOfFirstParenthesis != 0) {
                    iPositionOfSecondParenthesis = i;
                    break;
                }
            }
            pszTemp = (char *) malloc(
                    iPositionOfFirstParenthesis +1 + iLengthInitialization + iLengthCondition + iLengthIncrement +
                    psnCurrent->size - iPositionOfSecondParenthesis + 1);
            if (pszTemp == NULL) {
                printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
                return -1;
            }
            memset(pszTemp, 0,
                   iPositionOfFirstParenthesis + iLengthInitialization + iLengthCondition + iLengthIncrement +
                   psnCurrent->size - iPositionOfSecondParenthesis + 1);
            pszTemp[iPositionOfFirstParenthesis +1 + iLengthInitialization + iLengthCondition + iLengthIncrement +
                    psnCurrent->size - iPositionOfSecondParenthesis] = '\0';

            // Copy everything before the first parenthesis, which will include the "for"
            for (int i = 0; i < iPositionOfFirstParenthesis + 1; ++i) {
                pszTemp[i] = psnCurrent->line[i];
            }

            // Copy the initialization, condition and increment into the new string
            for (int i = 0; i < iLengthInitialization; ++i) {
                pszTemp[iPositionOfFirstParenthesis + 1 + i] = strForInitialization[i];
            }
            for (int i = 0; i < iLengthCondition; ++i) {
                pszTemp[iPositionOfFirstParenthesis + iLengthInitialization + 1 + i] = strForCondition[i];
            }
            for (int i = 0; i < iLengthIncrement; ++i) {
                pszTemp[iPositionOfFirstParenthesis + iLengthInitialization + iLengthCondition + 1 +
                        i] = strForIncrement[i];
            }

            // Copy the rest of the string after the second parenthesis
            for (int i = 0; i < psnCurrent->size - iPositionOfSecondParenthesis; ++i) {
                pszTemp[iPositionOfFirstParenthesis + iLengthInitialization + iLengthCondition + iLengthIncrement + 1 +
                        i] = psnCurrent->line[iPositionOfSecondParenthesis + i];
            }
            pszTemp[iPositionOfFirstParenthesis + iLengthInitialization + iLengthCondition + iLengthIncrement +
                    psnCurrent->size - iPositionOfSecondParenthesis + 1] = '\0';
            psnCurrent->size = iPositionOfFirstParenthesis +1 + iLengthInitialization + iLengthCondition + iLengthIncrement +
                               psnCurrent->size - iPositionOfSecondParenthesis + 1;

            memset(&psnTemp, 0, sizeof(SENT_NODE));
            psnTemp.line = pszTemp;
            psnTemp.size = psnCurrent->size;

            // Delete the old node and add the new node with the for loop
            iStatus = deleteSpecificNode(list, iNodeWithWhilePosition - 1);
            if (iStatus != 0) {
                printf("Error in changeWhileLoopsToForLoops...\n");
            } else {
                iStatus = addAtIndex(list, &psnTemp, iNodeWithWhilePosition - 1);
            }

            free(pszTemp);
        }
        if (iFoundWhile) {
            printAllNodes(list);
        }
        sizeOfList = list->size;
    }

    return 0;
}

int findIncrement(NODE_LIST *psnList, int *iNodePosition, char *strForIncrement){
    NODE *psnCurrent = psnList->pHead;
    NODE *psnPrev = NULL;

    int iFoundEndOfWhile = 0;
    int iPositionOfWhileEnd = 0;
    int iStatus = 0;
    int iCurrentLength = 0;

    int iCommentPosition = 0;
    for (int i = 0; i < *iNodePosition; ++i) {
        psnPrev = psnCurrent;
        psnCurrent = psnCurrent->pNextNode;
    }

    while (psnCurrent != NULL) {
        // Check if line contains comment, and returns the position of the comment
        iCommentPosition = checkIfLineHasComment(psnCurrent->line, psnCurrent->size);

        // Loop though string in node until comment or end of line
        for (int j = 0; j < (iCommentPosition == 0 ? psnCurrent->size : iCommentPosition); j++) {
            if (psnCurrent->line[j] == '}') {
                iPositionOfWhileEnd = *iNodePosition;
                iFoundEndOfWhile = 1;
                break;
            }
        }
        // If we found the end of the while loop, break the loop
        if (iFoundEndOfWhile) {
            break;
        }
        psnPrev = psnCurrent;
        *iNodePosition = *iNodePosition + 1;
        psnCurrent = psnCurrent->pNextNode;
    }
    if (iFoundEndOfWhile) {
        int i = 0;
        // Find the start of the increment
        for (i = 0; i < psnPrev->size; ++i) {
            if (('a' <= psnPrev->line[i] && psnPrev->line[i] <= 'z') ||
                ('A' <= psnPrev->line[i] && psnPrev->line[i] <= 'Z')) {
                break;
            }
        }
        iCurrentLength = i;
        // Copy the increment into a new string, add '' to the start of the string to look good inside the for loop
        strForIncrement[0] = ' ';
        for (int j = 0; j < psnPrev->size - iCurrentLength; j++) {
            // Break if we find a semicolon
            if (psnPrev->line[i] == ';') {
                break;
            }
            strForIncrement[j + 1] = psnPrev->line[i];
            i++;
        }
        // We delete this since the for loop will have the increment in the for loop
        iStatus = deleteSpecificNode(psnList, iPositionOfWhileEnd-1);
        if (iStatus != 0) {
            printf("Error in findIncrement...\n");
            return -1;
        }
        printf("For increment: %s\n", strForIncrement);
    }
    return 0;
}

int findInitialization(NODE_LIST *psnList, int iNodeWithWhilePosition, int iNodePosition, char *strForInitialization){
    int iStatus = 0;
    int i = 0;
    int iCurrentLength = 0;

    NODE *psnCurrent = psnList->pHead;
    // Find the node before the while loop
    for (i = 0; i < iNodePosition - 1; ++i) {
        psnCurrent = psnCurrent->pNextNode;
    }
    // Find the start of the initialization
    for (i = 0; i < psnCurrent->size; ++i) {
        if (('a' <= psnCurrent->line[i] && psnCurrent->line[i] <= 'z') ||
            ('A' <= psnCurrent->line[i] && psnCurrent->line[i] <= 'Z')) {
            break;
        }
    }
    iCurrentLength = i;
    // Copy the initialization into a new string, break if we find a semicolon
    for (int j = 0; j < psnCurrent->size - iCurrentLength; j++) {
        strForInitialization[j] = psnCurrent->line[i];
        if (psnCurrent->line[i] == ';') {
            break;
        }
        i++;
    }
    // Delete the node with the initialization, since the for loop will have the initialization
    iStatus = deleteSpecificNode(psnList, iNodeWithWhilePosition - 1);

    if (iStatus != 0) {
        printf("Error in findInitialization...\n");
        return -1;
    }

    printf("For initialization: %s\n", strForInitialization);
    return 0;
}

int findCondition(int *piPlacementOfFor, int *piNodeWithWhilePosition, int *piNodePosition, NODE_LIST *psnList, char *pstrForCondition, int *piFoundWhile) {
    NODE *psnCurrent = psnList->pHead;
    char *pszWhile = "while";
    char *pszCurrentLine;
    int iSizeOfWhile = 5;
    int iSizeOfFor = 3;
    int iCommentPosition = 0;
    int iStatus = 0;

    int iLengthOfCurrentLine = 0;

    while (psnCurrent != NULL) {
        iCommentPosition = 0;
        pszCurrentLine = psnCurrent->line;

        // Check if line contains comment
        iCommentPosition = checkIfLineHasComment(pszCurrentLine, psnCurrent->size);

        // Check the length of the line with or without comment
        // Change the node if the length is less then size of while
        if (iCommentPosition == 0) {
            iLengthOfCurrentLine = psnCurrent->size;
        } else {
            iLengthOfCurrentLine = iCommentPosition;
        }

        if (iLengthOfCurrentLine > iSizeOfWhile) {

            // Loop though string in node until comment or end of line
            for (int j = 0; j < (iCommentPosition == 0 ? psnCurrent->size - iSizeOfWhile : iCommentPosition - iSizeOfWhile); j++) {
                // Check if the line contains "while", if it does, change position to possible start of variable

                if (strncmp(&pszCurrentLine[j], pszWhile, iSizeOfWhile) == 0) {
                    iStatus = changeWhileToFor(psnCurrent, j, *piNodePosition, psnList);
                    if (iStatus != 0) {
                        printf("Error in changeWhileToFor...\n");
                        return -1;
                    }
                    // Get back to the same position, with the new node
                    psnCurrent = psnList->pHead;
                    for (int i = 0; i < *piNodePosition; ++i) {
                        psnCurrent = psnCurrent->pNextNode;
                    }
                    pszCurrentLine = psnCurrent->line;
                    *piPlacementOfFor = j;
                    *piNodeWithWhilePosition = *piNodePosition;
                    *piFoundWhile = 1;
                    j += iSizeOfFor;
                    // Exclude spaces
                    while (pszCurrentLine[j] == ' ' || pszCurrentLine[j] == '\t') {
                        if (pszCurrentLine[j] == '(') {
                            break;
                        }
                        j++;
                    }
                    // Check if the line contains "(", if it does, change position to possible start of variable
                    pstrForCondition[0] = ' ';
                    // It is expected that this position is the start of the "(", read from this position until we find the end of the condition
                    if (pszCurrentLine[j] == '(') {
                        j++;
                        int k = 0;
                        // Copy the condition into a new string, stop when we find the end of the condition
                        while (pszCurrentLine[j] != ')') {
                            pstrForCondition[k + 1] = pszCurrentLine[j];
                            k++;
                            j++;
                        }
                        // Add a semicolon to the end of the condition to make it make sense in the for loop
                        pstrForCondition[k] = ';';
                    }
                    break;
                }
            }
        }
        // If we found the while loop, break the loop
        if (*piFoundWhile) {
            break;
        }
        *piNodePosition = *piNodePosition + 1;
        psnCurrent = psnCurrent->pNextNode;
    }
    return 0;
}

int changeWhileToFor(NODE *psnCurrent, int iPositionOfWhile, int iNodePosition, NODE_LIST *psnList) {
    char *strFor = "for";
    int iSizeOfFor = 3;
    int iSizeOfWhile = 5;
    int iStatus = 0;
    int sizeOfTemp = 0;

    SENT_NODE psnTemp = {NULL, 0};

    sizeOfTemp = psnCurrent->size + iSizeOfFor - iSizeOfWhile;

    char *pszTemp = (char *) malloc(sizeOfTemp + 1);
    if (pszTemp == NULL) {
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return - 1;
    }
    memset(pszTemp, 0, sizeOfTemp);
    pszTemp[sizeOfTemp] = '\0';

    // Copy everything before the while loop, add "for" instead of "while"
    for (int k = 0; k < iPositionOfWhile; ++k) {
        pszTemp[k] = psnCurrent->line[k];
    }
    // Copy "for" into the new string
    for (int k = 0; k < iSizeOfFor; ++k) {
        pszTemp[iPositionOfWhile + k] = strFor[k];
    }
    // Copy the rest of the string after the while loop
    for (int k = iPositionOfWhile + iSizeOfFor; k < sizeOfTemp; k++) {
        pszTemp[k] = psnCurrent->line[k + iSizeOfWhile - iSizeOfFor];
    }
    pszTemp[psnCurrent->size + iSizeOfFor - iSizeOfWhile] = '\0';
    psnCurrent->size = sizeOfTemp;

    memset(&psnTemp, 0, sizeof(SENT_NODE));
    psnTemp.line = pszTemp;
    psnTemp.line[psnCurrent->size] = '\0';
    psnTemp.size = psnCurrent->size;

    // Delete the old node and add the new node with the for loop
    iStatus = deleteSpecificNode(psnList, iNodePosition);

    if (iStatus != 0) {

    } else {
        iStatus = addAtIndex(psnList, &psnTemp, iNodePosition);
    }
    free(pszTemp);

    if (iStatus != 0) {
        printf("Error in changeWhileToFor...\n");
        return -1;
    }
    return 0;
}

int checkIfLineHasComment(char *pszCurrentLine, int iSize) {
    // Loop through until we find two slashes, which indicates a comment
    if (iSize < 2) {
        return 0;
    }
    if (iSize == 34){
        printf("%s\n" , pszCurrentLine);
    }
    for (int j = 0; j < iSize-1; j++) {
        //printf("size: %d, j: %d\n", iSize, j+1);
        if (pszCurrentLine[j] == '/' && pszCurrentLine[j + 1] == '/') {
            return j;
        }
    }
    return 0;
}


int changeAllCharVariableNamesToHungerianNotation(NODE_LIST *psnList) {
    NODE *psnCurrent = psnList->pHead;
    SENT_NODE psnTemp = {NULL, 0};
    char *pszVariableStart = "*psz";
    char strOldVariable[MAX_STRING_LENGTH];
    char strUpgradeOldVariable[MAX_STRING_LENGTH];
    char strNewLine[MAX_STRING_LENGTH];
    char strNewVariable[MAX_STRING_LENGTH];
    char *pszTempString;
    char *pszCurrentLine;

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
    // Loop through all lines in the psnList to find char, and check if they are using hungerian notation
    while (psnCurrent != NULL) {
        pszCurrentLine = psnCurrent->line;
        iCommentPosition = 0;
        // Loop though string in node
        iCommentPosition = checkIfLineHasComment(pszCurrentLine, psnCurrent->size);

        // Loop though string in node until comment or end of line
        for (int j = 0; j < (iCommentPosition == 0 ? psnCurrent->size : iCommentPosition); j++) {
            // Check if the line contains "char ", if it does, change position to possible start of variable
            if (strncmp(&pszCurrentLine[j], "char ", iSizeOfCharOffset) == 0) {
                j += iSizeOfCharOffset;
                iLinePositionForVariable = j;
                // Exclude spaces
                while (pszCurrentLine[j] == ' ') {
                    j++;
                }
                i = 0;
                // Find the end of the variable
                while (pszCurrentLine[j] != ' ' && pszCurrentLine[j] != ';') {
                    strOldVariable[i++] = pszCurrentLine[j++];
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
                    strNewLine[k] = pszCurrentLine[k];
                }
                // Add new variable to new line
                for (int k = 0; k < iLengthOfNewVariable; ++k) {
                    strNewLine[iLinePositionForVariable + k] = strNewVariable[k];
                }
                int oldPositionAfterVariable = iLinePositionForVariable + iLengthOfOldVariable + 1;
                // Add the rest of the old code after the variable to the new line
                for (int k = iLinePositionForVariable + iLengthOfNewVariable;
                     k < psnCurrent->size + (iLengthOfNewVariable -
                                             iLengthOfOldVariable); ++k) {
                    strNewLine[k] = pszCurrentLine[oldPositionAfterVariable++];
                }

                iLengthOfNewLine = strlen(strNewLine);

                pszTempString = (char *) malloc(iLengthOfNewLine + 1);
                if (pszTempString == NULL) {
                    printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
                    return - 1;
                }
                memset(pszTempString, 0, iLengthOfNewLine);
                strncpy(pszTempString, strNewLine, iLengthOfNewLine);
                pszTempString[iLengthOfNewLine] = '\0';

                memset(&psnTemp, 0, sizeof(SENT_NODE));
                psnTemp.line = pszTempString;
                psnTemp.line[iLengthOfNewLine] = '\0';
                psnTemp.size = iLengthOfNewLine;

                iStatus = deleteSpecificNode(psnList, iListPosition);

                if (iStatus != 0) {
                    printf("Error in changeAllCharVariableNamesToHungerianNotation...\n");
                    free(pszTempString);
                    return -1;
                }

                iStatus = addAtIndex(psnList, &psnTemp, iListPosition);

                if (iStatus != 0) {
                    printf("Error in changeAllCharVariableNamesToHungerianNotation...\n");
                    free(pszTempString);
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
    SENT_NODE psnTemp = {NULL, 0};
    int iCurrentNode = 0;
    int iStatus = 0;
    int iNewSize = 0;

    char *psnCurrentLine = NULL;
    char *pszTemp = NULL;

    while (psnCurrent != NULL) {
        psnCurrentLine = psnCurrent->line;
        iNewSize = 0;
        for (int j = 0; j < psnCurrent->size; j++) {
            // Check if there are three spaces in a row, if so replace with \t
            if (j < psnCurrent->size - 2 && psnCurrentLine[j] == ' ' && psnCurrentLine[j + 1] == ' ' &&
                psnCurrentLine[j + 2] == ' ') {
                psnCurrentLine[iNewSize++] = '\t';
                // Skip two spaces
                j += 2;
            } else {
                psnCurrentLine[iNewSize++] = psnCurrentLine[j];
            }
        }
        pszTemp = (char *) malloc(iNewSize + 1);
        if (pszTemp == NULL) {
            printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
            return - 1;
        }
        memset(pszTemp, 0, iNewSize + 1);
        pszTemp[iNewSize] = '\0';
        for (int j = 0; j < iNewSize; j++) {
            pszTemp[j] = psnCurrentLine[j];
        }
        pszTemp[iNewSize] = '\0';

        memset(&psnTemp, 0, sizeof(SENT_NODE));
        psnTemp.line = pszTemp;
        psnTemp.size = iNewSize;

        // Remove old node and add new node with replaced spaces
        iStatus = deleteSpecificNode(psnList, iCurrentNode);

        if (iStatus != 0) {
            printf("Error in removeEveryConcurrentlyTreeLinesOfSpace...\n");
            free(pszTemp);
            return -1;
        }

        iStatus = addAtIndex(psnList, &psnTemp, iCurrentNode);

        if (iStatus != 0) {
            printf("Error in removeEveryConcurrentlyTreeLinesOfSpace...\n");
            free(pszTemp);
            return -1;
        }

        free(pszTemp);

        psnCurrent = psnCurrent->pNextNode;
        iCurrentNode++;
    }
    return 0;
}