#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include "tableReservationList.h"
#include "orderList.h"

void addToEnd(LIST *list, TABLERESERVATION *temp);
void addAtIndex(LIST *list, TABLERESERVATION *temp, int index);

int add(LIST *pslList, const SENT_TABLE_RESERVATION *pssSentTableReservation) {
    if (pssSentTableReservation == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL - Error message: %s\n", strerror(errno));
        return -1;
    }

    TABLERESERVATION *pstTemp;
    int iLengthOfName = strlen(pssSentTableReservation->pszName);
    pstTemp = (TABLERESERVATION *) malloc(sizeof(TABLERESERVATION) + iLengthOfName + 1);

    memset(pstTemp, 0, sizeof(TABLERESERVATION) + iLengthOfName + 1);
    if (pstTemp == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }

    pstTemp->iTableNumber = pssSentTableReservation->iTableNumber;
    pstTemp->iSeats = pssSentTableReservation->iSeats;
    pstTemp->iTime = pssSentTableReservation->iTime;
    pstTemp->pszName = (char *) malloc(iLengthOfName + 1);

    if (pstTemp->pszName == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        free(pstTemp);
        return -1;
    }
    strcpy(pstTemp->pszName, pssSentTableReservation->pszName);

    pstTemp->psoFoodOrders = (ORDER_LIST *) malloc(sizeof(ORDER_LIST));
    if (pstTemp->psoFoodOrders == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        free(pstTemp);
        return -1;
    }
    pstTemp->psoFoodOrders->pHead = NULL;
    pstTemp->psoFoodOrders->pTail = NULL;
    pstTemp->psoFoodOrders->size = 0;

    if (pslList->pHead == NULL) {
        pslList->pHead = pstTemp;
        pslList->pTail = pstTemp;
        pstTemp->iReservationNumber = 1;
        return 0;
    }

    TABLERESERVATION *pstCurrent = pslList->pHead;
    int counter = 0;
    while (pstCurrent != NULL) {
        if (strcmp(pstCurrent->pszName, pstTemp->pszName) > 0) {
            addAtIndex(pslList, pstTemp, counter);
            return 0;
        }
        pstCurrent = pstCurrent->pNextReservation;
        counter++;
    }
    addToEnd(pslList, pstTemp);
    return 0;
}

void addToEnd(LIST *pslList, TABLERESERVATION *pstTemp) {
    pslList->pTail->pNextReservation = pstTemp;
    pstTemp->iReservationNumber = pslList->pTail->iReservationNumber + 1;
    pstTemp->pPrevReservation = pslList->pTail;
    pslList->pTail = pstTemp;

    pslList->size++;
}

void addAtIndex(LIST *pslList, TABLERESERVATION *pstTemp, int iIndex) {

    TABLERESERVATION *pstCurrent;
    int iCounter = 0;

    if (iIndex == pslList->size + 1) {
        addToEnd(pslList, pstTemp);
        return;
    }
    if (iIndex == 0) {
        pstTemp->pNextReservation = pslList->pHead;
        pslList->pHead->pPrevReservation = pstTemp;
        pslList->pHead = pstTemp;
        pstTemp->iReservationNumber = 1;
    } else {
        pstCurrent = pslList->pHead;
        while (pstCurrent->pNextReservation != NULL && iCounter < iIndex - 1) {
            pstCurrent = pstCurrent->pNextReservation;
            iCounter++;
        }
        pstTemp->pNextReservation = pstCurrent->pNextReservation;
        pstTemp->pPrevReservation = pstCurrent;
        pstTemp->iReservationNumber = pstCurrent->iReservationNumber + 1;
        if (pstCurrent->pNextReservation != NULL) {
            pstCurrent->pNextReservation->pPrevReservation = pstTemp;
        }
        pstCurrent->pNextReservation = pstTemp;
    }
    fixReservationNumbersFromIndex(pslList, pstTemp->iReservationNumber-1);
    pslList->size++;
}

void fixReservationNumbersFromIndex(LIST *pslList, int iIndex) {
    TABLERESERVATION *current = pslList->pHead;
    int counter = 0;
    while (current != NULL) {
        if (counter >= iIndex) {
            current->iReservationNumber = counter + 1;
        }
        current = current->pNextReservation;
        counter++;
    }
}

void freeLinkedList(LIST *pslList) {
    TABLERESERVATION *pstCurrent = pslList->pHead;
    TABLERESERVATION *next;
    while (pstCurrent != NULL) {
        next = pstCurrent->pNextReservation;
        pstCurrent->pNextReservation = NULL;
        if (pstCurrent->psoFoodOrders != NULL) {
            orderFreeLinkedList(pstCurrent->psoFoodOrders);
            free(pstCurrent->psoFoodOrders);
        }
        free(pstCurrent->pszName);
        free(pstCurrent);
        pstCurrent = next;
    }
}

int printAllNodes(LIST *pslList) {
    TABLERESERVATION *pstCurrent = pslList->pHead;
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
        pstCurrent = pstCurrent->pNextReservation;
    }
    printf("\n");
    return 0;
}

int printSpecificNodeAndFood(LIST *pslList, int iReservationNumber) {
    int iCounter = 0;
    int iIndex = iReservationNumber - 1;
    if (iIndex < 0) {
        errno = EINVAL;
        printf("reservationNumber position cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    TABLERESERVATION *pstCurrent = pslList->pHead;
    while (pstCurrent != NULL && iCounter < iIndex) {
        pstCurrent = pstCurrent->pNextReservation;
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

int printSpecificReservationByReservationNumber(LIST *pslList, int iReservationNumber) {
    TABLERESERVATION *pstCurrent = pslList->pHead;
    int iCounter = 0;

    if (iReservationNumber < 1) {
        errno = EINVAL;
        printf("reservation number cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iReservationNumber - 1 > pslList->size) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pNextReservation;
        iCounter++;
    }
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
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

int printReservationByName(LIST *pslList, const char *pszName) {
    int iFoundReservation = 0;
    TABLERESERVATION *pstCurrent = pslList->pHead;

    if (pszName == NULL) {
        errno = EINVAL;
        printf("Name cannot be NULL - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (pstCurrent != NULL) {
        if (strcmp(pstCurrent->pszName, pszName) == 0) {
            printf("Name: %s\n", pstCurrent->pszName);
            printf("Reservation Number: %d\n", pstCurrent->iReservationNumber);
            printf("  Table Number: %d\n", pstCurrent->iTableNumber);
            printf("  Time: %d\n\n", pstCurrent->iTime);
            iFoundReservation = 1;
        }
        pstCurrent = pstCurrent->pNextReservation;
    }
    if (iFoundReservation != 1) {
        errno = ENOENT;
        printf("Name not found - Error message: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int printReservationOrdersAndSum(LIST *pslList, const int iReservationNumber) {
    TABLERESERVATION *pstCurrent = pslList->pHead;
    int iCounter = 0;

    if (iReservationNumber < 1) {
        errno = EINVAL;
        printf("reservation number cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iReservationNumber - 1 > pslList->size) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pNextReservation;
        iCounter++;
    }
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }
    printf("Name: %s\n", pstCurrent->pszName);
    printf("Reservation Number: %d\n", pstCurrent->iReservationNumber);
    printf("  Table Number: %d\n", pstCurrent->iTableNumber);
    printf("  Seats: %d\n", pstCurrent->iSeats);
    printf("  Time: %d\n", pstCurrent->iTime);
    orderPrintAllOrdersAndSum(pstCurrent->psoFoodOrders);
    return 0;
}

int printReservationOrdersForSpecificName(LIST *pslList, int reservationNumber, char *pszName){
    TABLERESERVATION *pstCurrent = pslList->pHead;
    int iCounter = 0;

    if (reservationNumber < 1) {
        errno = EINVAL;
        printf("reservation number cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (reservationNumber - 1 > pslList->size) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == reservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pNextReservation;
        iCounter++;
    }
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }
    printSumForSpecificName(pstCurrent->psoFoodOrders, pszName);
    return 0;
}

int deleteSpecificReservation(LIST *pslList, int iReservationNumber) {
    TABLERESERVATION *pstCurrent = pslList->pHead;
    int iIndex = iReservationNumber - 1;
    int iCounter = 0;

    if (iReservationNumber < 1) {
        errno = EINVAL;
        printf("Reservation number cannot be less than one - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iIndex > pslList->size) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pNextReservation;
        iCounter++;
    }

    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (pstCurrent->pPrevReservation == NULL) {
        pslList->pHead = pstCurrent->pNextReservation;
    } else {
        pstCurrent->pPrevReservation->pNextReservation = pstCurrent->pNextReservation;
    }

    if (pstCurrent->pNextReservation == NULL) {
        pslList->pTail = pstCurrent->pPrevReservation;
    } else {
        pstCurrent->pNextReservation->pPrevReservation = pstCurrent->pPrevReservation;
    }

    if (pstCurrent->psoFoodOrders != NULL) {
        orderFreeLinkedList(pstCurrent->psoFoodOrders);
        free(pstCurrent->psoFoodOrders);
    }
    free(pstCurrent->pszName);

    fixReservationNumbersFromIndex(pslList, pstCurrent->iReservationNumber-1);

    free(pstCurrent);
    pslList->size--;

    return 0;
}

int addFoodToSpecificReservation(LIST *pslList, int iReservationNumber, SENT_ORDER *pssSentOrder) {
    TABLERESERVATION *pstCurrent = pslList->pHead;
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

    if (iReservationNumber - 1 > pslList->size) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }


    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pNextReservation;
    }
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    iStatus = orderAdd(pstCurrent->psoFoodOrders, pssSentOrder);
    if (iStatus != 0) {
        printf("Failed to add order - Error message: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}