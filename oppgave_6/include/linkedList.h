//
// Created by stefannaeve on 4/30/24.
//

#ifndef OPPGAVE_6_LINKEDLIST_H
#define OPPGAVE_6_LINKEDLIST_H

#define MAX_LINE_LENGTH 206

typedef struct _SENT_NODE {
    char *line;
} SENT_NODE;

typedef struct _NODE {
    struct _NODE *pNextNode;
    char line[MAX_LINE_LENGTH];
    int price;
} NODE;

typedef struct _NODE_LIST {
    NODE *pHead;
    NODE *pTail;
    int size;
} NODE_LIST;

void nodeAddToEnd(NODE_LIST *psoList, NODE *psnTemp);
int add(NODE_LIST *psnList, SENT_NODE *pssSentNode);
void orderPrintAllNodes(NODE_LIST *psnList);

#endif //OPPGAVE_6_LINKEDLIST_H
