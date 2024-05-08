#include <malloc.h>
#include <string.h>
#include <errno.h>
#include "../include/linkedList.h"

int add(NODE_LIST *psnList, SENT_NODE *pssSentNode) {
    if (pssSentNode == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL- Error message: %s\n", strerror(errno));
        return -1;
    }

    NODE *psoTemp;
    psoTemp = (NODE *) malloc(sizeof(NODE));

    if (psoTemp == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }

    psoTemp->line = (char *) malloc(strlen(pssSentNode->line) + 1);
    if (psoTemp->line == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }

    memset(psoTemp->line, 0, strlen(pssSentNode->line) + 1);
    strncpy(psoTemp->line, pssSentNode->line, strlen(pssSentNode->line));
    psoTemp->line[strlen(pssSentNode->line)] = '\0';
    psoTemp->size = pssSentNode->size;

    psoTemp->pNextNode = NULL;

    if (psnList->pHead == NULL) {
        psnList->pHead = psoTemp;
        psnList->pTail = psoTemp;
        return 0;
    }

    nodeAddToEnd(psnList, psoTemp);

    return 0;
}

void nodeAddToEnd(NODE_LIST *psnList, NODE *psnTemp) {
    psnList->pTail->pNextNode = psnTemp;
    psnList->pTail = psnTemp;

    psnList->size++;
}

int addAtIndex(NODE_LIST *psnList, SENT_NODE *pssSentNode, int iIndex) {
    if (pssSentNode == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL- Error message: %s\n", strerror(errno));
        return - 1;
    }

    NODE *psoTemp;
    psoTemp = (NODE *) malloc(sizeof(NODE));

    if (psoTemp == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return - 1;
    }

    psoTemp->line = (char *) malloc(strlen(pssSentNode->line) + 1);
    if (psoTemp->line == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return - 1;
    }

    memset(psoTemp->line, 0, strlen(pssSentNode->line) + 1);
    strncpy(psoTemp->line, pssSentNode->line, strlen(pssSentNode->line));
    psoTemp->line[strlen(pssSentNode->line)] = '\0';
    psoTemp->size = pssSentNode->size;

    printf("pssSentNode->line: %s\n", pssSentNode->line);
    printf("pssSentNode->size: %d\n", pssSentNode->size);
    printf("psoTemp->line: %s\n", psoTemp->line);
    printf("psoTemp->size: %d\n", psoTemp->size);

    psoTemp->pNextNode = NULL;

    if (psnList->pHead == NULL) {
        psnList->pHead = psoTemp;
        psnList->pTail = psoTemp;
        return 0;
    }

    NODE *psoCurrent = psnList->pHead;
    NODE *psoPrevious = NULL;
    int iCounter = 0;

    while (psoCurrent != NULL && iCounter < iIndex) {
        psoPrevious = psoCurrent;
        psoCurrent = psoCurrent->pNextNode;
        iCounter++;
    }

    if (psoPrevious == NULL) {
        psoTemp->pNextNode = psnList->pHead;
        psnList->pHead = psoTemp;
    } else {
        psoPrevious->pNextNode = psoTemp;
        psoTemp->pNextNode = psoCurrent;
    }

    psnList->size++;
    return 0;
};

void freeLinkedList(NODE_LIST *psnList) {
    NODE *psnCurrent = psnList->pHead;
    NODE *psnNext;
    while (psnCurrent != NULL) {
        psnNext = psnCurrent->pNextNode;
        psnCurrent->pNextNode = NULL;
        free(psnCurrent->line);
        free(psnCurrent);
        psnCurrent = psnNext;
    }

    psnList->pHead = NULL;
    psnList->pTail = NULL;
    psnList->size = 0;
}

void printAllNodes(NODE_LIST *psnList) {
    int iCount = 0;
    NODE *psnCurrent = psnList->pHead;
    while (psnCurrent != NULL) {
        printf("%02d--%s", psnCurrent->size, psnCurrent->line);
        psnCurrent = psnCurrent->pNextNode;
        iCount++;
    }
    printf("\n");
}

int deleteSpecificNode(NODE_LIST *psnList, int iIndex) {
    NODE *psoCurrent = psnList->pHead;
    NODE *psoPrevious = NULL;
    int iCounter = 0;

    if (iIndex < 0) {
        errno = EINVAL;
        printf("index position cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iIndex > psnList->size) {
        errno = ERANGE;
        printf("Index position cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }


    while (psoCurrent != NULL && iCounter < iIndex) {
        psoPrevious = psoCurrent;
        psoCurrent = psoCurrent->pNextNode;
        iCounter++;
    }

    if (psoCurrent == NULL) {
        errno = ERANGE;
        printf("Index position cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (psoPrevious == NULL) {
        psnList->pHead = psoCurrent->pNextNode;
    } else {
        psoPrevious->pNextNode = psoCurrent->pNextNode;
    }

    free(psoCurrent->line);
    free(psoCurrent);
    psoCurrent = NULL;
    psnList->size--;

    return 0;
}