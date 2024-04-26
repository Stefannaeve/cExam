#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include "tableReservationList.h"
#include "orderList.h"

void addToEnd(LIST *list, TABLERESERVATION *temp);
void addAtIndex(LIST *list, TABLERESERVATION *temp, int index);

int add(LIST *list, const SENT_TABLE_RESERVATION *sentTableReservation) {
    if (sentTableReservation == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL - Error message: %s\n", strerror(errno));
        return -1;
    }

    TABLERESERVATION *temp;
    int iLengthOfName = strlen(sentTableReservation->name);

    temp = (TABLERESERVATION *) malloc(sizeof(TABLERESERVATION) + iLengthOfName + 1);
    memset(temp, 0, sizeof(TABLERESERVATION) + iLengthOfName + 1);
    if (temp == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }

    temp->iTableNumber = sentTableReservation->iTableNumber;
    temp->seats = sentTableReservation->seats;
    temp->time = sentTableReservation->time;
    temp->name = (char *) malloc(iLengthOfName + 1);

    if (temp->name == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        free(temp);
        return -1;
    }
    strcpy(temp->name, sentTableReservation->name);

    temp->foodOrders = (ORDER_LIST *) malloc(sizeof(ORDER_LIST));
    if (temp->foodOrders == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        free(temp);
        return -1;
    }
    temp->foodOrders->pHead = NULL;
    temp->foodOrders->pTail = NULL;
    temp->foodOrders->size = 0;

    if (list->pHead == NULL) {
        list->pHead = temp;
        list->pTail = temp;
        temp->iReservationNumber = 1;
        return 0;
    }

    TABLERESERVATION *current = list->pHead;
    int counter = 0;
    while (current != NULL) {
        if (strcmp(current->name, temp->name) > 0) {
            addAtIndex(list, temp, counter);
            return 0;
        }
        current = current->pNextReservation;
        counter++;
    }
    addToEnd(list, temp);
    return 0;
}

void addToEnd(LIST *list, TABLERESERVATION *temp) {
    list->pTail->pNextReservation = temp;
    temp->iReservationNumber = list->pTail->iReservationNumber + 1;
    temp->pPrevReservation = list->pTail;
    list->pTail = temp;

    list->size++;
}

void addAtIndex(LIST *list, TABLERESERVATION *temp, int index) {

    TABLERESERVATION *current;
    int counter = 0;

    if (index == list->size + 1) {
        addToEnd(list, temp);
        return;
    }
    if (index == 0) {
        temp->pNextReservation = list->pHead;
        list->pHead->pPrevReservation = temp;
        list->pHead = temp;
        temp->iReservationNumber = 1;
    } else {
        current = list->pHead;
        while (current->pNextReservation != NULL && counter < index - 1) {
            current = current->pNextReservation;
            counter++;
        }
        temp->pNextReservation = current->pNextReservation;
        temp->pPrevReservation = current;
        temp->iReservationNumber = current->iReservationNumber + 1;
        if (current->pNextReservation != NULL) {
            current->pNextReservation->pPrevReservation = temp;
        }
        current->pNextReservation = temp;
    }
    fixReservationNumbersFromIndex(list, temp->iReservationNumber-1);
    list->size++;
}

void fixReservationNumbersFromIndex(LIST *list, int index) {
    TABLERESERVATION *current = list->pHead;
    int counter = 0;
    while (current != NULL) {
        if (counter >= index) {
            current->iReservationNumber = counter + 1;
        }
        current = current->pNextReservation;
        counter++;
    }
}

void freeLinkedList(LIST *list) {
    TABLERESERVATION *current = list->pHead;
    TABLERESERVATION *next;
    while (current != NULL) {
        next = current->pNextReservation;
        current->pNextReservation = NULL;
        if (current->foodOrders != NULL) {
            orderFreeLinkedList(current->foodOrders);
            free(current->foodOrders);
        }
        free(current->name);
        free(current);
        current = next;
    }
}

int printAllNodes(LIST *list) {
    TABLERESERVATION *current = list->pHead;
    if(current == NULL) {
        printf("You have no reservations\n");
        return -1;
    }
    while (current != NULL) {
        printf("Reservation Number: %d\n", current->iReservationNumber);
        printf("Name: %s\n", current->name);
        printf("  Table Number: %d\n", current->iTableNumber);
        printf("  Seats: %d\n", current->seats);
        printf("  Time: %d\n\n", current->time);
        current = current->pNextReservation;
    }
    printf("\n");
    return 0;
}

int printSpecificNodeAndFood(LIST *list, int reservationNumber) {
    int index = reservationNumber - 1;
    if (index < 0) {
        errno = EINVAL;
        printf("reservationNumber position cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    TABLERESERVATION *current = list->pHead;
    int counter = 0;
    while (current != NULL && counter < index) {
        current = current->pNextReservation;
        counter++;
    }
    if (current == NULL) {
        errno = ERANGE;
        printf("Index position cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }
    printf("Name: %s\n", current->name);
    printf("Reservation Number: %d\n", current->iReservationNumber);
    printf("  Table Number: %d\n", current->iTableNumber);
    printf("  Seats: %d\n", current->seats);
    printf("  Time: %d\n", current->time);
    orderPrintAllOrders(current->foodOrders);
    return 0;
}

int printReservationByName(LIST *list, const char *name) {
    int foundReservation = 0;

    if (name == NULL) {
        errno = EINVAL;
        printf("Name cannot be NULL - Error message: %s\n", strerror(errno));
        return -1;
    }

    TABLERESERVATION *current = list->pHead;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("Name: %s\n", current->name);
            printf("Reservation Number: %d\n", current->iReservationNumber);
            printf("  Table Number: %d\n", current->iTableNumber);
            printf("  Time: %d\n\n", current->time);
            foundReservation = 1;
        }
        current = current->pNextReservation;
    }
    if (foundReservation != 1) {
        errno = ENOENT;
        printf("Name not found - Error message: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int printReservationOrdersAndSum(LIST *list, const int reservationNumber) {
    if (reservationNumber < 1) {
        errno = EINVAL;
        printf("reservation number cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (reservationNumber - 1 > list->size) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    TABLERESERVATION *current = list->pHead;
    int counter = 0;
    while (current != NULL) {
        if(current->iReservationNumber == reservationNumber){
            break;
        }
        current = current->pNextReservation;
        counter++;
    }
    if (current == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }
    printf("Name: %s\n", current->name);
    printf("Reservation Number: %d\n", current->iReservationNumber);
    printf("  Table Number: %d\n", current->iTableNumber);
    printf("  Seats: %d\n", current->seats);
    printf("  Time: %d\n", current->time);
    orderPrintAllOrdersAndSum(current->foodOrders);
    return 0;
}

int deleteSpecificReservation(LIST *list, int reservationNumber) {
    int index = reservationNumber - 1;
    if (reservationNumber < 1) {
        errno = EINVAL;
        printf("Reservation number cannot be less than one - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (index > list->size) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    TABLERESERVATION *current = list->pHead;
    int counter = 0;

    while (current != NULL) {
        if(current->iTableNumber == reservationNumber){
            break;
        }
        current = current->pNextReservation;
        counter++;
    }

    if (current == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (current->pPrevReservation == NULL) {
        list->pHead = current->pNextReservation;
    } else {
        current->pPrevReservation->pNextReservation = current->pNextReservation;
    }

    if (current->pNextReservation == NULL) {
        list->pTail = current->pPrevReservation;
    } else {
        current->pNextReservation->pPrevReservation = current->pPrevReservation;
    }

    if (current->foodOrders != NULL) {
        orderFreeLinkedList(current->foodOrders);
        free(current->foodOrders);
    }
    free(current->name);

    fixReservationNumbersFromIndex(list, current->iReservationNumber-1);

    free(current);
    list->size--;

    return 0;
}

int addFoodToSpecificReservation(LIST *list, int reservationNumber, const SENT_ORDER *sentOrder) {
    if (sentOrder == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (reservationNumber < 1) {
        errno = EINVAL;
        printf("Reservation number cannot be less than one - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (reservationNumber - 1 > list->size) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }


    TABLERESERVATION *current = list->pHead;
    while (current != NULL) {
        if(current->iReservationNumber == reservationNumber){
            break;
        }
        current = current->pNextReservation;
    }
    if (current == NULL) {
        errno = ERANGE;
        printf("Reservation number cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    int status = orderAdd(current->foodOrders, sentOrder);
    if (status != 0) {
        printf("Failed to add order - Error message: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}