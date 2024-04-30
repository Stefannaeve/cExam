#include <stdio.h>
#include "../include/beautifier.h"
#include <string.h>

int handleCurrentWord(char *currentChar, char *currentWord, int *currentWordPosition);

void beautify(char *path) {
    char currentWord[1024];
    char lastWord[1024];
    char completeFile[1024];
    char updatedFile[1024];

    char *type[2] = {"int", "char"};

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
            printf("%s\n", currentWord);
            for (int j = 0; j < 2; ++j) {
                if (strcmp(currentWord, type[j]) == 0) {
                    printf("Found type: %s\n", currentWord);
                }
            }
            memset(currentWord, 0, 1024);
            if (i > 200) {
                break;
            }
        }
        i++;
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
    return 1;
}