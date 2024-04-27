#ifndef __TASK_MAIN_H__
#define __TASK_MAIN_H__
int main(int argc, char *argv[]);

#define MAX_STRINGS 20

#define MAX_STRING_LENGTH 1024

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
    char szWord[];
} TASK2_WORD_METADATA;

#endif //__TASK_MAIN_H__
