#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include "tableReservationList.h"
#include "orderList.h"

int add(LIST *pslList, const SENT_TABLE_RESERVATION *pssSentTableReservation) {
    int iCounter = 0;
    TABLERESERVATION *pstTemp;
    TABLERESERVATION *pstCurrent;
    if (pssSentTableReservation == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL - Error message: %s\n", strerror(errno));
        return -1;
    }
    int iLengthOfName = strlen(pssSentTableReservation->pszName);

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
    pstTemp->pszName = (char *) malloc(iLengthOfName + 1);

    if (pstTemp->pszName == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        free(pstTemp);
        return -1;
    }
    memset(pstTemp->pszName, 0, iLengthOfName + 1);
    strncpy(pstTemp->pszName, pssSentTableReservation->pszName, iLengthOfName);
    pstTemp->pszName[iLengthOfName] = '\0';

    pstTemp->psoFoodOrders = (ORDER_LIST *) malloc(sizeof(ORDER_LIST));
    if (pstTemp->psoFoodOrders == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        free(pstTemp);
        return -1;
    }
    memset(pstTemp->psoFoodOrders, 0, sizeof(ORDER_LIST));
    pstTemp->psoFoodOrders->psoHead = NULL;
    pstTemp->psoFoodOrders->psoTail = NULL;
    pstTemp->psoFoodOrders->iSize = 0;

    if (pslList->pstHead == NULL) {
        pslList->pstHead = pstTemp;
        pslList->pstTail = pstTemp;
        pstTemp->iReservationNumber = 1;
        return 0;
    }

    pstCurrent = pslList->pstHead;
    iCounter = 0;
    while (pstCurrent != NULL) {
        if (strcmp(pstCurrent->pszName, pstTemp->pszName) > 0) {
            addAtIndex(pslList, pstTemp, iCounter);
            return 0;
        }
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }
    addToEnd(pslList, pstTemp);
    return 0;
}

void addToEnd(LIST *pslList, TABLERESERVATION *pstTemp) {
    pslList->pstTail->pstNextReservation = pstTemp;
    pstTemp->iReservationNumber = pslList->pstTail->iReservationNumber + 1;
    pstTemp->pstPrevReservation = pslList->pstTail;
    pslList->pstTail = pstTemp;

    pslList->iSize++;
}

void addAtIndex(LIST *pslList, TABLERESERVATION *pstTemp, int iIndex) {
    TABLERESERVATION *pstCurrent;
    int iCounter = 0;

    if (iIndex == pslList->iSize + 1) {
        addToEnd(pslList, pstTemp);
        return;
    }
    if (iIndex == 0) {
        pstTemp->pstNextReservation = pslList->pstHead;
        pslList->pstHead->pstPrevReservation = pstTemp;
        pslList->pstHead = pstTemp;
        pstTemp->iReservationNumber = 1;
    } else {
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
    fixReservationNumbersFromIndex(pslList, pstTemp->iReservationNumber-1);
    pslList->iSize++;
}

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

void freeLinkedList(LIST *pslList) {
    TABLERESERVATION *pstCurrent = pslList->pstHead;
    TABLERESERVATION *pstNext;
    while (pstCurrent != NULL) {
        pstNext = pstCurrent->pstNextReservation;
        pstCurrent->pstNextReservation = NULL;
        if (pstCurrent->psoFoodOrders != NULL) {
            orderFreeLinkedList(pstCurrent->psoFoodOrders);
            free(pstCurrent->psoFoodOrders);
        }
        free(pstCurrent->pszName);
        free(pstCurrent);
        pstCurrent = pstNext;
    }
}

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
        printf("Index position cannot be more than iSize of list - Error message: %s\n", strerror(errno));
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
    TABLERESERVATION *pstCurrent = pslList->pstHead;
    int iCounter = 0;

    if (iReservationNumber < 1) {
        errno = EINVAL;
        printf("reservation number cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (iReservationNumber - 1 > pslList->iSize) {
        errno = ERANGE;
        printf("Reservation number cannot be more than iSize of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than iSize of list - Error message: %s\n", strerror(errno));
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
    TABLERESERVATION *pstCurrent = pslList->pstHead;

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
        pstCurrent = pstCurrent->pstNextReservation;
    }
    if (iFoundReservation != 1) {
        errno = ENOENT;
        printf("Name not found - Error message: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

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
        printf("Reservation number cannot be more than iSize of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than iSize of list - Error message: %s\n", strerror(errno));
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
        printf("Reservation number cannot be more than iSize of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than iSize of list - Error message: %s\n", strerror(errno));
        return -1;
    }
    printSumForSpecificName(pstCurrent->psoFoodOrders, pszName);
    return 0;
}

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
        printf("Reservation number cannot be more than iSize of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pstNextReservation;
        iCounter++;
    }

    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than iSize of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (pstCurrent->pstPrevReservation == NULL) {
        pslList->pstHead = pstCurrent->pstNextReservation;
    } else {
        pstCurrent->pstPrevReservation->pstNextReservation = pstCurrent->pstNextReservation;
    }

    if (pstCurrent->pstNextReservation == NULL) {
        pslList->pstTail = pstCurrent->pstPrevReservation;
    } else {
        pstCurrent->pstNextReservation->pstPrevReservation = pstCurrent->pstPrevReservation;
    }

    if (pstCurrent->psoFoodOrders != NULL) {
        orderFreeLinkedList(pstCurrent->psoFoodOrders);
        free(pstCurrent->psoFoodOrders);
    }
    free(pstCurrent->pszName);

    fixReservationNumbersFromIndex(pslList, pstCurrent->iReservationNumber-1);

    free(pstCurrent);
    pslList->iSize--;

    return 0;
}

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
        printf("Reservation number cannot be more than iSize of list - Error message: %s\n", strerror(errno));
        return -1;
    }


    while (pstCurrent != NULL) {
        if(pstCurrent->iReservationNumber == iReservationNumber){
            break;
        }
        pstCurrent = pstCurrent->pstNextReservation;
    }
    if (pstCurrent == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than iSize of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    iStatus = orderAdd(pstCurrent->psoFoodOrders, pssSentOrder);
    if (iStatus != 0) {
        printf("Failed to add order - Error message: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}