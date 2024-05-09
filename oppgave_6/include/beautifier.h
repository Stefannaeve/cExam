#ifndef __OPPGAVE_6_BEAUTIFIER_H__
#define __OPPGAVE_6_BEAUTIFIER_H__

#include "../include/linkedList.h"

#define MAX_STRING_LENGTH 206

int removeEveryConcurrentlyTreeLinesOfSpace(NODE_LIST *psnList);

int changeAllCharVariableNamesToHungerianNotation(NODE_LIST *psnList);

int changeWhileLoopsToForLoops(NODE_LIST *list);

int checkIfLineHasComment(char *pszCurrentLine, int iSize);

int changeWhileToFor(NODE *psnCurrent, int iPositionOfWhile, int iNodePosition, NODE_LIST *psnList);

int findCondition(int *piPlacementOfFor, int *piNodeWithWhilePosition, int *piNodePosition, NODE_LIST *psnList, char *aszForCondition, int *piFoundWhile);

int findInitialization(NODE_LIST *psnList, int iNodeWithWhilePosition, int iNodePosition, char *aszForInitialization);

int findIncrement(NODE_LIST *psnList, int *iNodePosition, char *aszForIncrement);

int beautify(char *filename);

#endif //__OPPGAVE_6_BEAUTIFIER_H__
