#include "tableReservationList.h"
#include "orderList.h"

#ifndef __TASK_MENUAPPLICATION_H__
#define __TASK_MENUAPPLICATION_H__

#define ERROR -1
#define FALSE 0
#define TRUE 1
#define QUIT 2
#define USER_INPUT_SIZE 1024

#define SEATS 3
#define TIME 5
#define TABLE 3
#define RESERVATION_NUMBER 5

int menuApplication();

int addReservation(LIST *pslList);

int menuHandling(char *aszArray[], char *pszInputArray, int iSizeOfArray);

int addFoodToReservation(LIST *pslList);

int deleteReservation(LIST *pslList);

int getReservation(LIST *pslList, char *pszInputArray);

int printSpecificReservationWithSum(LIST *pslList);

int printSumForSpesificNameAtSpesificTable(LIST *pslList);

int makeOrder(SENT_ORDER *pssSentOrder);

void printListOptions(char *aszArray[], int iSizeOfArray);

int inputWithCharLimit(char *charArray, int iLengthOfArray);

int askUserQuestion(char *pszTitle, char *pszInputArray, int iExpectedSize);

int askUserQuestionInt(char *pszTitle, char *pszInputArray, int iExpectedSize);

int yesOrNo();

#endif //__TASK_MENUAPPLICATION_H__
