#include "orderList.h"

#ifndef __TASK_TABLERESERVATIONLIST_H__
#define __TASK_TABLERESERVATIONLIST_H__

typedef struct _SENT_TABLE_RESERVATION {
    int iTableNumber;
    int iSeats;
    int iTime;
    char *pszName;
} SENT_TABLE_RESERVATION;

typedef struct _TABLERESERVATION {
    struct _TABLERESERVATION *pstNextReservation;
    struct _TABLERESERVATION *pstPrevReservation;
    int iReservationNumber;
    int iTableNumber;
    int iSeats;
    int iTime;
    char *pszName;
    ORDER_LIST *psoFoodOrders;
} TABLERESERVATION;

typedef struct LIST {
    TABLERESERVATION *pstHead;
    TABLERESERVATION *pstTail;
    int iSize;
} LIST;

int add(LIST *list, const SENT_TABLE_RESERVATION *sentTableReservation);
int add(LIST *list, const SENT_TABLE_RESERVATION *sentTableReservation);
int addAt(LIST *list, const SENT_TABLE_RESERVATION *sentTableReservation, int indexOffset, int position);
void fixReservationNumbersFromIndex(LIST *list, int index);
void freeLinkedList(LIST *list);
int printAllNodes(LIST *list);
int printReservationOrdersForSpecificName(LIST *list, int iReservationNumber, char *name);
int printSpecificNodeAndFood(LIST *list, int reservationNumber);
int printReservationByName(LIST *list, const char *name);
int printSpecificReservationByReservationNumber(LIST *list, int reservationNumber);
int printReservationOrdersAndSum(LIST *list, const int reservationNumber);
int deleteSpecificReservation(LIST *list, int reservationNumber);
int addFoodToSpecificReservation(LIST *list, int reservationNumber, SENT_ORDER *sentOrder);
void addToEnd(LIST *list, TABLERESERVATION *temp);
void addAtIndex(LIST *list, TABLERESERVATION *temp, int index);

#endif //__TASK_TABLERESERVATIONLIST_H__
