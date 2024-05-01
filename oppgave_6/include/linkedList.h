//
// Created by stefannaeve on 4/30/24.
//

#ifndef OPPGAVE_6_LINKEDLIST_H
#define OPPGAVE_6_LINKEDLIST_H

#define MAX_LINE_LENGTH 206

typedef struct _SENT_NODE {
    char *line;
    int size;
} SENT_NODE;

typedef struct _NODE {
    struct _NODE *pNextNode;
    char *line;
    int size;
} NODE;

typedef struct _NODE_LIST {
    NODE *pHead;
    NODE *pTail;
    int size;
} NODE_LIST;

void nodeAddToEnd(NODE_LIST *psnList, NODE *psnTemp);
int add(NODE_LIST *psnList, SENT_NODE *pssSentNode);
void printAllNodes(NODE_LIST *psnList);
void freeLinkedList(NODE_LIST *psnList);
int deleteSpecificNode(NODE_LIST *psnList, int iIndex);
void addAtIndex(NODE_LIST *psnList, SENT_NODE *pssSentNode, int iIndex);

#endif //OPPGAVE_6_LINKEDLIST_H
