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

int main(int argc, char *argv[]) {
    int length = 0;
    char *firstWord = NULL;

    if (argc != 1) {
        printf("%s takes no arguments\n", argv[0]);
        return -1;
    }

    char string[MAX_STRING_LENGTH];
    int i = 0;
    FILE *binaryFile;

    memset(string, 0, sizeof(string));

    FILE *file = fopen("eksamen_v24_oppgave2.txt", "r");
    if (file == NULL) {
        printf("Failed to find file - Error message: %s\n", strerror(errno));
        return -1;
    }
    binaryFile = fopen("eksamen_v24_oppgave2.bin", "wb");
    if (binaryFile == NULL) {
        printf("Failed to find or create binary file - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (fgets(string, MAX_STRING_LENGTH, file) != NULL) {
        string[MAX_STRING_LENGTH - 1] = '\0';
        string[strcspn(string, "\r\n")] = '\0';

        length = strlen(string);
        if (firstWord == NULL) {
            firstWord = (char *) malloc(sizeof(char) * length + 1);
            if (firstWord == NULL) {
                printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
                break;
            }
            strncpy(firstWord, string, length);
        }

        TASK2_WORD_METADATA *pWord = (TASK2_WORD_METADATA *) malloc(sizeof(TASK2_WORD_METADATA) + length + 1);
        memset(pWord, 0, sizeof(TASK2_WORD_METADATA) + length + 1);
        if (pWord == NULL) {
            printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        } else {
            pWord->iIndex = i + 1;
            pWord->iSize = length;
            strncpy(pWord->szWord, string, pWord->iSize);
            pWord->bIsPalindrom = isPalindrome(pWord->szWord);
            pWord->bIsHeterogram = isHeterogram(pWord->szWord);
            pWord->bIsUppercase = isUpperCase(pWord->szWord);
            pWord->bIsLowercase = isLowerCase(pWord->szWord);

            if (i > 0) {
                pWord->bIsAnagram = isAnagram(pWord->szWord, firstWord);
                pWord->bIsDisjoint = areDisjoint(pWord->szWord, firstWord);
            } else {
                pWord->bIsAnagram = false;
                pWord->bIsDisjoint = false;
            }


            printf("Word %d: %s\n", pWord->iIndex, pWord->szWord);
            printf("Is Palindrom: %s\n", pWord->bIsPalindrom ? "True" : "False");
            printf("Is Heterogram: %s\n", pWord->bIsHeterogram ? "True" : "False");
            printf("Is Uppercase: %s\n", pWord->bIsUppercase ? "True" : "False");
            printf("Is Lowercase: %s\n", pWord->bIsLowercase ? "True" : "False");
            printf("Is Anagram: %s\n", pWord->bIsAnagram ? "True" : "False");
            printf("Is Disjoint: %s\n", pWord->bIsDisjoint ? "True" : "False");
            printf("\n");

            fwrite(pWord, sizeof(TASK2_WORD_METADATA) + pWord->iSize + 1, 1, binaryFile);

            free(pWord);
            i++;
        } // if pWord != NULL

    } // while loop

    free(firstWord);

    fclose(file);

    fclose(binaryFile);

    printf("\nClosing project...\n");

    return 0;
}
