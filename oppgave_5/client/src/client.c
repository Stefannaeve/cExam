#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include "../include/client.h"

int client(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];

    int sockFd = 0;
    int iSizeOfBuffer = 0;
    int iIpv4 = 0;
    int iPort = 0;
    int iPhone = 0;
    int iStatus = 0;
    int iSizeOfExit = 4;
    ssize_t iBytesSent = 0;

    SNP ssSnp = {0};
    struct sockaddr_in saAddr = {0};
    SNP_CONNECT ssSnpConnect = {0};
    SNP *pssSnp = NULL;

    iStatus = checkArguments(argc, argv);

    if (iStatus != 0) {
        return iStatus;
    }

    // Initiation of variables
    iIpv4 = inet_addr(argv[2]);
    iPort = atoi(argv[4]);
    iPhone = atoi(argv[6]);

    saAddr.sin_family = AF_INET;
    saAddr.sin_port = htons(iPort);
    saAddr.sin_addr.s_addr = iIpv4; //Home

    ssSnpConnect.iMagicNumber = MAGIC_NUMBER_CONNECT;
    ssSnpConnect.iIpAddress = saAddr.sin_addr.s_addr;
    ssSnpConnect.iPhoneNumber = iPhone;

    ssSnp.ssSnpHeader.iMagicNumber = MAGIC_NUMBER_SNP;

    // Create a socket
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        iStatus = -1;
        printf("Failed to create endpoint and retrieve file descriptor - Error message %s\n", strerror(errno));
    } else {
        printf("Successfully created endpoint with socket\n");

        // Connect to the server
        if (connect(sockFd, (struct sockaddr *) &saAddr, sizeof(saAddr)) < 0) {
            iStatus = -1;
            printf("Connection failed: Error message: %s\n", strerror(errno));
        } else {

            // Send the connect message to give basic information before sending messages
            iBytesSent = send(sockFd, &ssSnpConnect, sizeof(ssSnpConnect), 0);
            if (iBytesSent <= 0) {
                iStatus = -1;
                printf("Failed to send message - Error message: %s\n", strerror(errno));
            } else {

                printf("Connect successfully handled\n");

                memset(buffer, 0, BUFFER_SIZE);

                printf("Write your message, write \"exit\" to exit program\n");

                // While loop to send message until the user writes "exit" or the connection is lost
                while (1) {
                    printf("Enter message: ");
                    userInput(buffer, BUFFER_SIZE);

                    iSizeOfBuffer = strlen(buffer) + 1;

                    // Check if the user wants to exit the program
                    if(strncmp(buffer, "exit", iSizeOfExit) == 0){
                        break;
                    }

                    // Allocate memory for the message from userInput
                    pssSnp = (SNP *) malloc(sizeof(SNP) + iSizeOfBuffer * sizeof(char));
                    memset(pssSnp, 0, sizeof(SNP) + iSizeOfBuffer * sizeof(char));
                    pssSnp->ssSnpHeader.iMagicNumber = ssSnp.ssSnpHeader.iMagicNumber;
                    pssSnp->ssSnpHeader.iSizeOfBody = iSizeOfBuffer;
                    strncpy(pssSnp->body, buffer, iSizeOfBuffer);
                    pssSnp->body[iSizeOfBuffer - 1] = '\0';

                    // Send the message to the server, use signal to ignore SIGPIPE for my own error handling
                    signal(SIGPIPE, SIG_IGN);
                    iBytesSent = send(sockFd, pssSnp, sizeof(SNP) + pssSnp->ssSnpHeader.iSizeOfBody, 0);

                    free(pssSnp);

                    if (iBytesSent <= 0) {
                        iStatus = -1;
                        printf("Failed to send message - Error message: %s\n", strerror(errno));
                        break;
                    }
                }
            }
        }
    }

    // Close the socket and shutdown the connection
    printf("Closing socket\n");

    close(sockFd);
    shutdown(sockFd, SHUT_RDWR);
    sockFd = -1;

    if (iStatus == -1) {
        return -1;
    }

    return 0;
}

int checkArguments(int argc, char *argv[]) {
    int iSizeOfServer = 7;
    int iSizeOfIp = 9;
    int iSizeOfPort = 5;
    int iSizeOfPhone = 6;

    // Check if all arguments are correct
    if (argc != 7) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[1234]\n", argv[0]);
        return -1;
    }
    if(strncmp(argv[1], "-server", iSizeOfServer) != 0){
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[1234]\n", argv[0]);
        printf("First argument is not \"-server\"\n");
        return -1;
    }
    if(strncmp(argv[2], "127.0.0.1", iSizeOfIp) != 0){
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[1234]\n", argv[0]);
        printf("Second argument is not the loop-back address\n");
        return -1;
    }

    if (strncmp(argv[3], "-port", iSizeOfPort) != 0) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[1234]\n", argv[0]);
        printf("Third argument is not \"-port\"\n");
        return -1;
    }

    if (atoi(argv[4]) < 0 || atoi(argv[5]) > 65535) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[1234]\n", argv[0]);
        printf("Port number is not in the range of 0-65535\n");
        return -1;
    }

    if (strncmp(argv[5], "-phone", iSizeOfPhone) != 0) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[1234]\n", argv[0]);
        printf("Fourth argument is not \"-phone\"\n");
        return -1;
    }

    if (atoi(argv[6]) < 1000 || atoi(argv[6]) > 9999) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[1234]\n", argv[0]);
        printf("Phone number is not in the range of 1000-9999\n");
        return -1;
    }
    return 0;
}

void userInput(char *strBuffer, int iSize) {
    int iBufferLength = 0;
    fgets(strBuffer, iSize - 1, stdin);

    iBufferLength = strlen(strBuffer);

    // Remove newline and carriage return from the end of the string
    while (strBuffer[iBufferLength - 1] == '\r' || strBuffer[iBufferLength - 1] == '\n') {
        strBuffer[iBufferLength - 1] = '\0';
        iBufferLength = strlen(strBuffer);
    }

    strBuffer[iSize - 1] = '\0';
}
