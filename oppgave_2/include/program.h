#ifndef __TASK_PROGRAM_H__
#define __TASK_PROGRAM_H__

#define MAX_STRING_LENGTH 1024

#include <stdbool.h>

// #pragma pack (1)
typedef struct _TASK2_WORD_METADATA {
    int iIndex;
    bool bIsPalindrom;
    bool bIsHeterogram;
    bool bIsUppercase;
    bool bIsLowercase;
    bool bIsAnagram;
    bool bIsDisjoint;
    int iSize;
    char strWord[];
} TASK2_WORD_METADATA;

int program();

#endif //__TASK_PROGRAM_H__
