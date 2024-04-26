//
// Created by stefannaeve on 4/26/24.
//

#ifndef TASK_ORDERLIST_H
#define TASK_ORDERLIST_H

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

int orderAdd(ORDER_LIST *list, const SENT_ORDER *sentOrder);
void orderAddToEnd(ORDER_LIST *list, ORDER *temp);
void orderFreeLinkedList(ORDER_LIST *list);
void orderPrintAllNodes(ORDER_LIST *list);
int orderPrintSpecificNode(ORDER_LIST *list, int index);
int orderDeleteSpecificNode(ORDER_LIST *list, int index);

#endif //TASK_ORDERLIST_H
