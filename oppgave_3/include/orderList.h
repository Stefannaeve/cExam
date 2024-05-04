#ifndef __TASK_ORDERLIST_H__
#define __TASK_ORDERLIST_H__

typedef struct _SENT_ORDER {
    char *pszName;
    char *pszFoodDescription;
    int iPrice;
} SENT_ORDER;

typedef struct _ORDER {
    struct _ORDER *psoNextOrder;
    char *pszName;
    char *pszFoodDescription;
    int iPrice;
} ORDER;

typedef struct _ORDER_LIST {
    ORDER *psoHead;
    ORDER *psoTail;
    int iSize;
} ORDER_LIST;

int orderAdd(ORDER_LIST *list, SENT_ORDER *sentOrder);
void orderAddToEnd(ORDER_LIST *list, ORDER *temp);
void orderFreeLinkedList(ORDER_LIST *list);
void orderPrintAllOrders(ORDER_LIST *list);
void orderPrintAllOrdersAndSum(ORDER_LIST *list);
void printSumForSpecificName(ORDER_LIST *list, char *name);

#endif //__TASK_ORDERLIST_H__
