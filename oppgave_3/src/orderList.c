#include <malloc.h>
#include <string.h>
#include <errno.h>
#include "orderList.h"

#define START 0
#define END 1

int orderAdd(ORDER_LIST *list, const SENT_ORDER *sentOrder) {
    if (sentOrder == NULL) {
        errno = EINVAL;
        printf("Struct cannot be NULL- Error message: %s\n", strerror(errno));
        return -1;
    }

    ORDER *temp;
    int iLengthOfName = strlen(sentOrder->name);
    int iLengthOfFoodDescription = strlen(sentOrder->foodDescription);

    temp = (ORDER *) malloc(sizeof(ORDER) + iLengthOfName + 1 + iLengthOfFoodDescription + 1);
    if (temp == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }

    temp->name = (char *) malloc(iLengthOfName + 1);
    if (temp->name == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }
    strcpy(temp->name, sentOrder->name);

    temp->foodDescription = (char *) malloc(iLengthOfFoodDescription + 1);
    if (temp->foodDescription == NULL) {
        errno = ENOMEM;
        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
        return -1;
    }
    strcpy(temp->foodDescription, sentOrder->foodDescription);

    temp->price = sentOrder->price;

    temp->pNextOrder = NULL;

    if (list->pHead == NULL) {
        list->pHead = temp;
        list->pTail = temp;
        return 0;
    }

    orderAddToEnd(list, temp);

    return 0;
}

void orderAddToEnd(ORDER_LIST *list, ORDER *temp) {
    list->pTail->pNextOrder = temp;
    list->pTail = temp;

    list->size++;
}

void orderFreeLinkedList(ORDER_LIST *list) {
    ORDER *current = list->pHead;
    ORDER *next;
    while (current != NULL) {
        next = current->pNextOrder;
        current->pNextOrder = NULL;
        free(current);
        current = next;
    }
}

void orderPrintAllNodes(ORDER_LIST *list) {
    ORDER *current = list->pHead;
    while (current != NULL) {
        printf("Name: %s\n", current->name);
        printf("Food Description: %s\n", current->foodDescription);
        printf("Price: %d\n", current->price);
        current = current->pNextOrder;
    }
    printf("\n");
}

int orderPrintSpecificNode(ORDER_LIST *list, int index) {
    if (index < 0) {
        errno = EINVAL;
        printf("index position cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    ORDER *current = list->pHead;
    int counter = 0;
    while (current != NULL && counter < index) {
        current = current->pNextOrder;
        counter++;
    }
    if (current == NULL) {
        errno = ERANGE;
        printf("Index position cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    printf("Name: %s\n", current->name);
    printf("Food Description: %s\n", current->foodDescription);
    printf("Price: %d\n", current->price);

    return 0;
}

int orderDeleteSpecificNode(ORDER_LIST *list, int index) {
    if (index < 0) {
        errno = EINVAL;
        printf("index position cannot be negative - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (index > list->size) {
        errno = ERANGE;
        printf("Index position cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    ORDER *current = list->pHead;
    ORDER *previous = NULL;
    int counter = 0;

    while (current != NULL && counter < index) {
        previous = current;
        current = current->pNextOrder;
        counter++;
    }

    if (current == NULL) {
        errno = ERANGE;
        printf("Index position cannot be more than size of list - Error message: %s\n", strerror(errno));
        return -1;
    }

    if (previous == NULL) {
        list->pHead = current->pNextOrder;
    } else {
        previous->pNextOrder = current->pNextOrder;
    }

    free(current);
    list->size--;

    return 0;
}