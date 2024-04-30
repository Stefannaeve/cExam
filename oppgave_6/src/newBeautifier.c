#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define TYPE_ARRAY_LENGTH 2

#define MAX_STRING_LENGTH 512

typedef struct _WORD {
    int wordsIsType;
    int wordsIsVariable;
    int wordsIsDone;

    int specialCharacters;
    int letterCharacters;
    int spaceCharacters;

    int currentCharIsLetter;

    int wordSize;

    char currentWord[];
} WORD;

void changeToNewWord(WORD *currentWord, char *currentChar);

void handleVariableDismissFunction(WORD *currentWord, char *lastWord, char oldVariables[10][MAX_STRING_LENGTH],
                                   char newVariables[10][MAX_STRING_LENGTH], int *variablesFound);

int handleCurrentWord(WORD *currentWord, char *currentChar);

void checkIfWordIsType(char *currentWord, char *type[], int *lastWordIsType);

void beautify1(char *path) {
    char lastWord[MAX_STRING_LENGTH];
    char completeFile[MAX_STRING_LENGTH];
    char updatedFile[MAX_STRING_LENGTH];

    char buildForLoop[MAX_STRING_LENGTH];
    char forCondition[MAX_STRING_LENGTH];
    char forCondition1[MAX_STRING_LENGTH];
    char forCondition2[MAX_STRING_LENGTH];
    char forCondition3[MAX_STRING_LENGTH];
    char forBody[MAX_STRING_LENGTH];

    char oldVariables[10][MAX_STRING_LENGTH];
    char newVariables[10][MAX_STRING_LENGTH];

    int insideCurlyBrackets = FALSE;
    int insideWhileCurlyBrackets = FALSE;
    int insideParentheses = FALSE;

    int lastWordReadyForProcessing = FALSE;
    int lastWordIsType = FALSE;
    int lastWordIsVariable = FALSE;
    int lastWordIsWhile = FALSE;
    int lastCharIsLetter = FALSE;
    int currentCharIsLetter = FALSE;

    int variablesFound = 0;
    int currentWordPosition = 0;

    int specialCharacters = 0;
    int letterCharacters = 0;
    int spaceCharacters = 0;

    char currentChar;
    char lastChar;
    char *type[TYPE_ARRAY_LENGTH] = {"int", "char"};

    WORD *currentWord = malloc(sizeof(WORD) + MAX_STRING_LENGTH);
    memset(currentWord, 0, sizeof(WORD) + MAX_STRING_LENGTH);
    currentWord->wordsIsType = FALSE;
    currentWord->wordsIsVariable = FALSE;
    currentWord->wordsIsDone = FALSE;
    currentWord->specialCharacters = 0;
    currentWord->letterCharacters = 0;
    currentWord->spaceCharacters = 0;
    currentWord->currentCharIsLetter = FALSE;
    currentWord->wordSize = 0;
    currentWord->currentWord[MAX_STRING_LENGTH - 1] = '\0';


    FILE *cFile = fopen(path, "r");
    if (cFile == NULL) {
        printf("Could not open cFile %s\n", path);
        return;
    }

    fread(completeFile, 1, MAX_STRING_LENGTH, cFile);
    int position = ftell(cFile);
    completeFile[position] = '\0';
    printf("%s\n", completeFile);
    fclose(cFile);

    for (int i = 0; i < position; ++i) {
        currentChar = completeFile[i];

        int asciiSmallLettersBeginning = 'a';
        int asciiSmallLettersEnd = 'z';

        int asciiBigLettersBeginning = 'A';
        int asciiBigLettersEnd = 'Z';

        changeToNewWord(currentWord, &currentChar);

        if (currentWord->wordsIsDone) {
            if (currentWord->wordSize > 0) {
                if (lastWordIsType == TRUE) {
                    printf("Found variable: %s\n", currentWord->currentWord);
                    handleVariableDismissFunction(currentWord, lastWord, oldVariables, newVariables, &variablesFound);
                    lastWordIsType = FALSE;
                }
                printf("%s\n", currentWord->currentWord);
                checkIfWordIsType(currentWord->currentWord, type, &lastWordIsType);
                strncpy(lastWord, currentWord->currentWord, MAX_STRING_LENGTH);
                memset(currentWord, 0, sizeof(WORD) + MAX_STRING_LENGTH);
                currentWord->wordsIsType = FALSE;
                currentWord->wordsIsVariable = FALSE;
                currentWord->wordsIsDone = FALSE;
                currentWord->specialCharacters = 0;
                currentWord->letterCharacters = 0;
                currentWord->spaceCharacters = 0;
                currentWord->currentCharIsLetter = FALSE;
                currentWord->wordSize = 0;
                currentWord->currentWord[MAX_STRING_LENGTH - 1] = '\0';
                currentWord->currentWord[0] = currentChar;
            }
        } else {

            handleCurrentWord(currentWord, &currentChar);
            /*
            if (lastCharIsLetter && lastWordIsType) {
                if (currentChar == '+') {
                    lastWordReadyForProcessing = TRUE;
                    for (int i = 0; i < MAX_STRING_LENGTH; ++i) {
                        if (strncmp(currentWord, oldVariables[i], MAX_STRING_LENGTH) != 0) {
                            strncpy(currentWord, newVariables[i], MAX_STRING_LENGTH);
                            lastWordReadyForProcessing = FALSE;
                            break;
                        }
                    }
                    if (lastWordReadyForProcessing) {
                        handleVariableDismissFunction(currentWord);
                    }
                }
            } */
        }


    }
}

void changeToNewWord(WORD *currentWord, char *currentChar) {
    int asciiSmallLettersBeginning = 'a';
    int asciiSmallLettersEnd = 'z';

    int asciiBigLettersBeginning = 'A';
    int asciiBigLettersEnd = 'Z';

    int lastCharIsLetter = FALSE;
    int currentCharIsLetter = FALSE;

    int lastCharIsSpace = FALSE;
    int currentCharIsSpace = FALSE;

    int lastCharIsSpecial = FALSE;
    int currentCharIsSpecial = FALSE;

    char lastChar;
    if (currentWord->wordSize > 0) {
        lastChar = currentWord->currentWord[currentWord->wordSize];
        if (lastChar == ' ') {
            lastCharIsSpace = TRUE;
        } else if (asciiSmallLettersBeginning < lastChar && lastChar < asciiSmallLettersEnd ||
                   asciiBigLettersBeginning < lastChar && lastChar < asciiBigLettersEnd) {
            lastCharIsLetter = TRUE;
        } else {
            lastCharIsSpecial = TRUE;
        }
        if (*currentChar == ' ') {
            currentCharIsSpace = TRUE;
        } else if (asciiSmallLettersBeginning < *currentChar && *currentChar < asciiSmallLettersEnd ||
                   asciiBigLettersBeginning < *currentChar && *currentChar < asciiBigLettersEnd) {
            currentCharIsLetter = TRUE;
        } else {
            currentCharIsSpecial = TRUE;
        }
    }
    // special and letter can go together, but space can only be alone
    if (lastCharIsSpace && currentCharIsSpace) {
        currentWord->wordsIsDone = FALSE;
        return;
    }
    if (lastCharIsSpecial && currentCharIsSpecial || lastCharIsLetter && currentCharIsLetter ||
        lastCharIsSpecial && currentCharIsLetter || lastCharIsLetter && currentCharIsSpecial) {
        currentWord->wordsIsDone = FALSE;
        return;
    }
    if (lastCharIsSpace && currentCharIsLetter || lastCharIsSpace && currentCharIsSpecial) {
        currentWord->wordsIsDone = TRUE;
        return;
    }
    if ((lastCharIsLetter && currentCharIsSpace) || (lastCharIsSpecial && currentCharIsSpace)) {
        currentWord->wordsIsDone = TRUE;
        return;
    }
}

int handleCurrentWord(WORD *currentWord, char *currentChar) {
    int asciiSmallLettersBeginning = 'a';
    int asciiSmallLettersEnd = 'z';

    int asciiBigLettersBeginning = 'A';
    int asciiBigLettersEnd = 'Z';

    int asciiValue = (int) *currentChar;

    char lastChar = currentWord->currentWord[currentWord->wordSize - 1];

    if (asciiValue == 32) {
        currentWord->spaceCharacters = currentWord->spaceCharacters + 1;
    }
    if (33 <= asciiValue && asciiValue <= 64) {
        currentWord->specialCharacters = currentWord->specialCharacters + 1;
    }
    if (asciiValue == 32) {
        currentWord->spaceCharacters = currentWord->spaceCharacters + 1;
    }


    currentWord->currentWord[currentWord->wordSize] = *currentChar;
    currentWord->wordSize = currentWord->wordSize + 1;


    return 1;
}

void handleVariableDismissFunction(WORD *currentWord, char *lastWord, char oldVariables[10][MAX_STRING_LENGTH],
                                   char newVariables[10][MAX_STRING_LENGTH], int *variablesFound) {
    char newVariable[MAX_STRING_LENGTH];
    char oldVariable[MAX_STRING_LENGTH];
    strncpy(oldVariable, currentWord->currentWord, MAX_STRING_LENGTH);
    memset(newVariable, 0, MAX_STRING_LENGTH);
    char *integer = "i";
    char *character = "*sz";
    if (strcmp(lastWord, "int") == 0) {
        if (currentWord->currentWord[0] != 'i') {
            if (97 <= (int) currentWord->currentWord[0] && (int) currentWord->currentWord[0] <= 122) {
                currentWord->currentWord[0] = (char) ((int) currentWord->currentWord[0] - 32);
            }
            strcat(newVariable, integer);
        }
    }
    if (strcmp(lastWord, "char") == 0) {
        if (currentWord->currentWord[0] != 's') {
            if (currentWord->currentWord[0] == '*') {
                // Remove * from current word
                for (int i = 0; i < currentWord->wordSize; ++i) {
                    currentWord->currentWord[i] = currentWord->currentWord[i + 1];
                }
                if (97 <= (int) currentWord->currentWord[0] && (int) currentWord->currentWord[0] <= 122) {
                    currentWord->currentWord[0] = (char) ((int) currentWord->currentWord[0] - 32);
                }
            }
        }
        strcat(newVariable, character);
    }
    strcat(newVariable, currentWord->currentWord);

    strncpy(oldVariables[*variablesFound], oldVariable, MAX_STRING_LENGTH);
    strncpy(newVariables[*variablesFound], newVariable, MAX_STRING_LENGTH);

    *variablesFound = *variablesFound + 1;

    printf("Updated variable: %s\n", newVariable);
    // Make current word into new variable
    memset(currentWord->currentWord, 0, MAX_STRING_LENGTH);
    strncpy(currentWord->currentWord, newVariable, MAX_STRING_LENGTH);
}

void checkIfWordIsType(char *currentWord, char *type[], int *lastWordIsType) {
    for (int i = 0; i < TYPE_ARRAY_LENGTH; ++i) {
        if (strcmp(currentWord, type[i]) == 0) {
            *lastWordIsType = TRUE;
            printf("Found type: %s\n", currentWord);
        }
    }
}