#include "tableReservationList.h"


typedef struct _TABLERESERVATION {
    struct _TABLERESERVATION *pNextReservation;
    struct _TABLERESERVATION *pPrevReservation;
    int iTableNumber;
    int seats;
    int time;
    char *name;
    ORDERS foodOrders;
} TABLERESERVATION;

typedef struct _LIST {
    TABLERESERVATION *pHead;
    TABLERESERVATION *pTail;
    int size;
} LIST;