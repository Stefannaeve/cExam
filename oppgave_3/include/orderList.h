//
// Created by stefannaeve on 4/26/24.
//

#ifndef __TASK_ORDERLIST_H__
#define __TASK_ORDERLIST_H__

typedef struct _SENT_ORDER {
    char *name;
    char *foodDescription;
    int price;
} SENT_ORDER;

typedef struct _ORDER {
    struct _ORDER *pNextOrder;
    char *name;
    char *foodDescription;
    int price;
} ORDER;

typedef struct _ORDER_LIST {
    ORDER *pHead;
    ORDER *pTail;
    int size;
} ORDER_LIST;

int orderAdd(ORDER_LIST *list, SENT_ORDER *sentOrder);
void orderAddToEnd(ORDER_LIST *list, ORDER *temp);
void orderFreeLinkedList(ORDER_LIST *list);
void orderPrintAllOrders(ORDER_LIST *list);
void orderPrintAllOrdersAndSum(ORDER_LIST *list);
void printSumForSpecificName(ORDER_LIST *list, char *name);
int orderPrintSpecificNode(ORDER_LIST *list, int index);
int orderDeleteSpecificNode(ORDER_LIST *list, int index);

#endif //__TASK_ORDERLIST_H__
