#include <stdio.h>
#include <malloc.h>
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

int addReservation(LIST *list);

int menuHandling(char *array[], char *inputArray, int sizeOfArray);

void printListOptions(char *array[], int sizeOfArray);

int inputWithCharLimit(char *charArray, int lengthOfArray);

int askUserQuestion(char *title, char *inputArray, int expectedSize);

int yesOrNo();

int menuApplication() {
    char *array[] = {
            "Add a reservation",
            "get reservation by reservation number",
            "get reservation by name",
            "Delete reservation",
            "Add food to reservation",
            "Print specific reservation with sum",
            "Print table sum for one name",
            "Quit"
    };

    int sizeOfArray = sizeof(array) / sizeof(char *);
    char *inputArray = (char *) malloc(sizeof(char) * 2);
    LIST list = {NULL, NULL, 0};

    while (TRUE) {

        char result = menuHandling(array, inputArray, sizeOfArray);

        if (result != TRUE) {

        } else {
            switch (*inputArray) {
                case '1':
                    printf("1\n");
                    addReservation(&list);
                    break;
                case '2':
                    printf("2\n");
                    memset(inputArray, 0, 2);
                    //getReservation(&list, inputArray, sizeOfArray);
                    break;
                case '3':
                    printf("3\n");
                    //getReservationByName();
                    break;
                case '4':
                    //deleteReservation();
                    break;
                case '5':
                    //addFoodToReservation();
                    break;
                case '6':
                    //printSpecificReservationWithSum();
                    break;
                case '7':
                    //printTableSumFromOneName();
                    break;
                case '8':
                    printAllNodes(&list);
                    free(inputArray);
                    freeLinkedList(&list);
                    return TRUE;
                default:
                    printf("Please insert a valid integer from \"1\" - \"8\"\n");
                    break;
            }
        }

    }
    return TRUE;
}

int getReservation(LIST *list, char *inputArray, int sizeOfArray) {
    int status = TRUE;

    char *array[] = {
            "See all reservations",
            "See reservation by name",
            "See reservation by table number",
            "See reservation by reservation number"
    };

    while (TRUE) {

        char result = menuHandling(array, inputArray, sizeOfArray);

        if (result != TRUE) {

        } else {
            switch (*inputArray) {
                case '1':
                    printAllNodes(list);
                    break;
                case '2':
                    printf("What is the name you are after?\n");
                    status = askUserQuestion("Name: ", name, USER_INPUT_SIZE);
                    if (status != TRUE) {
                        if (status == QUIT) {
                            printf("Quitting\n");
                        } else {
                            printf("Error message: %s\n", strerror(errno));
                        }
                    } else {
                    printReservationByName(list, inputArray);
                    break;
                case '3':
                    printf("3\n");
                    //getReservationByName();
                    break;
                case '4':
                    //deleteReservation();
                    break;
                case '5':
                    //addFoodToReservation();
                    break;
                case '6':
                    //printSpecificReservationWithSum();
                    break;
                case '7':
                    //printTableSumFromOneName();
                    break;
                case '8':
                    printAllNodes(&list);
                    free(inputArray);
                    freeLinkedList(&list);
                    return TRUE;
                default:
                    printf("Please insert a valid integer from \"1\" - \"8\"\n");
                    break;
            }
        }

    }
}

int addReservation(LIST *list) {
    int table = 0;
    int seats = 0;
    int time = 0;
    int status = TRUE;

    printf("You will be asked to fill in a various number of values\n");
    printf("Please insert the name of the reservation holder\n");
    char *name = (char *) malloc(sizeof(char) * USER_INPUT_SIZE);
    if (name == NULL) {
        status = ERROR;
        errno = ENOMEM;
        printf("Memory allocation issue - Error message: %s\n", strerror(errno));
    } else {
        status = askUserQuestion("Name: ", name, USER_INPUT_SIZE);
        if (status != TRUE) {
            if (status == QUIT) {
                printf("Quitting\n");
            } else {
                printf("Error message: %s\n", strerror(errno));
            }
        } else {

            printf("\nPlease insert the number of people in the reservation\n");
            char *tempSeats = (char *) malloc(sizeof(char) * 3);
            status = askUserQuestion("Seats: ", tempSeats, 3);
            if (status == QUIT) {
                printf("Quitting\n");
            } else {

                printf("\nPlease insert the time of the reservation\n");
                char *tempTime = (char *) malloc(sizeof(char) * 5);
                if (tempTime == NULL) {
                    status = ERROR;
                    errno = ENOMEM;
                    printf("Memory allocation issue - Error message: %s\n", strerror(errno));
                } else {
                    status = askUserQuestion("Time: ", tempTime, 5);

                    if (status == QUIT) {
                        printf("Quitting\n");
                    } else {


                        printf("\nPlease insert the table number of the reservation\n");
                        char *tableNumber = (char *) malloc(sizeof(char) * 2);

                        if (tableNumber == NULL) {
                            status = ERROR;
                            errno = ENOMEM;
                            printf("Memory allocation issue - Error message: %s\n", strerror(errno));
                        } else {

                            status = askUserQuestion("Table number: ", tableNumber, 2);
                            if (status == QUIT) {
                                printf("Quitting\n");
                            } else {

                                time = atoi(tempTime);
                                table = atoi(tableNumber);
                                seats = atoi(tempSeats);

                                SENT_TABLE_RESERVATION sentTableReservation = {table, seats, time,
                                                                               name};
                                add(list, &sentTableReservation);
                            }
                            free(tableNumber);
                        } // TABLE NUMBER
                    }
                    free(tempTime);
                } // TIME
            }
            free(tempSeats);
        } // SEATS
        free(name);
    } // NAME
    if (status == ERROR) {
        return ERROR;
    }
    if (status == QUIT) {
        return QUIT;
    }
    return TRUE;
}

int askUserQuestion(char *title, char *inputArray, int expectedSize) {
    while (1) {
        printf("%s", title);
        fflush(stdout);
        int result = inputWithCharLimit(inputArray, expectedSize);
        if (result != TRUE) {
            printf("Error message: %s\nTry again", strerror(errno));
            return ERROR;
        } else {
            printf("\nYou wrote: %s\nIs that right? y/n or q for quit\n", inputArray);
            int status = yesOrNo();
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
    char character[2];
    while (1) {
        inputWithCharLimit(character, 2);
        switch (character[0]) {
            case 'y':
                return TRUE;
            case 'n':
                return FALSE;
            case 'q':
                printf("Are you sure you want to quit? y/n\n");
                inputWithCharLimit(character, 2);
                if (character[0] == 'y') {
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

int menuHandling(char *array[], char *inputArray, int sizeOfArray) {

    printf("Type the command you want [1-%d]:\n", sizeOfArray);

    printListOptions(array, sizeOfArray);

    int result = inputWithCharLimit(inputArray, 2);
    if (result != TRUE) {
        printf("Issue with getting stream from user - Error message: %s", strerror(errno));
        return FALSE;
    }

    return TRUE;
}

void printListOptions(char *array[], int sizeOfArray) {
    for (int i = 0; i < sizeOfArray; ++i) {
        printf("%d. %s\n", i + 1, array[i]);
    }
}

int inputWithCharLimit(char *charArray, int lengthOfArray) {
    if (charArray == NULL) {
        errno = ENOMEM;
        printf("Memory allocation issue, given array is NULL - Error message: \n", strerror(errno));
        return FALSE;
    }

    if (lengthOfArray > USER_INPUT_SIZE) {
        errno = EINVAL;
        printf("Length of array is not to exceed %d - Error message: \n", USER_INPUT_SIZE, strerror(errno));
        return FALSE;
    }

    char szUserInput[USER_INPUT_SIZE] = {0};
    fgets(szUserInput, USER_INPUT_SIZE - 1, stdin);

    while (szUserInput[strlen(szUserInput) - 1] == '\r' || szUserInput[strlen(szUserInput) - 1] == '\n') {
        szUserInput[strlen(szUserInput) - 1] = 0;
    }

    strncpy(charArray, szUserInput, lengthOfArray);

    charArray[lengthOfArray - 1] = '\0';

    return TRUE;
}