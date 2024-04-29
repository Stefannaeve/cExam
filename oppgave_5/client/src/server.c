#include <stdint.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


#define BUFFERSIZE 1024

#define THREADS 2

typedef struct _SNP_CONNECT{
    int32_t iMagicNumber;
    int32_t iIpAddress;
    int32_t iPhoneNumber;
} SNP_CONNECT;

typedef struct _SNP_HEADER {
    int32_t iMagicNumber;
    int32_t sizeOfBody;
} SNP_HEADER;

// "My name" Network Protocol
typedef struct _SNP {
    SNP_HEADER snpHeader;
    char body[];
} SNP;

typedef struct _THREAD_STRUCT{
    struct sockaddr_in saAddr;
    struct sockaddr_in saConClient;
    SNP_HEADER clientHeader;
    struct _SNP_CONNECT snpConnect;
} THREAD_STRUCT;

int server(int argc, char *argv[]) {
    int iPort = 8080;

    THREAD_STRUCT threadStructArray[THREADS];

    for (int i = 0; i < THREADS - 1; ++i) {
        memset(&threadStructArray[i].saAddr, 0, sizeof(threadStructArray[i].saAddr));
        memset(&threadStructArray[i].saConClient, 0, sizeof(threadStructArray[i].saConClient));
        memset(&threadStructArray[i].clientHeader, 0, sizeof(threadStructArray[i].clientHeader));
        memset(&threadStructArray[i].snpConnect, 0, sizeof(threadStructArray[i].snpConnect));

        threadStructArray[i].saAddr.sin_family = AF_INET;
        threadStructArray[i].saAddr.sin_port = htons(iPort+i);
        threadStructArray[i].saAddr.sin_addr.s_addr = INADDR_ANY;
    }

    printf("Server started on port %d\n", iPort);



    int sockFd;
    int sockNewFd = 0;
    int addrLen = sizeof(saAddr);
    char buffer[BUFFERSIZE];

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        printf("Failed to create endpoint and retrieve file descriptor - Error message %s\n", strerror(errno));
    } else {
        printf("Successfully created endpoint with socket\n");

        if (bind(sockFd, (struct sockaddr *) &saAddr, sizeof(saAddr)) < 0) {
            printf("Failed to bind address and port to socket - Error message: %s\n", strerror(errno));
        } else {
            printf("Socket successfully bound to IP address and port\n");
        }
    }
}
