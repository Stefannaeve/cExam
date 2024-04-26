#include <stdio.h>
#include "tableReservationList.h"
#include "orderList.h"

int main(int argc, char *argv[]){
    if (argc != 1){
        printf("%s takes no arguments\n", argv[0]);
        return -1;
    }

    LIST list = {NULL, NULL, 0};
    SENT_TABLE_RESERVATION sentTableReservation = {1, 4, 12, "John"};

    add(&list, &sentTableReservation);
    SENT_ORDER sentOrder = {"Gunnar", "Biff", 230};
    addFoodToSpecificReservation(&list, 1, &sentOrder);
    printAllNodes(&list);
    printSpecificNodeAndFood(&list, 1);

    return 0;
}