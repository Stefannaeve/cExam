#include <malloc.h>
#include <string.h>
#include <errno.h>
#include "orderList.h"

int orderAdd(ORDER_LIST *psoList, SENT_ORDER *pssSentOrder) {
    ORDER *psoTemp;
    int iLengthOfName = 0;
    int iLengthOfFoodDescription = 0;

    if (pssSentOrder == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL- Error message: %s\n", strerror(errno));
        return -1;
    }

    iLengthOfName = strlen(pssSentOrder->pszName);
    iLengthOfFoodDescription = strlen(pssSentOrder->pszFoodDescription);

    psoTemp = (ORDER *) malloc(sizeof(ORDER) + iLengthOfName + 1 + iLengthOfFoodDescription + 1);
    if (psoTemp == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }
    memset(psoTemp, 0, sizeof(ORDER) + iLengthOfName + 1 + iLengthOfFoodDescription + 1);

    psoTemp->pszName = (char *) malloc(iLengthOfName + 1);
    if (psoTemp->pszName == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }
    memset(psoTemp->pszName, 0, iLengthOfName + 1);
    strncpy(psoTemp->pszName, pssSentOrder->pszName, iLengthOfName);
    psoTemp->pszName[iLengthOfName] = '\0';

    psoTemp->pszFoodDescription = (char *) malloc(iLengthOfFoodDescription + 1);
    if (psoTemp->pszFoodDescription == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }
    memset(psoTemp->pszFoodDescription, 0, iLengthOfFoodDescription + 1);
    strncpy(psoTemp->pszFoodDescription, pssSentOrder->pszFoodDescription, iLengthOfFoodDescription);
    psoTemp->pszFoodDescription[iLengthOfFoodDescription] = '\0';

    psoTemp->iPrice = pssSentOrder->iPrice;

    psoTemp->psoNextOrder = NULL;

    if (psoList->psoHead == NULL) {
        psoList->psoHead = psoTemp;
        psoList->psoTail = psoTemp;
        free(pssSentOrder->pszName);
        free(pssSentOrder->pszFoodDescription);
        free(pssSentOrder);
        return 0;
    }

    free(pssSentOrder->pszName);
    free(pssSentOrder->pszFoodDescription);
    free(pssSentOrder);

    orderAddToEnd(psoList, psoTemp);

    return 0;
}

void orderAddToEnd(ORDER_LIST *psoList, ORDER *psoTemp) {
    psoList->psoTail->psoNextOrder = psoTemp;
    psoList->psoTail = psoTemp;

    psoList->iSize++;
}

void orderFreeLinkedList(ORDER_LIST *psoList) {
    ORDER *psoCurrent = psoList->psoHead;
    ORDER *psoNext;
    while (psoCurrent != NULL) {
        psoNext = psoCurrent->psoNextOrder;
        psoCurrent->psoNextOrder = NULL;
        free(psoCurrent->pszName);
        free(psoCurrent->pszFoodDescription);
        free(psoCurrent);
        psoCurrent = psoNext;
    }
}

void orderPrintAllOrders(ORDER_LIST *psoList) {
    int iCount = 0;
    ORDER *psoCurrent = psoList->psoHead;
    while (psoCurrent != NULL) {
        if(iCount == 0){
            printf("  Food:\n");
        }
        printf("     Name: %s\n", psoCurrent->pszName);
        printf("     Food Description: %s\n", psoCurrent->pszFoodDescription);
        printf("     Price: %d\n\n", psoCurrent->iPrice);
        psoCurrent = psoCurrent->psoNextOrder;
        iCount++;
    }
    printf("\n");
}

void orderPrintAllOrdersAndSum(ORDER_LIST *psoList) {
    ORDER *psoCurrent = psoList->psoHead;
    int iSum = 0;
    while (psoCurrent != NULL) {
        if(iSum == 0){
            printf("  Food:\n");
        }
        printf("     Name: %s\n", psoCurrent->pszName);
        printf("     Food Description: %s\n", psoCurrent->pszFoodDescription);
        printf("     Price: %d\n\n", psoCurrent->iPrice);
        iSum += psoCurrent->iPrice;
        psoCurrent = psoCurrent->psoNextOrder;
    }
    printf("Total iPrice: %d\n", iSum);
}

void printSumForSpecificName(ORDER_LIST *psoList, char *pszName) {
    ORDER *psoCurrent = psoList->psoHead;
    int iLengthOfCurrentName = 0;
    int iLengthOfName = strlen(pszName);
    int iBiggestLength = 0;
    int iSum = 0;
    while (psoCurrent != NULL) {
        iLengthOfCurrentName = strlen(psoCurrent->pszName);
        if (iLengthOfCurrentName > iLengthOfName) {
            iBiggestLength = iLengthOfName;
        } else {
            iBiggestLength = iLengthOfCurrentName;
        }
        // iBiggestLength + 1 to include the null terminator for accurate comparison
        if (strncmp(psoCurrent->pszName, pszName, iBiggestLength + 1) == 0) {
            printf("  Name: %s\n", psoCurrent->pszName);
            printf("  Food Description: %s\n", psoCurrent->pszFoodDescription);
            printf("  Price: %d\n\n", psoCurrent->iPrice);
            iSum += psoCurrent->iPrice;
        }
        psoCurrent = psoCurrent->psoNextOrder;
    }
    printf("Total iPrice for %s: %d\n", pszName, iSum);
}

int orderPrintSpecificNode(ORDER_LIST *psoList, int iIndex) {
    ORDER *psoCurrent = psoList->psoHead;
    int iCounter = 0;

    if (iIndex < 0) {
        errno = EINVAL;
        printf("index position cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (psoCurrent != NULL && iCounter < iIndex) {
        psoCurrent = psoCurrent->psoNextOrder;
        iCounter++;
    }
    if (psoCurrent == NULL) {
        errno = ERANGE;
        printf("Index position cannot be more than iSize of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    printf("Name: %s\n", psoCurrent->pszName);
    printf("Food Description: %s\n", psoCurrent->pszFoodDescription);
    printf("Price: %d\n", psoCurrent->iPrice);

    return 0;
}

int orderDeleteSpecificNode(ORDER_LIST *psoList, int iIndex) {
    ORDER *psoCurrent = psoList->psoHead;
    ORDER *psoPrevious = NULL;
    int iCounter = 0;

    if (iIndex < 0) {
        errno = EINVAL;
        printf("index position cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iIndex > psoList->iSize) {
        errno = ERANGE;
        printf("Index position cannot be more than iSize of list - Error message: %s\n", strerror(errno));
        return -1;
    }


    while (psoCurrent != NULL && iCounter < iIndex) {
        psoPrevious = psoCurrent;
        psoCurrent = psoCurrent->psoNextOrder;
        iCounter++;
    }

    if (psoCurrent == NULL) {
        errno = ERANGE;
        printf("Index position cannot be more than iSize of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (psoPrevious == NULL) {
        psoList->psoHead = psoCurrent->psoNextOrder;
    } else {
        psoPrevious->psoNextOrder = psoCurrent->psoNextOrder;
    }

    free(psoCurrent);
    psoList->iSize--;

    return 0;
}