#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFERSIZE 1024

#define THREADS 1

typedef struct _SNP_CONNECT {
    int32_t iMagicNumber;
    int32_t iIpAddress;
    int32_t iPhoneNumber;
} SNP_CONNECT;

typedef struct _SNP_HEADER {
    int32_t iMagicNumber;
    int32_t iIpAddress;
    int32_t iPhoneNumber;
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
int userInput(char *buffer, int size);

int client(int argc, char *argv[]) {
    int iPort = 8080;
    int status = 0;
    int i;

    THREAD_STRUCT threadStructs[THREADS];
    pthread_t threads[THREADS];

    for (i = 0; i < THREADS; ++i) {
        memset(&threadStructs[i], 0, sizeof(THREAD_STRUCT));
        threadStructs[i].snp.ssSnpHeader.iMagicNumber = 0x01DF5E76;
        threadStructs[i].saAddr.sin_family = AF_INET;
        threadStructs[i].saAddr.sin_port = htons(iPort + i);
        threadStructs[i].saAddr.sin_addr.s_addr = htonl(0x7F000001);
        threadStructs[i].snp.ssSnpHeader.iPhoneNumber = 12345678;
        threadStructs[i].snp.ssSnpHeader.iIpAddress = threadStructs[i].saAddr.sin_addr.s_addr;
    }

    for (i = 0; i < THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, threadClient, (void *) &threadStructs[i]) != 0) {
            status = -1;
            printf("Failed to create thread: %d - Error message: %s\n", i, strerror(errno));
            break;
        }
    }
    if (status == -1) {
        for (int j = 0; j <= i; ++j) {
            pthread_cancel(threads[j]);
        }
    } else {
        for (int j = 0; j < THREADS; ++j) {
            if (pthread_join(threads[j], NULL) != 0) {
                printf("Failed to join thread: %d - Error message: %s\n", j, strerror(errno));
            }
        }
    }
    return 0;
}

void *threadClient(void *arg) {
    THREAD_STRUCT *threadStruct = (THREAD_STRUCT *) arg;
    int sockFd;
    char buffer[BUFFERSIZE];
    int sizeOfBuffer;
    srand((unsigned int)time(NULL));
    int randomNumber = rand() % 100000000;
    threadStruct->snpConnect.iMagicNumber = randomNumber;

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        printf("Failed to create endpoint and retrieve file descriptor - Error message %s\n", strerror(errno));

    } else {
        printf("Successfully created endpoint with socket\n");



        if (connect(sockFd, (struct sockaddr *) &threadStruct->saAddr, sizeof(threadStruct->saAddr)) < 0) {
            printf("Connection failed: Error message: %s\n", strerror(errno));

        } else {

            printf("Connect successfully handled\n");

            memset(buffer, 0, BUFFERSIZE);

            printf("Write your message\n");

            userInput(buffer, BUFFERSIZE);

            sizeOfBuffer = strlen(buffer);

            SNP *snp = (SNP *) malloc(sizeof(SNP) + sizeOfBuffer * sizeof(char));

            memset(snp, 0, sizeof(SNP) + sizeOfBuffer * sizeof(char));

            snp->ssSnpHeader.iSizeOfBody = sizeOfBuffer;

            strncpy(snp->body, buffer, sizeOfBuffer);

            snp->body[sizeOfBuffer - 1] = '\0';

            send(sockFd, snp, sizeof(SNP) + snp->ssSnpHeader.iSizeOfBody, 0);

            printf("Closing socket\n");

            close(sockFd);
            sockFd = -1;
            return 0;
        }
    }
    return NULL;
}

int userInput(char *buffer, int size) {
    int bufferLength;
    fgets(buffer, size - 1, stdin);

    bufferLength = strlen(buffer);

    while (buffer[bufferLength - 1] == '\r' || buffer[bufferLength - 1] == '\n') {
        buffer[bufferLength - 1] = '\0';
        bufferLength = strlen(buffer);
    }

    buffer[size - 1] = '\0';
}
