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

#define SEATS 3
#define TIME 5
#define TABLE 3
#define RESERVATION_NUMBER 5

int addReservation(LIST *list);

int menuHandling(char *array[], char *inputArray, int sizeOfArray);

int addFoodToReservation(LIST *list);

int deleteReservation(LIST *list);

int getReservation(LIST *list, char *inputArray);

int makeOrder(SENT_ORDER *sentOrder);

void printListOptions(char *array[], int sizeOfArray);

int inputWithCharLimit(char *charArray, int lengthOfArray);

int askUserQuestion(char *title, char *inputArray, int expectedSize);

int yesOrNo();

int menuApplication() {
    int status = TRUE;
    char *array[] = {
            "Add a reservation",
            "get reservations",
            "Delete reservation",
            "Add food to reservation",
            "Print specific reservation with sum",
            "Print table sum for one name",
            "Quit"
    };

    int sizeOfArray = sizeof(array) / sizeof(char *);
    char *inputArray = (char *) malloc(sizeof(char) * 2);
    LIST list = {NULL, NULL, 0};

    while (status) {

        status = menuHandling(array, inputArray, sizeOfArray);

        if (status != TRUE) {

        } else {
            switch (*inputArray) {
                case '1':
                    status = addReservation(&list);
                    break;
                case '2':
                    memset(inputArray, 0, 2);
                    status = getReservation(&list, inputArray);
                    break;
                case '3':
                    status = deleteReservation(&list);
                    break;
                case '4':
                    addFoodToReservation(&list);
                    break;
                case '5':
                    //printSpecificReservationWithSum();
                    break;
                case '6':
                    //printTableSumFromOneName();
                    break;
                case '7':
                    status = FALSE;
                    break;
                default:
                    printf("Please insert a valid integer from \"1\" - \"%d\"\n", sizeOfArray);
                    break;
            }
        }

    }
    printAllNodes(&list);
    free(inputArray);
    freeLinkedList(&list);
    return TRUE;
}

int addFoodToReservation(LIST *list) {
    int status = TRUE;

    char *reservationNumber = (char *) malloc(sizeof(char) * RESERVATION_NUMBER);
    if (reservationNumber == NULL) {
        status = ERROR;
        errno = ENOMEM;
        printf("Memory allocation issue - Error message: %s\n", strerror(errno));
    } else {

        printf("What is the reservation number you are after?\n");
        status = askUserQuestion("Reservation number: ", reservationNumber, RESERVATION_NUMBER);
        if (status != TRUE) {
            if (status == QUIT) {
                printf("Returning\n");
            } else {
                printf("Error message: %s\n", strerror(errno));
            }
        } else {
            status = printSpecificReservationByReservationNumber(list, atoi(reservationNumber));
            if (status == ERROR) {
                
            } else {

                printf("Are you sure you want to add food to this reservation? y/n\n");
                status = yesOrNo();
                if (status == TRUE) {
                    printf("Adding food to reservation\n");
                    SENT_ORDER *sentOrder = malloc(sizeof(struct _SENT_ORDER));
                    if (sentOrder == NULL) {
                        status = ERROR;
                        errno = ENOMEM;
                        printf("Memory allocation issue - Error message: %s\n", strerror(errno));
                    } else {
                        status = makeOrder(sentOrder);
                        if (status == ERROR) {
                            free(sentOrder);
                        } else {
                            addFoodToSpecificReservation(list, atoi(reservationNumber), sentOrder);
                        }
                    }
                } else {
                    printf("Adding food cancelled...\n");
                    printf("Returning\n");
                }
            }
        }
        free(reservationNumber);
    }
    if (status == ERROR) {
        return ERROR;
    }
    return TRUE;
}

int makeOrder(SENT_ORDER *sentOrder) {
    int status = TRUE;
    char *name = (char *) malloc(sizeof(char) * USER_INPUT_SIZE);
    //char *foodDescription = (char *) malloc(sizeof(char) * USER_INPUT_SIZE);

    printf("Please insert the name of the person ordering\n");
    status = askUserQuestion("Name: ", name, USER_INPUT_SIZE);
    if (status != TRUE) {
        if (status == QUIT) {
            printf("Returning\n");
        } else {
            printf("Error message: %s\n", strerror(errno));
        }
    } else {
        sentOrder->name = (char *) malloc(sizeof(char) + strlen(name));
        strncpy(sentOrder->name, name, USER_INPUT_SIZE);
        memset(name, 0, USER_INPUT_SIZE);

        printf("Please insert the food description\n");
        status = askUserQuestion("Food description: ", name, USER_INPUT_SIZE);
        if (status != TRUE) {
            if (status == QUIT) {
                printf("Returning\n");
            } else {
                printf("Error message: %s\n", strerror(errno));
            }
        } else {
            sentOrder->foodDescription = (char *) malloc(sizeof(char) + strlen(name));
            strncpy(sentOrder->foodDescription, name, USER_INPUT_SIZE);
            memset(name, 0, USER_INPUT_SIZE);

            printf("Please insert the price of the food\n");
            status = askUserQuestion("Price: ", name, USER_INPUT_SIZE);
            if (status != TRUE) {
                if (status == QUIT) {
                    printf("Returning\n");
                } else {
                    printf("Error message: %s\n", strerror(errno));
                }
            } else {
                sentOrder->price = atoi(name);
            }
        }
    }
    free(name);
    if (status == ERROR) {
        return ERROR;
    }
    return TRUE;
}

int deleteReservation(LIST *list) {
    int status = TRUE;

    char *reservationNumber = (char *) malloc(sizeof(char) * RESERVATION_NUMBER);

    printf("What is the reservation number you are after?\n");
    status = askUserQuestion("Reservation number: ", reservationNumber, RESERVATION_NUMBER);
    if (status != TRUE) {
        if (status == QUIT) {
            printf("Returning\n");
        } else {
            printf("Error message: %s\n", strerror(errno));
        }
    } else {
        status = printSpecificReservationByReservationNumber(list, atoi(reservationNumber));
        if (status == ERROR) {
            return ERROR;
        }

        printf("Are you sure you want to delete this reservation? y/n\n");
        status = yesOrNo();
        if (status == TRUE) {
            printf("Deleting reservation\n");
            deleteSpecificReservation(list, atoi(reservationNumber));
        } else {
            printf("Deletion cancelled...\n");
            printf("Returning\n");
        }


    }
    free(reservationNumber);
    if (status == ERROR) {
        return ERROR;
    }
    return TRUE;
}

int getReservation(LIST *list, char *inputArray) {
    int status = TRUE;

    char *array[] = {
            "Get all reservations",
            "Get reservation by name",
            "Get reservation by reservation number",
            "Quit"
    };

    int sizeOfArray = sizeof(array) / sizeof(char *);
    char *name = (char *) malloc(sizeof(char) * USER_INPUT_SIZE);

    while (status == TRUE) {

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
                        printReservationByName(list, name);
                    }
                    break;
                case '3':
                    printf("What is the reservation number you are after?\n");
                    status = askUserQuestion("Reservation number: ", name, RESERVATION_NUMBER);
                    if (status != TRUE) {
                        if (status == QUIT) {
                            printf("Returning\n");
                        } else {
                            printf("Error message: %s\n", strerror(errno));
                        }
                    } else {
                        printSpecificReservationByReservationNumber(list, atoi(name));
                    }
                    break;
                case '4':
                    status = QUIT;
                    break;
                default:
                    printf("Please insert a valid integer from \"1\" - \"%d\"\n", sizeOfArray);
                    break;
            }
        }
    }
    printf("1\n");
    free(name);
    printf("2\n");

    if (status == ERROR) {
        return ERROR;
    }
    return TRUE;
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
                printf("Returning\n");
            } else {
                printf("Error message: %s\n", strerror(errno));
            }
        } else {

            printf("\nPlease insert the number of people in the reservation\n");
            char *tempSeats = (char *) malloc(sizeof(char) * SEATS);
            status = askUserQuestion("Seats: ", tempSeats, SEATS);
            if (status == QUIT) {
                printf("Returning\n");
            } else {

                printf("\nPlease insert the time of the reservation\n");
                char *tempTime = (char *) malloc(sizeof(char) * TIME);
                if (tempTime == NULL) {
                    status = ERROR;
                    errno = ENOMEM;
                    printf("Memory allocation issue - Error message: %s\n", strerror(errno));
                } else {
                    status = askUserQuestion("Time: ", tempTime, TIME);

                    if (status == QUIT) {
                        printf("Returning\n");
                    } else {


                        printf("\nPlease insert the table number of the reservation\n");
                        char *tableNumber = (char *) malloc(sizeof(char) * TABLE);

                        if (tableNumber == NULL) {
                            status = ERROR;
                            errno = ENOMEM;
                            printf("Memory allocation issue - Error message: %s\n", strerror(errno));
                        } else {

                            status = askUserQuestion("Table number: ", tableNumber, TABLE);
                            if (status == QUIT) {
                                printf("Returning\n");
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

    return TRUE;
}

int askUserQuestion(char *title, char *inputArray, int expectedSize) {
    int status = TRUE;

    while (1) {
        printf("%s", title);
        fflush(stdout);
        memset(inputArray, 0, expectedSize);
        status = inputWithCharLimit(inputArray, expectedSize);
        if (status != TRUE) {
            printf("Error message: %s\nTry again", strerror(errno));
            return ERROR;
        } else {
            printf("\nYou wrote: %s\nIs that right? y/n or q for quit\n", inputArray);
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
