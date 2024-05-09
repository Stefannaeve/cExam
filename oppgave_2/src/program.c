#include "program.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include "eksamen_v24_oppgave2_all_lower.h"
#include "eksamen_v24_oppgave2_all_upper.h"
#include "eksamen_v24_oppgave2_anagram.h"
#include "eksamen_v24_oppgave2_disjoint.h"
#include "eksamen_v24_oppgave2_heterogram.h"
#include "eksamen_v24_oppgave2_palindrom.h"

int program(){
    FILE *fdBinaryFile;
    FILE *fdFile;
    TASK2_WORD_METADATA *pstWord;
    int iLength = 0;
    int i = 0;
    int iStatus = 0;
    int firstInstance = 0;
    char *pszFirstWord = NULL;
    char string[MAX_STRING_LENGTH];

    memset(string, 0, sizeof(string));

    // Open the file in read mode
    fdFile = fopen("eksamen_v24_oppgave2.txt", "r");
    if (fdFile == NULL) {
        printf("Failed to find fdFile - Error message: %s\n", strerror(errno));
        return -1;
    }
    fdBinaryFile = fopen("eksamen_v24_oppgave2.bin", "wb");
    if (fdBinaryFile == NULL) {
        printf("Failed to find or create binary fdFile - Error message: %s\n", strerror(errno));
        return -1;
    }

    // Read every line of the text file, and add all the data into a struct with the right booleans from the functions
    // Then write the struct to a binary file
    while (fgets(string, MAX_STRING_LENGTH, fdFile) != NULL) {
        string[MAX_STRING_LENGTH - 1] = '\0';
        // Remove newline characters
        string[strcspn(string, "\r\n")] = '\0';

        iLength = strlen(string);
        // Allocate memory for the first word
        if (firstInstance == 0) {
            pszFirstWord = (char *) malloc(sizeof(char) * iLength + 1);
            printf("1: %d\n", iLength);
        }
        // Break while if memory allocation failed
        if (pszFirstWord == NULL) {
            iStatus = -1;
            printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        } else {
            if (firstInstance == 0){
                memset(pszFirstWord, 0, iLength + 1);
                printf("2: %d\n", iLength);
                strncpy(pszFirstWord, string, iLength);
                pszFirstWord[iLength] = '\0';
                firstInstance = 1;
            }

            // Allocate memory for the struct
            pstWord = (TASK2_WORD_METADATA *) malloc(sizeof(TASK2_WORD_METADATA) + iLength + 1);
            if (pstWord == NULL) {
                iStatus = -1;
                printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
            } else {
                // Fill the struct with the data
                memset(pstWord, 0, sizeof(TASK2_WORD_METADATA) + iLength + 1);
                pstWord->iIndex = i + 1;
                pstWord->iSize = iLength;
                strncpy(pstWord->strWord, string, pstWord->iSize);
                pstWord->strWord[pstWord->iSize] = '\0';
                pstWord->bIsPalindrom = isPalindrome(pstWord->strWord);
                pstWord->bIsHeterogram = isHeterogram(pstWord->strWord);
                pstWord->bIsUppercase = isUpperCase(pstWord->strWord);
                pstWord->bIsLowercase = isLowerCase(pstWord->strWord);

                // If pstWord is not the first word, check if it is an anagram and disjoint, else set them to false
                if (i > 0) {
                    pstWord->bIsAnagram = isAnagram(pstWord->strWord, pszFirstWord);
                    pstWord->bIsDisjoint = areDisjoint(pstWord->strWord, pszFirstWord);
                } else {
                    pstWord->bIsAnagram = false;
                    pstWord->bIsDisjoint = false;
                }

                // Print the data
                printf("Word %d: %s\n", pstWord->iIndex, pstWord->strWord);
                printf("Is Palindrom: %s\n", pstWord->bIsPalindrom ? "True" : "False");
                printf("Is Heterogram: %s\n", pstWord->bIsHeterogram ? "True" : "False");
                printf("Is Uppercase: %s\n", pstWord->bIsUppercase ? "True" : "False");
                printf("Is Lowercase: %s\n", pstWord->bIsLowercase ? "True" : "False");
                printf("Is Anagram: %s\n", pstWord->bIsAnagram ? "True" : "False");
                printf("Is Disjoint: %s\n", pstWord->bIsDisjoint ? "True" : "False");
                printf("\n");

                // Write the struct to the binary file
                iStatus = fwrite(pstWord, sizeof(TASK2_WORD_METADATA) + pstWord->iSize + 1, 1, fdBinaryFile);

                // Free the memory
                free(pstWord);
                pstWord = NULL;
                i++;
            } // if pstWord != NULL
        }
        if (iStatus != 1) {
            printf("Failed to write to binary fdFile - Error message: %s\n", strerror(errno));
            break;
        }
        memset(string, 0, sizeof(string));
    } // while loop

    // cleanup
    free(pszFirstWord);
    pszFirstWord = NULL;

    fclose(fdFile);
    fdFile = NULL;

    fclose(fdBinaryFile);
    fdBinaryFile = NULL;

    printf("\nClosing project...\n");

    return 0;
}
