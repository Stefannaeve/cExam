#include "orderList.h"

#ifndef __TASK_TABLERESERVATIONLIST_H__
#define __TASK_TABLERESERVATIONLIST_H__

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
    int iReservationNumber;
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
int add(LIST *list, const SENT_TABLE_RESERVATION *sentTableReservation);
int addAt(LIST *list, const SENT_TABLE_RESERVATION *sentTableReservation, int indexOffset, int position);
void fixReservationNumbersFromIndex(LIST *list, int index);
void freeLinkedList(LIST *list);
int printAllNodes(LIST *list);
int printReservationOrdersForSpecificName(LIST *list, int reservationNumber, char *name);
int printSpecificNodeAndFood(LIST *list, int reservationNumber);
int printReservationByName(LIST *list, const char *name);
int printSpecificReservationByReservationNumber(LIST *list, int reservationNumber);
int printReservationOrdersAndSum(LIST *list, const int reservationNumber);
int deleteSpecificReservation(LIST *list, int reservationNumber);
int addFoodToSpecificReservation(LIST *list, int reservationNumber, SENT_ORDER *sentOrder);

#endif //__TASK_TABLERESERVATIONLIST_H__
