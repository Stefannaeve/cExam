#include <stdint.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>


#define BUFFERSIZE 1024
#define THREADS 2

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

typedef struct _SERVER_THREAD_STRUCT {
    struct sockaddr_in saAddr;
    SNP snp;
} SERVER_THREAD_STRUCT;

void *threadServer(void *arg);

int server(int argc, char *argv[]) {
    int iPort = 8080;
    int status = 0;
    int i;

    SERVER_THREAD_STRUCT serverThreadStructs[THREADS];
    pthread_t threads[THREADS];


    for (i = 0; i < THREADS; ++i) {
        memset(&serverThreadStructs[i], 0, sizeof(SERVER_THREAD_STRUCT));
        serverThreadStructs[i].saAddr.sin_family = AF_INET;
        serverThreadStructs[i].saAddr.sin_port = htons(iPort + i);
        serverThreadStructs[i].saAddr.sin_addr.s_addr = INADDR_ANY;
    }

    for (i = 0; i < THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, threadServer, (void *) &serverThreadStructs[i]) != 0) {
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

void *threadServer(void *arg) {
    SERVER_THREAD_STRUCT serverThreadStructs = *((SERVER_THREAD_STRUCT *) arg);
    int sockFd;
    int sockNewFd = 0;
    char buffer[BUFFERSIZE];
    int addrLen = sizeof(serverThreadStructs.saAddr);

    SNP snp = {0};

    struct sockaddr_in saConClient = {0};

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        printf("Failed to create endpoint and retrieve file descriptor - Error message %s\n", strerror(errno));
    } else {
        printf("Successfully created endpoint with socket\n");

        if (bind(sockFd, (struct sockaddr *) &serverThreadStructs.saAddr, sizeof(serverThreadStructs.saAddr)) < 0) {
            printf("Failed to bind address and port to socket - Error message: %s\n", strerror(errno));
        } else {
            printf("Socket successfully bound to IP address and port\n");

            int irc = listen(sockFd, 5);

            if (irc < 0) {
                printf("Failed to find incoming connection through listen - Error message %s", strerror(errno));
            } else {

                sockNewFd = accept(sockFd, (struct sockaddr *) &saConClient, (socklen_t *) &addrLen);
                if (sockNewFd < 0) {
                    printf("Failed to extract first connection request and create new socket - Error message: %s\n",
                           strerror(errno));
                } else {

                    read(sockNewFd, &snp.ssSnpHeader, sizeof(snp.ssSnpHeader));
                    SNP *pSnp = (SNP *) malloc(sizeof(SNP) + snp.ssSnpHeader.iSizeOfBody + 1);
                    memset(pSnp, 0, sizeof(SNP) + snp.ssSnpHeader.iSizeOfBody + 1);

                    if (pSnp == NULL) {
                        printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
                        return NULL;
                    }

                    memcpy(pSnp, &snp, sizeof(snp.ssSnpHeader));

                    printf("Magic Number: %d\n", pSnp->ssSnpHeader.iMagicNumber);
                    printf("IP Address: %02X\n", ntohl(pSnp->ssSnpHeader.iIpAddress));
                    printf("Phone Number: %d\n", pSnp->ssSnpHeader.iPhoneNumber);
                    printf("Size of Body: %d\n", pSnp->ssSnpHeader.iSizeOfBody);


                    read(sockNewFd, snp.body, snp.ssSnpHeader.iSizeOfBody);
                    snp.body[snp.ssSnpHeader.iSizeOfBody] = '\0';

                    printf("Number %d: %s\n", pSnp->ssSnpHeader.iPhoneNumber, snp.body);

                    printf("Closing socket\n");

                    close(sockFd);
                    close(sockNewFd);
                    sockFd = -1;
                    sockNewFd = -1;

                    free(pSnp);
                }
            }
        }
    }
    return 0;

}
