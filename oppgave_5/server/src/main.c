#include "../include/server.h"

int main(int argc, char *argv[]){
    int statusCode = server(argc, argv);
    if (statusCode != 0){
        return 1;
    }
    return 0;
}