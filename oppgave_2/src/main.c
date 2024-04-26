#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "eksamen_v24_oppgave2_all_lower.h"
#include "eksamen_v24_oppgave2_all_upper.h"
#include "eksamen_v24_oppgave2_anagram.h"
#include "eksamen_v24_oppgave2_disjoint.h"
#include "eksamen_v24_oppgave2_heterogram.h"
#include "eksamen_v24_oppgave2_palindrom.h"

#include "main.h"

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
    char *szWord; // The word written «after» the struct
} TASK2_WORD_METADATA;

int main(int argc, char *argv[]) {
    char strings[MAX_STRINGS][MAX_STRING_LENGTH];
    int i = 0;
    int length;
    int index = 0;
    FILE *binaryFile;

    memset(strings, 0, sizeof(strings));

    FILE *file = fopen("eksamen_v24_oppgave2.txt", "r");
    if (file == NULL) {
        printf("Failed to find file - Error message: %s\n", strerror(errno));
        return -1;
    }

    index = 0;
    while (index < MAX_STRINGS && fgets(strings[index], MAX_STRING_LENGTH, file) != NULL) {
        strings[index][strcspn(strings[index], "\r")] = '\0';
        index++;
    }

    fclose(file);

    TASK2_WORD_METADATA *pWordMetadata = (TASK2_WORD_METADATA *) malloc(sizeof(TASK2_WORD_METADATA) * index);
    if (pWordMetadata == NULL) {
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
    } else {
        while (i < index) {
            length = strlen(strings[i]);
            TASK2_WORD_METADATA *pWord = (TASK2_WORD_METADATA *) malloc(sizeof(TASK2_WORD_METADATA) + length + 1);
            if (pWord == NULL) {
                printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
                break;
            }
            pWord->iIndex = i + 1;
            pWord->iSize = length;
            pWord->szWord = (char *) malloc(sizeof(char) * length + 1);
            if (pWord->szWord == NULL) {
                printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
                break;
            }
            strcpy(pWord->szWord, strings[i]);
            pWord->bIsPalindrom = isPalindrome(pWord->szWord);
            pWord->bIsHeterogram = isHeterogram(pWord->szWord);
            pWord->bIsUppercase = isUpperCase(pWord->szWord);
            pWord->bIsLowercase = isLowerCase(pWord->szWord);

            if (i > 0) {
                pWord->bIsAnagram = isAnagram(pWordMetadata[0].szWord, pWord->szWord);
                pWord->bIsDisjoint = areDisjoint(pWordMetadata[0].szWord, pWord->szWord);
            } else {
                pWord->bIsAnagram = false;
                pWord->bIsDisjoint = false;
            }

            /* LEGACY CODE
            if(i > 0){
                pWordMetadata[i - 1].bIsAnagram = isAnagram(pWordMetadata[i - 1].szWord, pWord->szWord);
                pWordMetadata[i - 1].bIsDisjoint = areDisjoint(pWordMetadata[i - 1].szWord, pWord->szWord);
            } else {
                pWordMetadata[i].bIsAnagram = false;
                pWordMetadata[i].bIsDisjoint = false;
            }
             */

            pWordMetadata[i] = *pWord;

            free(pWord);

            i++;
        }

        for (int j = 0; j < index; ++j) {
            printf("Word %d: %s\n", pWordMetadata[j].iIndex, pWordMetadata[j].szWord);
            printf("Is Palindrom: %s\n", pWordMetadata[j].bIsPalindrom ? "True" : "False");
            printf("Is Heterogram: %s\n", pWordMetadata[j].bIsHeterogram ? "True" : "False");
            printf("Is Uppercase: %s\n", pWordMetadata[j].bIsUppercase ? "True" : "False");
            printf("Is Lowercase: %s\n", pWordMetadata[j].bIsLowercase ? "True" : "False");
            printf("Is Anagram: %s\n", pWordMetadata[j].bIsAnagram ? "True" : "False");
            printf("Is Disjoint: %s\n", pWordMetadata[j].bIsDisjoint ? "True" : "False");
            printf("\n");
        }

        binaryFile = fopen("eksamen_v24_oppgave2.bin", "w+b");
        if (binaryFile == NULL) {
            printf("Failed to find or create binary file - Error message: %s\n", strerror(errno));
        } else {

            for (int j = 0; j < index; ++j) {
                fwrite(&pWordMetadata[j], sizeof(TASK2_WORD_METADATA), 1, binaryFile);
                fwrite(pWordMetadata[j].szWord, sizeof(char), pWordMetadata[j].iSize, binaryFile);
            }

            fclose(binaryFile);

        } // open binary file

        for (int k = 0; k < index; ++k) {
            free(pWordMetadata[k].szWord);
        }
        free(pWordMetadata);

    } // malloc of pWordMetadata

    printf("\nClosing project...\n");

    return 0;
}
