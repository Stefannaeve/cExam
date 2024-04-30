#include <stdio.h>
#include "../include/beautifier.h"
#include <string.h>

#define MAX_WORD_LENGTH 512
#define TYPE_ARRAY_LENGTH 2
#define TRUE 1
#define FALSE 0

int handleCurrentWord(char *currentChar, char *currentWord, int *currentWordPosition);
void checkIfWordIsType(char *currentWord, char *type[], int *lastWordIsType);
void handleVariable(char *currentWord, char *lastWord, char oldVariables[10][512], char newVariables[10][512], int *variablesFound);

void beautify(char *path) {
    char currentWord[MAX_WORD_LENGTH];
    char lastWord[MAX_WORD_LENGTH];
    char completeFile[MAX_WORD_LENGTH];
    char updatedFile[MAX_WORD_LENGTH];

    int lastWordIsType = FALSE;
    int insideParentheses = FALSE;
    int insideCurlyBrackets = FALSE;
    int variablesFound = 0;

    char oldVariables[10][MAX_WORD_LENGTH];
    char newVariables[10][MAX_WORD_LENGTH];

    memset(oldVariables, 0, 10 * MAX_WORD_LENGTH);
    memset(newVariables, 0, 10 * MAX_WORD_LENGTH);

    char *type[TYPE_ARRAY_LENGTH] = {"int", "char"};

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", path);
        return;
    }

    fread(completeFile, 1, MAX_WORD_LENGTH, file);
    int position = ftell(file);
    completeFile[position] = '\0';
    printf("%s\n", completeFile);
    fclose(file);

    int i = 0;
    int currentWordPosition = 0;
    while (completeFile[i] != '\0') {
        handleCurrentWord(&completeFile[i], currentWord, &currentWordPosition);
        if (currentWordPosition == 0 && strlen(currentWord) > 0) {
            if (lastWordIsType == TRUE) {
                printf("Found variable: %s\n", currentWord);
                handleVariable(currentWord, lastWord, oldVariables, newVariables, &variablesFound);
                lastWordIsType = FALSE;
            }
            printf("%s\n", currentWord);
            checkIfWordIsType(currentWord, type, &lastWordIsType);
            strncpy(lastWord, currentWord, MAX_WORD_LENGTH);
            memset(currentWord, 0, MAX_WORD_LENGTH);
            if (i > 200) {
                break;
            }
        }
        i++;
    }
    printf("\n");
    printf("Hallo?\n");
    for (int j = 0; j < variablesFound; ++j) {
        printf("Old variable: %s\n", oldVariables[j]);
        printf("New variable: %s\n\n", newVariables[j]);
    }
}

void handleVariable(char *currentWord, char *lastWord, char oldVariables[10][MAX_WORD_LENGTH], char newVariables[10][MAX_WORD_LENGTH], int *variablesFound){
    char newVariable[MAX_WORD_LENGTH];
    char oldVariable[MAX_WORD_LENGTH];
    strncpy(oldVariable, currentWord, MAX_WORD_LENGTH);
    memset(newVariable, 0, MAX_WORD_LENGTH);
    char *integer = "i";
    char *character = "*sz";
    if (strcmp(lastWord, "int") == 0) {
        if (currentWord[0] != 'i'){
            if (97 <= (int)currentWord[0] && (int)currentWord[0] <= 122){
                currentWord[0] = (char)((int)currentWord[0] - 32);
            }
            strcat(newVariable, integer);
        }
    }
    if (strcmp(lastWord, "char") == 0) {
        if (currentWord[0] != 's'){
            if (currentWord[0] == '*') {
                // Remove * from current word
                for (int i = 0; i < strlen(currentWord); ++i) {
                    currentWord[i] = currentWord[i + 1];
                }
                if (97 <= (int) currentWord[0] && (int) currentWord[0] <= 122) {
                    currentWord[0] = (char) ((int) currentWord[0] - 32);
                }
            }
        }
        strcat(newVariable, character);
    }
    strcat(newVariable, currentWord);

    strncpy(oldVariables[*variablesFound], oldVariable, MAX_WORD_LENGTH);
    strncpy(newVariables[*variablesFound], newVariable, MAX_WORD_LENGTH);

    *variablesFound = *variablesFound + 1;

    printf("Updated variable: %s\n", newVariable);
    // Make current word into new variable
    memset(currentWord, 0, MAX_WORD_LENGTH);
    strncpy(currentWord, newVariable, MAX_WORD_LENGTH);
}

void checkIfWordIsType(char *currentWord, char *type[], int *lastWordIsType) {
    for (int i = 0; i < TYPE_ARRAY_LENGTH; ++i) {
        if (strcmp(currentWord, type[i]) == 0) {
            *lastWordIsType = TRUE;
            printf("Found type: %s\n", currentWord);
        }
    }
}

int handleCurrentWord(char *currentChar, char *currentWord, int *currentWordPosition) {
    int asciiValue = (int) *currentChar;
    if ((33 <= asciiValue && asciiValue <= 122)) {
        currentWord[*currentWordPosition] = *currentChar;
        *currentWordPosition = *currentWordPosition + 1;
    } else {
        currentWord[*currentWordPosition] = '\0';
        *currentWordPosition = 0;
        return 0;
    }

    if (*currentWordPosition == MAX_WORD_LENGTH) {
        currentWord[*currentWordPosition] = '\0';
        *currentWordPosition = 0;
        return 0;
    }
    return 1;
}