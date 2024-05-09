#include <malloc.h>
#include <string.h>
#include <errno.h>
#include "orderList.h"

// SINGLY LINKED LIST

// Add order to the end of the list
int orderAdd(ORDER_LIST *psoList, SENT_ORDER *pssSentOrder) {
    ORDER *psoTemp;
    int iLengthOfName = 0;
    int iLengthOfFoodDescription = 0;

    // Make sure the struct is not NULL
    if (pssSentOrder == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL- Error message: %s\n", strerror(errno));
        return -1;
    }

    // Find the lengths of the strings in the struct
    iLengthOfName = strlen(pssSentOrder->pszName);
    iLengthOfFoodDescription = strlen(pssSentOrder->pszFoodDescription);

    // Malloc the Order structs for insertion in the list
    psoTemp = (ORDER *) malloc(sizeof(ORDER) + iLengthOfName + 1 + iLengthOfFoodDescription + 1);
    if (psoTemp == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
    } else {
        memset(psoTemp, 0, sizeof(ORDER) + iLengthOfName + 1 + iLengthOfFoodDescription + 1);
        // Malloc the order struct strings and memset and zero terminate them
        psoTemp->pszName = (char *) malloc(iLengthOfName + 1);
        if (psoTemp->pszName == NULL) {
            errno = ENOMEM;
            printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        } else {
            memset(psoTemp->pszName, 0, iLengthOfName + 1);
            strncpy(psoTemp->pszName, pssSentOrder->pszName, iLengthOfName);
            psoTemp->pszName[iLengthOfName] = '\0';

            psoTemp->pszFoodDescription = (char *) malloc(iLengthOfFoodDescription + 1);
            if (psoTemp->pszFoodDescription == NULL) {
                errno = ENOMEM;
                printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
            } else {
                memset(psoTemp->pszFoodDescription, 0, iLengthOfFoodDescription + 1);
                strncpy(psoTemp->pszFoodDescription, pssSentOrder->pszFoodDescription, iLengthOfFoodDescription);
                psoTemp->pszFoodDescription[iLengthOfFoodDescription] = '\0';

                psoTemp->iPrice = pssSentOrder->iPrice;
                psoTemp->psoNextOrder = NULL;

                // Free the struct from called from function
                free(pssSentOrder->pszName);
                pssSentOrder->pszName = NULL;
                free(pssSentOrder->pszFoodDescription);
                pssSentOrder->pszFoodDescription = NULL;
                free(pssSentOrder);
                pssSentOrder = NULL;

                // If the list is empty, add the first order
                if (psoList->psoHead == NULL) {
                    psoList->psoHead = psoTemp;
                    psoList->psoTail = psoTemp;
                    return 0;
                }
                // The function to add to the end of the list
                orderAddToEnd(psoList, psoTemp);
                return 0;
            }
            free(psoTemp->pszName);
            psoTemp->pszName = NULL;
        }
        free(psoTemp);
        psoTemp = NULL;
    }



    free(pssSentOrder->pszName);
    pssSentOrder->pszName = NULL;
    free(pssSentOrder->pszFoodDescription);
    pssSentOrder->pszFoodDescription = NULL;
    free(pssSentOrder);
    pssSentOrder = NULL;

    return -1;
}

// Add order to the end of the list
void orderAddToEnd(ORDER_LIST *psoList, ORDER *psoTemp) {
    psoList->psoTail->psoNextOrder = psoTemp;
    psoList->psoTail = psoTemp;

    psoList->iSize++;
}

// Free every node in the list
void orderFreeLinkedList(ORDER_LIST *psoList) {
    ORDER *psoCurrent = psoList->psoHead;
    ORDER *psoNext;
    while (psoCurrent != NULL) {
        psoNext = psoCurrent->psoNextOrder;
        psoCurrent->psoNextOrder = NULL;
        free(psoCurrent->pszName);
        psoCurrent->pszName = NULL;
        free(psoCurrent->pszFoodDescription);
        psoCurrent->pszFoodDescription = NULL;
        free(psoCurrent);
        psoCurrent = psoNext;
    }
    psoList->psoHead = NULL;
    psoList->psoTail = NULL;
    psoList->iSize = 0;
    psoCurrent = NULL;
    psoNext = NULL;
}

// Print all orders in the list with all the necessary information
void orderPrintAllOrders(ORDER_LIST *psoList) {
    int iCount = 0;
    ORDER *psoCurrent = psoList->psoHead;
    while (psoCurrent != NULL) {
        if (iCount == 0) {
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

// Print all orders in the list with all the necessary information and sum the prices of all food
void orderPrintAllOrdersAndSum(ORDER_LIST *psoList) {
    ORDER *psoCurrent = psoList->psoHead;
    int iSum = 0;
    while (psoCurrent != NULL) {
        if (iSum == 0) {
            printf("  Food:\n");
        }
        printf("     Name: %s\n", psoCurrent->pszName);
        printf("     Food Description: %s\n", psoCurrent->pszFoodDescription);
        printf("     Price: %d\n\n", psoCurrent->iPrice);
        iSum += psoCurrent->iPrice;
        psoCurrent = psoCurrent->psoNextOrder;
    }
    printf("Total price: %d\n", iSum);
}

// Print one order, and the specific sum for a specific person
void printSumForSpecificName(ORDER_LIST *psoList, char *pszName) {
    ORDER *psoCurrent = psoList->psoHead;
    int iLengthOfCurrentName = 0;
    int iLengthOfName = strlen(pszName);
    int iBiggestLength = 0;
    int iSum = 0;

    // Loop through the list and compare the names
    while (psoCurrent != NULL) {
        // find the shortest length of the two strings, to avoid going out of bounds
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

    if (psoCurrent == NULL && iSum == 0) {
        printf("No orders found for %s\n", pszName);
        return;
    }
    printf("Total price for %s: %d\n", pszName, iSum);
}