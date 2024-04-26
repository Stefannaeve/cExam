#include <stdio.h>
#include "tableReservationList.h"
#include "orderList.h"

int main(int argc, char *argv[]){
    if (argc != 1){
        printf("%s takes no arguments\n", argv[0]);
        return -1;
    }

    LIST list = {NULL, NULL, 0};
    SENT_TABLE_RESERVATION sentTableReservation = { 1, 4, 12, "Fredrik"};
    SENT_TABLE_RESERVATION sentTableReservation2 = { 2, 7, 13, "Bengt"};
    SENT_TABLE_RESERVATION sentTableReservation3 = { 3, 8, 14, "Amund"};
    SENT_TABLE_RESERVATION sentTableReservation4 = { 4, 6, 14, "Fredrik"};


    add(&list, &sentTableReservation);
    SENT_ORDER sentOrder = {"Gunnar", "Biff", 230};
    SENT_ORDER sentOrder2 = {"Gunnar", "Cola", 50};
    SENT_ORDER sentOrder3 = {"Fredrik", "TorskSteik", 210};
    SENT_ORDER sentOrder4 = {"Fredrik", "Fanta", 55};

    add(&list, &sentTableReservation2);
    SENT_ORDER sentOrder5 = {"Bengt", "Kobe beef", 550};
    SENT_ORDER sentOrder6 = {"Bengt", "Cola", 30};
    SENT_ORDER sentOrder7 = {"Johnny", "Nokka", 210};
    SENT_ORDER sentOrder8 = {"Johnny", "Drikke", 55};

    add(&list, &sentTableReservation3);
    SENT_ORDER sentOrder9 = {"Amund", "Frosk", 120};
    SENT_ORDER sentOrder10 = {"Amund", "Såpevann", 30};
    SENT_ORDER sentOrder11 = {"Johnny", "Nokka", 210};
    SENT_ORDER sentOrder12 = {"Johnny", "Drikke", 55};

    add(&list, &sentTableReservation4);
    SENT_ORDER sentOrder13 = {"Fredrik", "Sjokolade", 160};
    SENT_ORDER sentOrder14 = {"Fredrik", "Apperol spritz", 120};
    SENT_ORDER sentOrder15 = {"Johnny", "Nokka godt", 210};
    SENT_ORDER sentOrder16 = {"Johnny", "Brun saus", 350};


    addFoodToSpecificReservation(&list, 1, &sentOrder);
    addFoodToSpecificReservation(&list, 1, &sentOrder2);
    addFoodToSpecificReservation(&list, 1, &sentOrder3);
    addFoodToSpecificReservation(&list, 1, &sentOrder4);

    addFoodToSpecificReservation(&list, 2, &sentOrder5);
    addFoodToSpecificReservation(&list, 2, &sentOrder6);
    addFoodToSpecificReservation(&list, 2, &sentOrder7);
    addFoodToSpecificReservation(&list, 2, &sentOrder8);

    addFoodToSpecificReservation(&list, 3, &sentOrder9);
    addFoodToSpecificReservation(&list, 3, &sentOrder10);
    addFoodToSpecificReservation(&list, 3, &sentOrder11);
    addFoodToSpecificReservation(&list, 3, &sentOrder12);

    addFoodToSpecificReservation(&list, 4, &sentOrder13);
    addFoodToSpecificReservation(&list, 4, &sentOrder14);
    addFoodToSpecificReservation(&list, 4, &sentOrder15);
    addFoodToSpecificReservation(&list, 4, &sentOrder16);

    printAllNodes(&list);

    printSpecificNodeAndFood(&list, 1);
    printSpecificNodeAndFood(&list, 2);
    deleteSpecificReservation(&list, 1);

    printAllNodes(&list);

    printReservationOrdersAndSum(&list, 3);

    freeLinkedList(&list);

    return 0;
}