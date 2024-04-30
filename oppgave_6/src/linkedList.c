#include <malloc.h>
#include <string.h>
#include <errno.h>
#include "../include/linkedList.h"

#define START 0
#define END 1

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

    strcpy(psoTemp->line, pssSentNode->line);

    psoTemp->pNextNode = NULL;

    if (psnList->pHead == NULL) {
        psnList->pHead = psoTemp;
        psnList->pTail = psoTemp;
        return 0;
    }

    nodeAddToEnd(psnList, psoTemp);

    return 0;
}

void nodeAddToEnd(NODE_LIST *psoList, NODE *psnTemp) {
    psoList->pTail->pNextNode = psnTemp;
    psoList->pTail = psnTemp;

    psoList->size++;
}

void orderFreeLinkedList(NODE_LIST *psnList) {
    NODE *psoCurrent = psnList->pHead;
    NODE *psoNext;
    while (psoCurrent != NULL) {
        psoNext = psoCurrent->pNextNode;
        psoCurrent->pNextNode = NULL;
        psoCurrent = psoNext;
    }
}

void orderPrintAllNodes(NODE_LIST *psnList) {
    int iCount = 0;
    NODE *psnCurrent = psnList->pHead;
    while (psnCurrent != NULL) {
        printf("%s", psnCurrent->line);
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

    free(psoCurrent);
    psnList->size--;

    return 0;
}