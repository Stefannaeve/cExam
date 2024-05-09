#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include "tableReservationList.h"
#include "orderList.h"

// Functino to add node to end of list, or to add at the head
int add(LIST *pslList, const SENT_TABLE_RESERVATION *pssSentTableReservation) {
    int iCounter = 0;
    int iLengthOfCurrentName = 0;
    int iLengthOfName = 0;
    int iBiggestLength = 0;
    TABLERESERVATION *pstTemp;
    TABLERESERVATION *pstCurrent;

    if (pssSentTableReservation == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL - Error message: %s\n", strerror(errno));
        return -1;
    }
    iLengthOfName = strlen(pssSentTableReservation->pszName);

    // Ready the list for a TABLERESERVATION instead of a SENT_TABLE_RESERVATION
    pstTemp = (TABLERESERVATION *) malloc(sizeof(TABLERESERVATION) + iLengthOfName + 1);
    if (pstTemp == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }
    memset(pstTemp, 0, sizeof(TABLERESERVATION) + iLengthOfName + 1);

    pstTemp->iTableNumber = pssSentTableReservation->iTableNumber;
    pstTemp->iSeats = pssSentTableReservation->iSeats;
    pstTemp->iTime = pssSentTableReservation->iTime;

    // Allocate memory for the name
    pstTemp->pszName = (char *) malloc(iLengthOfName + 1);
    if (pstTemp->pszName == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        free(pstTemp);
        pstTemp = NULL;
        return -1;
    }
    memset(pstTemp->pszName, 0, iLengthOfName + 1);
    strncpy(pstTemp->pszName, pssSentTableReservation->pszName, iLengthOfName);
    pstTemp->pszName[iLengthOfName] = '\0';

    // Allocate memory for the food list
    pstTemp->psoFoodOrders = (ORDER_LIST *) malloc(sizeof(ORDER_LIST));
    if (pstTemp->psoFoodOrders == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        free(pstTemp);
        pstTemp = NULL;
        return -1;
    }
    memset(pstTemp->psoFoodOrders, 0, sizeof(ORDER_LIST));
    pstTemp->psoFoodOrders->psoHead = NULL;
    pstTemp->psoFoodOrders->psoTail = NULL;
    pstTemp->psoFoodOrders->iSize = 0;

    // Add to head of list if list is empty
    if (pslList->pstHead == NULL) {
        pslList->pstHead = pstTemp;
        pslList->pstTail = pstTemp;
        pstTemp->iReservationNumber = 1;
        return 0;
    }

    pstCurrent = pslList->pstHead;
    iCounter = 0;
    while (pstCurrent != NULL) {
        // find the shortest length of the two strings, to avoid going out of bounds
        iLengthOfCurrentName = strlen(pstCurrent->pszName);
        if (iLengthOfCurrentName > iLengthOfName) {
            iBiggestLength = iLengthOfName;
        } else {
            iBiggestLength = iLengthOfCurrentName;
        }
        // iBiggestLength + 1 to include the null terminator for accurate comparison
        if (strncmp(pstCurrent->pszName, pstTemp->pszName, iBiggestLength) > 0) {
            addAtIndex(pslList, pstTemp, iCounter);
            return 0;
        }
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }
    // Add to end of list
    addToEnd(pslList, pstTemp);
    return 0;
}

// Add node to end of list
void addToEnd(LIST *pslList, TABLERESERVATION *pstTemp) {
    pslList->pstTail->pstNextReservation = pstTemp;
    pstTemp->iReservationNumber = pslList->pstTail->iReservationNumber + 1;
    pstTemp->pstPrevReservation = pslList->pstTail;
    pslList->pstTail = pstTemp;

    pslList->iSize++;
}

// Function to add node at specific index
void addAtIndex(LIST *pslList, TABLERESERVATION *pstTemp, int iIndex) {
    TABLERESERVATION *pstCurrent;
    int iCounter = 0;

    // Add to end of list
    if (iIndex == pslList->iSize + 1) {
        addToEnd(pslList, pstTemp);
        return;
    }
    // Add to head of list
    if (iIndex == 0) {
        pstTemp->pstNextReservation = pslList->pstHead;
        pslList->pstHead->pstPrevReservation = pstTemp;
        pslList->pstHead = pstTemp;
        pstTemp->iReservationNumber = 1;
    } else {
        // Add somewhere in the middle
        pstCurrent = pslList->pstHead;
        while (pstCurrent->pstNextReservation != NULL && iCounter < iIndex - 1) {
            pstCurrent = pstCurrent->pstNextReservation;
            iCounter++;
        }
        pstTemp->pstNextReservation = pstCurrent->pstNextReservation;
        pstTemp->pstPrevReservation = pstCurrent;
        pstTemp->iReservationNumber = pstCurrent->iReservationNumber + 1;
        if (pstCurrent->pstNextReservation != NULL) {
            pstCurrent->pstNextReservation->pstPrevReservation = pstTemp;
        }
        pstCurrent->pstNextReservation = pstTemp;
    }
    // Function to fic the reservations after insertion
    fixReservationNumbersFromIndex(pslList, pstTemp->iReservationNumber-1);
    pslList->iSize++;
}

// Function to fix reservation numbers after insertion
void fixReservationNumbersFromIndex(LIST *pslList, int iIndex) {
    TABLERESERVATION *pstCurrent = pslList->pstHead;
    int iCounter = 0;
    while (pstCurrent != NULL) {
        if (iCounter >= iIndex) {
            pstCurrent->iReservationNumber = iCounter + 1;
        }
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }
}

// Functino to free reservation list and food list
void freeLinkedList(LIST *pslList) {
    TABLERESERVATION *pstCurrent = pslList->pstHead;
    TABLERESERVATION *pstNext;
    while (pstCurrent != NULL) {
        pstNext = pstCurrent->pstNextReservation;
        pstCurrent->pstNextReservation = NULL;
        if (pstCurrent->psoFoodOrders != NULL) {
            orderFreeLinkedList(pstCurrent->psoFoodOrders);
            free(pstCurrent->psoFoodOrders);
            pstCurrent->psoFoodOrders = NULL;
        }
        free(pstCurrent->pszName);
        pstCurrent->pszName = NULL;
        free(pstCurrent);
        pstCurrent = pstNext;
    }
    pslList->pstHead = NULL;
    pslList->pstTail = NULL;
    pslList->iSize = 0;
    pstCurrent = NULL;
    pstNext = NULL;
}

// Function to print all nodes in list including all necessary information
int printAllNodes(LIST *pslList) {
    TABLERESERVATION *pstCurrent = pslList->pstHead;
    if(pstCurrent == NULL) {
        printf("You have no reservations\n");
        return -1;
    }
    while (pstCurrent != NULL) {
        printf("Reservation Number: %d\n", pstCurrent->iReservationNumber);
        printf("Name: %s\n", pstCurrent->pszName);
        printf("  Table Number: %d\n", pstCurrent->iTableNumber);
        printf("  Seats: %d\n", pstCurrent->iSeats);
        printf("  Time: %d\n\n", pstCurrent->iTime);
        pstCurrent = pstCurrent->pstNextReservation;
    }
    printf("\n");
    return 0;
}

int printSpecificNodeAndFood(LIST *pslList, int iReservationNumber) {
    int iCounter = 0;
    int iIndex = iReservationNumber - 1;
    TABLERESERVATION *pstCurrent;
    if (iIndex < 0) {
        errno = EINVAL;
        printf("reservationNumber position cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    pstCurrent = pslList->pstHead;
    while (pstCurrent != NULL && iCounter < iIndex) {
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Index position cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }
    printf("Name: %s\n", pstCurrent->pszName);
    printf("Reservation Number: %d\n", pstCurrent->iReservationNumber);
    printf("  Table Number: %d\n", pstCurrent->iTableNumber);
    printf("  Seats: %d\n", pstCurrent->iSeats);
    printf("  Time: %d\n", pstCurrent->iTime);
    orderPrintAllOrders(pstCurrent->psoFoodOrders);
    return 0;
}

// Function to print specific reservation by reservation number
int printSpecificReservationByReservationNumber(LIST *pslList, int iReservationNumber) {
    TABLERESERVATION *pstCurrent = pslList->pstHead;
    int iCounter = 0;

    if (iReservationNumber < 1) {
        errno = EINVAL;
        printf("reservation number cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iReservationNumber - 1 > pslList->iSize) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    // Find node with the right reservation number
    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }

    // If node is not found
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    // Print all information about the reservation
    printf("Name: %s\n", pstCurrent->pszName);
    printf("Reservation Number: %d\n", pstCurrent->iReservationNumber);
    printf("  Table Number: %d\n", pstCurrent->iTableNumber);
    printf("  Seats: %d\n", pstCurrent->iSeats);
    printf("  Time: %d\n", pstCurrent->iTime);
    orderPrintAllOrders(pstCurrent->psoFoodOrders);
    return 0;
}

// Function to print reservation by name
int printReservationByName(LIST *pslList, const char *pszName) {
    int iFoundReservation = 0;
    TABLERESERVATION *pstCurrent = pslList->pstHead;
    int iLengthOfCurrentName = 0;
    int iLengthOfName = strlen(pszName);
    int iBiggestLength = 0;

    if (pszName == NULL) {
        errno = EINVAL;
        printf("Name cannot be NULL - Error message: %s\n", strerror(errno));
        return -1;
    }

    // Find node(s) with the right name
    while (pstCurrent != NULL) {
        // find the shortest length of the two strings, to avoid going out of bounds
        iLengthOfCurrentName = strlen(pstCurrent->pszName);
        if (iLengthOfCurrentName > iLengthOfName) {
            iBiggestLength = iLengthOfName;
        } else {
            iBiggestLength = iLengthOfCurrentName;
        }
        // iBiggestLength + 1 to include the null terminator for accurate comparison
        if (strncmp(pstCurrent->pszName, pszName, iBiggestLength + 1) == 0) {
            printf("Name: %s\n", pstCurrent->pszName);
            printf("Reservation Number: %d\n", pstCurrent->iReservationNumber);
            printf("  Table Number: %d\n", pstCurrent->iTableNumber);
            printf("  Time: %d\n\n", pstCurrent->iTime);
            iFoundReservation = 1;
        }
        pstCurrent = pstCurrent->pstNextReservation;
    }

    // If no node is found
    if (iFoundReservation != 1) {
        errno = ENOENT;
        printf("Name not found - Error message: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

// Function to print reservation order and sum
int printReservationOrdersAndSum(LIST *pslList, const int iReservationNumber) {
    TABLERESERVATION *pstCurrent = pslList->pstHead;
    int iCounter = 0;

    if (iReservationNumber < 1) {
        errno = EINVAL;
        printf("reservation number cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iReservationNumber - 1 > pslList->iSize) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    // Find node with the right reservation number
    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }
    // If node is not found
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }
    // Print all information about the reservation if found
    printf("Name: %s\n", pstCurrent->pszName);
    printf("Reservation Number: %d\n", pstCurrent->iReservationNumber);
    printf("  Table Number: %d\n", pstCurrent->iTableNumber);
    printf("  Seats: %d\n", pstCurrent->iSeats);
    printf("  Time: %d\n", pstCurrent->iTime);
    orderPrintAllOrdersAndSum(pstCurrent->psoFoodOrders);
    return 0;
}

// Function to find sum for food at a specific name in a reservation
int printReservationOrdersForSpecificName(LIST *pslList, int iReservationNumber, char *pszName){
    TABLERESERVATION *pstCurrent = pslList->pstHead;
    int iCounter = 0;

    if (iReservationNumber < 1) {
        errno = EINVAL;
        printf("reservation number cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iReservationNumber - 1 > pslList->iSize) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    // Find node with the right reservation number
    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }

    // If node is not found
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    // Print sum for food at specific name
    printSumForSpecificName(pstCurrent->psoFoodOrders, pszName);
    return 0;
}

// Function to delete a specific reservation
int deleteSpecificReservation(LIST *pslList, int iReservationNumber) {
    TABLERESERVATION *pstCurrent = pslList->pstHead;
    int iIndex = iReservationNumber - 1;
    int iCounter = 0;

    if (iReservationNumber < 1) {
        errno = EINVAL;
        printf("Reservation number cannot be less than one - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iIndex > pslList->iSize) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    // Find right node with right reservation number
    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }

    // If node is not found
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    // Logic to remove the node from the list

    // If node is the head, or else bind the right node
    if (pstCurrent->pstPrevReservation == NULL) {
        pslList->pstHead = pstCurrent->pstNextReservation;
    } else {
        pstCurrent->pstPrevReservation->pstNextReservation = pstCurrent->pstNextReservation;
    }

    // If node is the tail, or else bind the right node
    if (pstCurrent->pstNextReservation == NULL) {
        pslList->pstTail = pstCurrent->pstPrevReservation;
    } else {
        pstCurrent->pstNextReservation->pstPrevReservation = pstCurrent->pstPrevReservation;
    }

    // If node has food orders, free the list
    if (pstCurrent->psoFoodOrders != NULL) {
        orderFreeLinkedList(pstCurrent->psoFoodOrders);
        free(pstCurrent->psoFoodOrders);
        pstCurrent->psoFoodOrders = NULL;
    }

    // Fix reservation numbers and free
    free(pstCurrent->pszName);
    pstCurrent->pszName = NULL;

    fixReservationNumbersFromIndex(pslList, pstCurrent->iReservationNumber-1);

    free(pstCurrent);
    pstCurrent = NULL;
    pslList->iSize--;

    return 0;
}

// Function to add food to a specific reservation
int addFoodToSpecificReservation(LIST *pslList, int iReservationNumber, SENT_ORDER *pssSentOrder) {
    TABLERESERVATION *pstCurrent = pslList->pstHead;
    int iStatus;

    if (pssSentOrder == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iReservationNumber < 1) {
        errno = EINVAL;
        printf("Reservation number cannot be less than one - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iReservationNumber - 1 > pslList->iSize) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    // Find node with the right reservation number
    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pstNextReservation;
    }
    // If node is not found
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    // Add food to the food list
    iStatus = orderAdd(pstCurrent->psoFoodOrders, pssSentOrder);
    if (iStatus != 0) {
        printf("Failed to add order - Error message: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}