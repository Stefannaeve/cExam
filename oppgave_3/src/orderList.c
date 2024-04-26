#include <malloc.h>
#include <string.h>

#define START 0
#define END 1

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

typedef struct _LIST {
    ORDER *pHead;
    ORDER *pTail;
    int size;
} LIST;

void addToEnd(LIST *list, ORDER *temp);
void freeLinkedList(LIST *list);
void printAllNodes(LIST *list);
int printSpecificNode(LIST *list, int index);
int deleteSpecificNode(LIST *list, int index);

int addAt(LIST *list, const SENT_ORDER *sentOrder) {
    if (sentOrder == NULL) {
        printf("Invalid input\n");
        return 1;
    }

    ORDER *temp;
    int iLengthOfName = strlen(sentOrder->name);
    int iLengthOfFoodDescription = strlen(sentOrder->foodDescription);

    temp = (ORDER *) malloc(sizeof(ORDER) + iLengthOfName + 1 + iLengthOfFoodDescription + 1);
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    temp->name = (char *) malloc(iLengthOfName + 1);
    if (temp->name == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    strcpy(temp->name, sentOrder->name);

    temp->foodDescription = (char *) malloc(iLengthOfFoodDescription + 1);
    if (temp->foodDescription == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    strcpy(temp->foodDescription, sentOrder->foodDescription);

    temp->price = sentOrder->price;

    temp->pNextOrder = NULL;

    if (list->pHead == NULL) {
        list->pHead = temp;
        list->pTail = temp;
        return 0;
    }

    addToEnd(list, temp);

    return 0;
}

void addToEnd(LIST *list, ORDER *temp) {
    list->pTail->pNextOrder = temp;
    list->pTail = temp;

    list->size++;
}

void freeLinkedList(LIST *list) {
    ORDER *current = list->pHead;
    ORDER *next;
    while (current != NULL) {
        next = current->pNextOrder;
        current->pNextOrder = NULL;
        free(current);
        current = next;
    }
}

void printAllNodes(LIST *list) {
    ORDER *current = list->pHead;
    while (current != NULL) {
        printf("Name: %s\n", current->name);
        printf("Food Description: %s\n", current->foodDescription);
        printf("Price: %d\n", current->price);
        current = current->pNextOrder;
    }
    printf("\n");
}

int printSpecificNode(LIST *list, int index) {
    if (index < 0) {
        printf("Error: Index cannot be negative!\n");
        return -1;
    }

    ORDER *current = list->pHead;
    int counter = 0;
    while (current != NULL && counter < index) {
        current = current->pNextOrder;
        counter++;
    }
    if (current == NULL) {
        printf("Index out of range!\n");
        return -1;
    }

    printf("Name: %s\n", current->name);
    printf("Food Description: %s\n", current->foodDescription);
    printf("Price: %d\n", current->price);

    return 0;
}

int deleteSpecificNode(LIST *list, int index) {
    if (index < 0) {
        printf("Error: Index cannot be negative!\n");
        return 1;
    }

    if (index > list->size) {
        printf("Error: Index out of range!\n");
        return 1;
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
        printf("Error: Index out of range!\n");
        return 1;
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