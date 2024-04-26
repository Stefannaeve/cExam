#include "orderList.h"

#ifndef TASK_TABLERESERVATIONLIST_H
#define TASK_TABLERESERVATIONLIST_H

#define START 0
#define END 1

typedef struct _SENT_TABLE_RESERVATION {
    int iTableNumber;
    int seats;
    int time;
    char *name;
} SENT_TABLE_RESERVATION;

typedef struct _TABLERESERVATION {
    struct _TABLERESERVATION *pNextReservation;
    struct _TABLERESERVATION *pPrevReservation;
    int iTableNumber;
    int seats;
    int time;
    char *name;
    ORDER_LIST *foodOrders;
} TABLERESERVATION;

typedef struct LIST {
    TABLERESERVATION *pHead;
    TABLERESERVATION *pTail;
    int size;
} LIST;

int add(LIST *list, const SENT_TABLE_RESERVATION *sentTableReservation);
int addAt(LIST *list, const SENT_TABLE_RESERVATION *sentTableReservation, int indexOffset, int position);
void freeLinkedList(LIST *list);
int printAllNodes(LIST *list);
void printAllNodesBackwards(LIST *list);
int printSpecificNodeAndFood(LIST *list, int tableNumber);
int deleteSpecificReservation(LIST *list, int tableNumber);
int addFoodToSpecificReservation(LIST *list, int tableNumber, const SENT_ORDER *sentOrder);

#endif //TASK_TABLERESERVATIONLIST_H
