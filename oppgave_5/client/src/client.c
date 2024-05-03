#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUFFERSIZE 1024
#define MAGIC_NUMBER_CONNECT 0xCAFE
#define MAGIC_NUMBER_SNP 0xBABE

typedef struct _SNP_CONNECT {
    int32_t iMagicNumber;
    int32_t iIpAddress;
    int32_t iPhoneNumber;
} SNP_CONNECT;

typedef struct _SNP_HEADER {
    int32_t iMagicNumber;
    int32_t iSizeOfBody;
} SNP_HEADER;

// "My name" Network Protocol
typedef struct _SNP {
    SNP_HEADER ssSnpHeader;
    char body[];
} SNP;

typedef struct _THREAD_STRUCT {
    struct sockaddr_in saAddr;
    SNP snp;
    struct _SNP_CONNECT snpConnect;
} THREAD_STRUCT;

void *threadClient(void *arg);

int checkArguments(int argc, char *argv[]);

void userInput(char *strBuffer, int iSize);

int client(int argc, char *argv[]) {
    char buffer[BUFFERSIZE];

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

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        iStatus = -1;
        printf("Failed to create endpoint and retrieve file descriptor - Error message %s\n", strerror(errno));
    } else {
        printf("Successfully created endpoint with socket\n");

        if (connect(sockFd, (struct sockaddr *) &saAddr, sizeof(saAddr)) < 0) {
            iStatus = -1;
            printf("Connection failed: Error message: %s\n", strerror(errno));
        } else {

            iBytesSent = send(sockFd, &ssSnpConnect, sizeof(ssSnpConnect), 0);
            if (iBytesSent <= 0) {
                iStatus = -1;
                printf("Failed to send message - Error message: %s\n", strerror(errno));
            } else {

                printf("Connect successfully handled\n");

                memset(buffer, 0, BUFFERSIZE);

                printf("Write your message, write \"exit\" to exit program\n");

                while (1) {
                    printf("Enter message: ");
                    userInput(buffer, BUFFERSIZE);

                    iSizeOfBuffer = strlen(buffer) + 1;

                    if(strncmp(buffer, "exit", iSizeOfExit) == 0){
                        break;
                    }

                    pssSnp = (SNP *) malloc(sizeof(SNP) + iSizeOfBuffer * sizeof(char));
                    memset(pssSnp, 0, sizeof(SNP) + iSizeOfBuffer * sizeof(char));
                    pssSnp->ssSnpHeader.iMagicNumber = ssSnp.ssSnpHeader.iMagicNumber;
                    pssSnp->ssSnpHeader.iSizeOfBody = iSizeOfBuffer - 1;
                    strncpy(pssSnp->body, buffer, iSizeOfBuffer);
                    pssSnp->body[iSizeOfBuffer - 1] = '\0';

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

    while (strBuffer[iBufferLength - 1] == '\r' || strBuffer[iBufferLength - 1] == '\n') {
        strBuffer[iBufferLength - 1] = '\0';
        iBufferLength = strlen(strBuffer);
    }

    strBuffer[iSize - 1] = '\0';
}
