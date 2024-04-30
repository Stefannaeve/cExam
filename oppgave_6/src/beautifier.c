#include <stdio.h>
#include "../include/beautifier.h"
#include <string.h>

#define MAX_WORD_LENGTH 1024
#define TYPE_ARRAY_LENGTH 2
#define TRUE 1
#define FALSE 0

int handleCurrentWord(char *currentChar, char *currentWord, int *currentWordPosition);
void checkIfWordIsType(char *currentWord, char *type[], int *lastWordIsType);

void beautify(char *path) {
    char currentWord[MAX_WORD_LENGTH];
    char lastWord[MAX_WORD_LENGTH];
    char completeFile[MAX_WORD_LENGTH];
    char updatedFile[MAX_WORD_LENGTH];
    int lastWordIsType = FALSE;

    char *type[TYPE_ARRAY_LENGTH] = {"int", "char"};

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", path);
        return;
    }

    fread(completeFile, 1, 1024, file);
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
                lastWordIsType = FALSE;
            }
            printf("%s\n", currentWord);
            checkIfWordIsType(currentWord, type, &lastWordIsType);
            strncpy(lastWord, currentWord, 1024);
            memset(currentWord, 0, 1024);
            if (i > 200) {
                break;
            }
        }
        i++;
    }
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

    if (*currentWordPosition == 1024) {
        currentWord[*currentWordPosition] = '\0';
        *currentWordPosition = 0;
        return 0;
    }
    return 1;
}