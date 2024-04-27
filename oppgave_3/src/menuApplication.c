#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "menuApplication.h"
#include "orderList.h"
#include "tableReservationList.h"

#define ERROR -1
#define FALSE 0
#define TRUE 1
#define QUIT 2
#define USER_INPUT_SIZE 1024

#define SEATS 3
#define TIME 5
#define TABLE 3
#define RESERVATION_NUMBER 5

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

int yesOrNo();

int menuApplication() {
    char *array[] = {
            "Add a reservation",
            "get reservations",
            "Delete reservation",
            "Add food to reservation",
            "Print specific reservation with sum",
            "Print table sum for one name",
            "Quit"
    };
    int iStatus = TRUE;
    int iSizeOfArray = sizeof(array) / sizeof(char *);
    char *pszInputArray = (char *) malloc(sizeof(char) * 2);

    LIST slList = {NULL, NULL, 0};

    while (iStatus == TRUE) {

        iStatus = menuHandling(array, pszInputArray, iSizeOfArray);

        if (iStatus != TRUE) {

        } else {
            switch (*pszInputArray) {
                case '1':
                    iStatus = addReservation(&slList);
                    break;
                case '2':
                    memset(pszInputArray, 0, 2);
                    iStatus = getReservation(&slList, pszInputArray);
                    break;
                case '3':
                    iStatus = deleteReservation(&slList);
                    break;
                case '4':
                    iStatus = addFoodToReservation(&slList);
                    break;
                case '5':
                    iStatus = printSpecificReservationWithSum(&slList);
                    break;
                case '6':
                    iStatus = printSumForSpesificNameAtSpesificTable(&slList);
                    break;
                case '7':
                    iStatus = FALSE;
                    break;
                default:
                    printf("Please insert a valid integer from \"1\" - \"%d\"\n", iSizeOfArray);
                    iStatus = TRUE;
                    break;
            }
        }

    }
    free(pszInputArray);
    freeLinkedList(&slList);
    if (iStatus == ERROR) {
        return ERROR;
    }
    return 0;
}

int printSumForSpesificNameAtSpesificTable(LIST *pslList){
    int iStatus = TRUE;
    char *pszReservationNumber = (char *) malloc(sizeof(char) * USER_INPUT_SIZE);
    if (pszReservationNumber == NULL) {
        iStatus = ERROR;
        errno = ENOMEM;
        printf("Memory allocation issue - Error message: %s\n", strerror(errno));
    } else {
        char *pszName = (char *) malloc(sizeof(char) * USER_INPUT_SIZE);
        if (pszName == NULL) {
            iStatus = ERROR;
            errno = ENOMEM;
            printf("Memory allocation issue - Error message: %s\n", strerror(errno));
        } else {
            printf("What is the reservation number you are after?\n");
            iStatus = askUserQuestion("Reservation number: ", pszReservationNumber, RESERVATION_NUMBER);
            if (iStatus != TRUE) {
                if (iStatus == QUIT) {
                    printf("Returning\n");
                } else {
                    printf("Error message: %s\n", strerror(errno));
                }
            } else {
                printf("What is the name you are after?\n");
                iStatus = askUserQuestion("Name: ", pszName, USER_INPUT_SIZE);
                if (iStatus != TRUE) {
                    if (iStatus == QUIT) {
                        printf("Quitting\n");
                    } else {
                        printf("Error message: %s\n", strerror(errno));
                    }
                } else {
                    printReservationOrdersForSpecificName(pslList, atoi(pszReservationNumber), pszName);
                }
            }
            free(pszName);
        } // NAME
        free(pszReservationNumber);
    } // RESERVATION NUMBER
    if (iStatus == ERROR) {
        return ERROR;
    }
    return TRUE;
}

int printSpecificReservationWithSum(LIST *pslList) {
    int iStatus = TRUE;
    char *pszReservationNumber = (char *) malloc(sizeof(char) * USER_INPUT_SIZE);
    if (pszReservationNumber == NULL) {
        errno = ENOMEM;
        printf("Memory allocation issue - Error message: %s\n", strerror(errno));
        iStatus = ERROR;
    } else {
        printf("What is the reservation number you are after?\n");
        iStatus = askUserQuestion("Reservation number: ", pszReservationNumber, RESERVATION_NUMBER);
        if (iStatus != TRUE) {
            if (iStatus == QUIT) {
                printf("Returning\n");
            } else {
                printf("Error message: %s\n", strerror(errno));
            }
        } else {
            printReservationOrdersAndSum(pslList, atoi(pszReservationNumber));
        }
        free(pszReservationNumber);
    }
    if (iStatus == ERROR) {
        return ERROR;
    }
    return TRUE;
}

int addFoodToReservation(LIST *pslList) {
    int iStatus = TRUE;

    char *pszReservationNumber = (char *) malloc(sizeof(char) * RESERVATION_NUMBER);
    if (pszReservationNumber == NULL) {
        iStatus = ERROR;
        errno = ENOMEM;
        printf("Memory allocation issue - Error message: %s\n", strerror(errno));
    } else {

        printf("What is the reservation number you are after?\n");
        iStatus = askUserQuestion("Reservation number: ", pszReservationNumber, RESERVATION_NUMBER);
        if (iStatus != TRUE) {
            if (iStatus == QUIT) {
                printf("Returning\n");
            } else {
                printf("Error message: %s\n", strerror(errno));
            }
        } else {
            iStatus = printSpecificReservationByReservationNumber(pslList, atoi(pszReservationNumber));
            if (iStatus == ERROR) {
                
            } else {

                printf("Are you sure you want to add food to this reservation? y/n\n");
                iStatus = yesOrNo();
                if (iStatus == TRUE) {
                    printf("Adding food to reservation\n");
                    SENT_ORDER *pssSentOrder = malloc(sizeof(struct _SENT_ORDER));
                    if (pssSentOrder == NULL) {
                        iStatus = ERROR;
                        errno = ENOMEM;
                        printf("Memory allocation issue - Error message: %s\n", strerror(errno));
                    } else {
                        iStatus = makeOrder(pssSentOrder);
                        if (iStatus == ERROR) {
                            free(pssSentOrder);
                        } else {
                            addFoodToSpecificReservation(pslList, atoi(pszReservationNumber), pssSentOrder);
                        }
                    }
                } else {
                    printf("Adding food cancelled...\n");
                    printf("Returning\n");
                }
            }
        }
        free(pszReservationNumber);
    }
    if (iStatus == ERROR) {
        return ERROR;
    }
    return TRUE;
}

int makeOrder(SENT_ORDER *pssSentOrder) {
    int iStatus = TRUE;
    char *pszName = (char *) malloc(sizeof(char) * USER_INPUT_SIZE);

    printf("Please insert the name of the person ordering\n");
    iStatus = askUserQuestion("Name: ", pszName, USER_INPUT_SIZE);
    if (iStatus != TRUE) {
        if (iStatus == QUIT) {
            printf("Returning\n");
        } else {
            printf("Error message: %s\n", strerror(errno));
        }
    } else {
        pssSentOrder->name = (char *) malloc(sizeof(char) * strlen(pszName) + 1) ;
        strncpy(pssSentOrder->name, pszName, strlen(pszName));
        pssSentOrder->name[strlen(pszName)] = '\0';
        memset(pszName, 0, USER_INPUT_SIZE);

        printf("Please insert the food description\n");
        iStatus = askUserQuestion("Food description: ", pszName, USER_INPUT_SIZE);
        if (iStatus != TRUE) {
            if (iStatus == QUIT) {
                printf("Returning\n");
            } else {
                printf("Error message: %s\n", strerror(errno));
            }
        } else {
            pssSentOrder->foodDescription = (char *) malloc(sizeof(char) * strlen(pszName) + 1);
            strncpy(pssSentOrder->foodDescription, pszName, strlen(pszName));
            pssSentOrder->foodDescription[strlen(pszName)] = '\0';
            memset(pszName, 0, USER_INPUT_SIZE);

            printf("Please insert the price of the food\n");
            iStatus = askUserQuestion("Price: ", pszName, USER_INPUT_SIZE);
            if (iStatus != TRUE) {
                if (iStatus == QUIT) {
                    printf("Returning\n");
                } else {
                    printf("Error message: %s\n", strerror(errno));
                }
            } else {
                pssSentOrder->price = atoi(pszName);
            }
        }
    }
    free(pszName);
    if (iStatus == ERROR) {
        return ERROR;
    }
    return TRUE;
}

int deleteReservation(LIST *pslList) {
    int iStatus = TRUE;

    char *pszReservationNumber = (char *) malloc(sizeof(char) * RESERVATION_NUMBER);

    printf("What is the reservation number you are after?\n");
    iStatus = askUserQuestion("Reservation number: ", pszReservationNumber, RESERVATION_NUMBER);
    if (iStatus != TRUE) {
        if (iStatus == QUIT) {
            printf("Returning\n");
        } else {
            printf("Error message: %s\n", strerror(errno));
        }
    } else {
        iStatus = printSpecificReservationByReservationNumber(pslList, atoi(pszReservationNumber));
        if (iStatus == ERROR) {
            return ERROR;
        }

        printf("Are you sure you want to delete this reservation? y/n\n");
        iStatus = yesOrNo();
        if (iStatus == TRUE) {
            printf("Deleting reservation\n");
            deleteSpecificReservation(pslList, atoi(pszReservationNumber));
        } else {
            printf("Deletion cancelled...\n");
            printf("Returning\n");
        }


    }
    free(pszReservationNumber);
    if (iStatus == ERROR) {
        return ERROR;
    }
    return TRUE;
}

int getReservation(LIST *pslList, char *pszInputArray) {
    int iStatus = TRUE;

    char *array[] = {
            "Get all reservations",
            "Get reservation by name",
            "Get reservation by reservation number",
            "Quit"
    };

    int iSizeOfArray = sizeof(array) / sizeof(char *);
    char *pszName = (char *) malloc(sizeof(char) * USER_INPUT_SIZE);

    while (iStatus == TRUE) {

        char result = menuHandling(array, pszInputArray, iSizeOfArray);

        if (result != TRUE) {

        } else {
            switch (*pszInputArray) {
                case '1':
                    printAllNodes(pslList);
                    break;
                case '2':
                    printf("What is the name you are after?\n");
                    iStatus = askUserQuestion("Name: ", pszName, USER_INPUT_SIZE);
                    if (iStatus != TRUE) {
                        if (iStatus == QUIT) {
                            printf("Quitting\n");
                        } else {
                            printf("Error message: %s\n", strerror(errno));
                        }
                    } else {
                        printReservationByName(pslList, pszName);
                    }
                    break;
                case '3':
                    printf("What is the reservation number you are after?\n");
                    iStatus = askUserQuestion("Reservation number: ", pszName, RESERVATION_NUMBER);
                    if (iStatus != TRUE) {
                        if (iStatus == QUIT) {
                            printf("Returning\n");
                        } else {
                            printf("Error message: %s\n", strerror(errno));
                        }
                    } else {
                        printSpecificReservationByReservationNumber(pslList, atoi(pszName));
                    }
                    break;
                case '4':
                    iStatus = QUIT;
                    break;
                default:
                    printf("Please insert a valid integer from \"1\" - \"%d\"\n", iSizeOfArray);
                    break;
            }
        }
    }
    printf("1\n");
    free(pszName);
    printf("2\n");

    if (iStatus == ERROR) {
        return ERROR;
    }
    return TRUE;
}

int addReservation(LIST *pslList) {
    int iTable = 0;
    int iSeats = 0;
    int iTime = 0;
    int iStatus = TRUE;
    char *pszName = NULL;
    char *pszTempSeats = NULL;
    char *pszTempTime = NULL;
    char *pszTableNumber = NULL;

    printf("You will be asked to fill in a various number of values\n");
    printf("Please insert the name of the reservation holder\n");
    pszName = (char *) malloc(sizeof(char) * USER_INPUT_SIZE);
    if (pszName == NULL) {
        iStatus = ERROR;
        errno = ENOMEM;
        printf("Memory allocation issue - Error message: %s\n", strerror(errno));
    } else {
        iStatus = askUserQuestion("Name: ", pszName, USER_INPUT_SIZE);
        if (iStatus != TRUE) {
            if (iStatus == QUIT) {
                printf("Returning\n");
            } else {
                printf("Error message: %s\n", strerror(errno));
            }
        } else {

            printf("\nPlease insert the number of people in the reservation\n");
            pszTempSeats = (char *) malloc(sizeof(char) * SEATS);
            iStatus = askUserQuestion("Seats: ", pszTempSeats, SEATS);
            if (iStatus == QUIT) {
                printf("Returning\n");
            } else {

                printf("\nPlease insert the time of the reservation\n");
                pszTempTime = (char *) malloc(sizeof(char) * TIME);
                if (pszTempTime == NULL) {
                    iStatus = ERROR;
                    errno = ENOMEM;
                    printf("Memory allocation issue - Error message: %s\n", strerror(errno));
                } else {
                    iStatus = askUserQuestion("Time: ", pszTempTime, TIME);

                    if (iStatus == QUIT) {
                        printf("Returning\n");
                    } else {
                        printf("\nPlease insert the table number of the reservation\n");
                        pszTableNumber = (char *) malloc(sizeof(char) * TABLE);

                        if (pszTableNumber == NULL) {
                            iStatus = ERROR;
                            errno = ENOMEM;
                            printf("Memory allocation issue - Error message: %s\n", strerror(errno));
                        } else {

                            iStatus = askUserQuestion("Table number: ", pszTableNumber, TABLE);
                            if (iStatus == QUIT) {
                                printf("Returning\n");
                            } else {

                                iTime = atoi(pszTempTime);
                                iTable = atoi(pszTableNumber);
                                iSeats = atoi(pszTempSeats);

                                SENT_TABLE_RESERVATION sentTableReservation = {iTable, iSeats, iTime,
                                                                               pszName};
                                add(pslList, &sentTableReservation);
                            }
                            free(pszTableNumber);
                        } // TABLE NUMBER
                    }
                    free(pszTempTime);
                } // TIME
            }
            free(pszTempSeats);
        } // SEATS
        free(pszName);
    } // NAME
    if (iStatus == ERROR) {
        return ERROR;
    }

    return TRUE;
}

int askUserQuestion(char *pszTitle, char *pszInputArray, int iExpectedSize) {
    int status = TRUE;

    while (1) {
        printf("%s", pszTitle);
        fflush(stdout);
        memset(pszInputArray, 0, iExpectedSize);
        status = inputWithCharLimit(pszInputArray, iExpectedSize);
        if (status != TRUE) {
            printf("Error message: %s\nTry again", strerror(errno));
            return ERROR;
        } else {
            printf("\nYou wrote: %s\nIs that right? y/n or q for quit\n", pszInputArray);
            status = yesOrNo();
            if (status != TRUE) {
                if (status == QUIT) {
                    return QUIT;
                }
                printf("\nTry again\n");
            } else {
                return TRUE;
            }
        }
    }
}

int yesOrNo() {
    char pszCharacter[2];
    pszCharacter[1] = '\0';
    while (1) {
        inputWithCharLimit(pszCharacter, 2);
        switch (pszCharacter[0]) {
            case 'y':
                return TRUE;
            case 'n':
                return FALSE;
            case 'q':
                printf("Are you sure you want to quit? y/n\n");
                inputWithCharLimit(pszCharacter, 2);
                if (pszCharacter[0] == 'y') {
                    return QUIT;
                } else {
                    return FALSE;
                }
            default:
                printf("Wrong input, it has to be \"y\" or \"n\"\n");
                break;
        }
    }
}

int menuHandling(char *aszArray[], char *szInputArray, int iSizeOfArray) {

    printf("Type the command you want [1-%d]:\n", iSizeOfArray);

    printListOptions(aszArray, iSizeOfArray);

    int result = inputWithCharLimit(szInputArray, 2);
    if (result != TRUE) {
        printf("Issue with getting stream from user - Error message: %s", strerror(errno));
        return ERROR;
    }

    return TRUE;
}

void printListOptions(char *aszArray[], int iSizeOfArray) {
    for (int i = 0; i < iSizeOfArray; ++i) {
        printf("%d. %s\n", i + 1, aszArray[i]);
    }
}

int inputWithCharLimit(char *pszCharArray, int iLengthOfArray) {
    if (pszCharArray == NULL) {
        errno = ENOMEM;
        printf("Memory allocation issue, given array is NULL - Error message: \n", strerror(errno));
        return ERROR;
    }

    if (iLengthOfArray > USER_INPUT_SIZE) {
        errno = EINVAL;
        printf("Length of array is not to exceed %d - Error message: \n", USER_INPUT_SIZE, strerror(errno));
        return ERROR;
    }

    char szUserInput[USER_INPUT_SIZE] = {0};
    fgets(szUserInput, USER_INPUT_SIZE - 1, stdin);

    while (szUserInput[strlen(szUserInput) - 1] == '\r' || szUserInput[strlen(szUserInput) - 1] == '\n') {
        szUserInput[strlen(szUserInput) - 1] = 0;
    }

    strncpy(pszCharArray, szUserInput, iLengthOfArray);

    pszCharArray[iLengthOfArray - 1] = '\0';

    return TRUE;
}
