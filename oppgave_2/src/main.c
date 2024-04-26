#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include "eksamen_v24_oppgave2_all_lower.h"
#include "eksamen_v24_oppgave2_all_upper.h"
#include "eksamen_v24_oppgave2_anagram.h"
#include "eksamen_v24_oppgave2_disjoint.h"
#include "eksamen_v24_oppgave2_heterogram.h"
#include "eksamen_v24_oppgave2_palindrom.h"

#define MAX_STRINGS 20
#define MAX_STRING_LENGTH 1024

typedef struct _TASK2_WORD_METADATA {
    int iIndex; // The index of the word, first = 1
    bool bIsPalindrom;
    bool bIsHeterogram;
    bool bIsUppercase;
    bool bIsLowercase;
    bool bIsAnagram;
    bool bIsDisjoint;
    int iSize; // The length word (number of chars)
    char szWord[]; // The word written «after» the struct
} TASK2_WORD_METADATA;

int main(int argc, char *argv[]) {
    char strings[MAX_STRINGS][MAX_STRING_LENGTH];

    memset(strings, 0, sizeof(strings));

    FILE *file = fopen("eksamen_v24_oppgave2.txt", "r");
    if (file == NULL) {
        printf("File not found\n");
        return -1;
    }

    int index = 0;
    while (index < MAX_STRINGS && fgets(strings[index], MAX_STRING_LENGTH, file) != NULL) {
        // Remove the newline character if necessary
        strings[index][strcspn(strings[index], "\n")] = '\0';
        index++;
    }

    TASK2_WORD_METADATA *pWordMetadata = (TASK2_WORD_METADATA *) malloc(sizeof(TASK2_WORD_METADATA) * index);

    int i = 0;
    while(i < index){
        TASK2_WORD_METADATA *pWord = (TASK2_WORD_METADATA *) malloc(sizeof(TASK2_WORD_METADATA) + strlen(strings[i]) + 1);
        pWord->iIndex = i + 1;
        pWord->iSize = strlen(strings[i]);
        strcpy(pWord->szWord, strings[i]);
        pWord->bIsPalindrom = isPalindrome(pWord->szWord);

        i++;
    }

    fclose(file);

    return 0;
}