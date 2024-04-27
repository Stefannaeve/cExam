#include <malloc.h>
#include <string.h>
#include <errno.h>
#include "orderList.h"

#define START 0
#define END 1

int orderAdd(ORDER_LIST *psoList, SENT_ORDER *pssSentOrder) {
    if (pssSentOrder == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL- Error message: %s\n", strerror(errno));
        return -1;
    }

    ORDER *psoTemp;
    int iLengthOfName = strlen(pssSentOrder->name);
    int iLengthOfFoodDescription = strlen(pssSentOrder->foodDescription);
    psoTemp = (ORDER *) malloc(sizeof(ORDER) + iLengthOfName + 1 + iLengthOfFoodDescription + 1);

    if (psoTemp == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }

    psoTemp->name = (char *) malloc(iLengthOfName + 1);
    if (psoTemp->name == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }
    strcpy(psoTemp->name, pssSentOrder->name);

    psoTemp->foodDescription = (char *) malloc(iLengthOfFoodDescription + 1);
    if (psoTemp->foodDescription == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }
    strcpy(psoTemp->foodDescription, pssSentOrder->foodDescription);

    psoTemp->price = pssSentOrder->price;

    psoTemp->pNextOrder = NULL;

    if (psoList->pHead == NULL) {
        psoList->pHead = psoTemp;
        psoList->pTail = psoTemp;
        free(pssSentOrder->name);
        free(pssSentOrder->foodDescription);
        free(pssSentOrder);
        return 0;
    }

    free(pssSentOrder->name);
    free(pssSentOrder->foodDescription);
    free(pssSentOrder);

    orderAddToEnd(psoList, psoTemp);

    return 0;
}

void orderAddToEnd(ORDER_LIST *psoList, ORDER *psoTemp) {
    psoList->pTail->pNextOrder = psoTemp;
    psoList->pTail = psoTemp;

    psoList->size++;
}

void orderFreeLinkedList(ORDER_LIST *psoList) {
    ORDER *psoCurrent = psoList->pHead;
    ORDER *psoNext;
    while (psoCurrent != NULL) {
        psoNext = psoCurrent->pNextOrder;
        psoCurrent->pNextOrder = NULL;
        free(psoCurrent->name);
        free(psoCurrent->foodDescription);
        free(psoCurrent);
        psoCurrent = psoNext;
    }
}

void orderPrintAllOrders(ORDER_LIST *psoList) {
    int iCount = 0;
    ORDER *psoCurrent = psoList->pHead;
    while (psoCurrent != NULL) {
        if(iCount == 0){
            printf("  Food:\n");
        }
        printf("     Name: %s\n", psoCurrent->name);
        printf("     Food Description: %s\n", psoCurrent->foodDescription);
        printf("     Price: %d\n\n", psoCurrent->price);
        psoCurrent = psoCurrent->pNextOrder;
        iCount++;
    }
    printf("\n");
}

void orderPrintAllOrdersAndSum(ORDER_LIST *psoList) {
    ORDER *psoCurrent = psoList->pHead;
    int iSum = 0;
    while (psoCurrent != NULL) {
        if(iSum == 0){
            printf("  Food:\n");
        }
        printf("     Name: %s\n", psoCurrent->name);
        printf("     Food Description: %s\n", psoCurrent->foodDescription);
        printf("     Price: %d\n\n", psoCurrent->price);
        iSum += psoCurrent->price;
        psoCurrent = psoCurrent->pNextOrder;
    }
    printf("Total price: %d\n", iSum);
}

void printSumForSpecificName(ORDER_LIST *psoList, char *pszName) {
    ORDER *psoCurrent = psoList->pHead;
    int iSum = 0;
    while (psoCurrent != NULL) {
        if (strcmp(psoCurrent->name, pszName) == 0) {
            printf("  Name: %s\n", psoCurrent->name);
            printf("  Food Description: %s\n", psoCurrent->foodDescription);
            printf("  Price: %d\n\n", psoCurrent->price);
            iSum += psoCurrent->price;
        }
        psoCurrent = psoCurrent->pNextOrder;
    }
    printf("Total price for %s: %d\n", pszName, iSum);
}

int orderPrintSpecificNode(ORDER_LIST *psoList, int iIndex) {
    ORDER *psoCurrent = psoList->pHead;
    int iCounter = 0;

    if (iIndex < 0) {
        errno = EINVAL;
        printf("index position cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (psoCurrent != NULL && iCounter < iIndex) {
        psoCurrent = psoCurrent->pNextOrder;
        iCounter++;
    }
    if (psoCurrent == NULL) {
        errno = ERANGE;
        printf("Index position cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    printf("Name: %s\n", psoCurrent->name);
    printf("Food Description: %s\n", psoCurrent->foodDescription);
    printf("Price: %d\n", psoCurrent->price);

    return 0;
}

int orderDeleteSpecificNode(ORDER_LIST *psoList, int iIndex) {
    ORDER *psoCurrent = psoList->pHead;
    ORDER *psoPrevious = NULL;
    int iCounter = 0;

    if (iIndex < 0) {
        errno = EINVAL;
        printf("index position cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iIndex > psoList->size) {
        errno = ERANGE;
        printf("Index position cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }


    while (psoCurrent != NULL && iCounter < iIndex) {
        psoPrevious = psoCurrent;
        psoCurrent = psoCurrent->pNextOrder;
        iCounter++;
    }

    if (psoCurrent == NULL) {
        errno = ERANGE;
        printf("Index position cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (psoPrevious == NULL) {
        psoList->pHead = psoCurrent->pNextOrder;
    } else {
        psoPrevious->pNextOrder = psoCurrent->pNextOrder;
    }

    free(psoCurrent);
    psoList->size--;

    return 0;
}