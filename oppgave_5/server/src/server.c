#include <stdint.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>


#define BUFFERSIZE 1024
#define THREADS 4

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

typedef struct _SERVER_THREAD_STRUCT {
    struct sockaddr_in saAddr;
    SNP snp;
    int32_t *iPhoneNumbers;
    pthread_mutex_t *mutex;
} SERVER_THREAD_STRUCT;

void *threadServer(void *arg);

int server(int argc, char *argv[]) {
    int iPort = 8080;
    int iStatus = 0;
    int i;
    int32_t phoneNumbers[THREADS] = {0};

    SERVER_THREAD_STRUCT serverThreadStructs[THREADS];
    pthread_t threads[THREADS];

    pthread_mutex_t phoneNumbersMutex;
    pthread_mutex_init(&phoneNumbersMutex, NULL);

    for (i = 0; i < THREADS; ++i) {
        memset(&serverThreadStructs[i], 0, sizeof(SERVER_THREAD_STRUCT));
        serverThreadStructs[i].saAddr.sin_family = AF_INET;
        serverThreadStructs[i].saAddr.sin_port = htons(iPort + i);
        serverThreadStructs[i].saAddr.sin_addr.s_addr = INADDR_ANY;
        serverThreadStructs[i].iPhoneNumbers = phoneNumbers;
        serverThreadStructs[i].mutex = &phoneNumbersMutex;
    }

    for (i = 0; i < THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, threadServer, (void *) &serverThreadStructs[i]) != 0) {
            iStatus = -1;
            printf("Failed to create thread: %d - Error message: %s\n", i, strerror(errno));
            break;
        }
    }
    if (iStatus == -1) {
        for (int j = 0; j <= i; ++j) {
            pthread_cancel(threads[j]);
        }
    } else {
        for (int j = 0; j < THREADS; ++j) {
            if (pthread_join(threads[j], NULL) != 0) {
                iStatus = -1;
                printf("Failed to join thread: %d - Error message: %s\n", j, strerror(errno));
            }
        }
    }
    pthread_mutex_destroy(&phoneNumbersMutex);
    if (iStatus != 0) {
        return -1;
    }
    return 0;
}

void *threadServer(void *arg) {
    SERVER_THREAD_STRUCT serverThreadStructs = *((SERVER_THREAD_STRUCT *) arg);
    ssize_t iSizeOfBody = 0;
    int addrLen = sizeof(serverThreadStructs.saAddr);
    int sockFd;
    int sockNewFd = 0;
    int irc = 0;
    int iStatus = 0;
    char buffer[BUFFERSIZE];
    int iPhone = 0;
    int firstConnection = 0;
    int iIncomingMagicNumber = 0;

    SNP ssSnp = {0};
    SNP *pssSnp = NULL;
    SNP_CONNECT ssSnpConnect = {0};

    struct sockaddr_in saConClient = {0};

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        iStatus = -1;
        printf("Failed to create endpoint and retrieve file descriptor - Error message %s\n", strerror(errno));
    } else {
        printf("Successfully created endpoint with socket\n");

        if (bind(sockFd, (struct sockaddr *) &serverThreadStructs.saAddr, sizeof(serverThreadStructs.saAddr)) < 0) {
            iStatus = -1;
            printf("Failed to bind address and port to socket - Error message: %s\n", strerror(errno));
        } else {
            printf("Socket successfully bound to IP address and port\n");

            irc = listen(sockFd, 5);

            if (irc < 0) {
                iStatus = -1;
                printf("Failed to find incoming connection through listen - Error message %s", strerror(errno));
            } else {

                sockNewFd = accept(sockFd, (struct sockaddr *) &saConClient, (socklen_t *) &addrLen);
                if (sockNewFd < 0) {
                    iStatus = -1;
                    printf("Failed to extract first connection request and create new socket - Error message: %s\n",
                           strerror(errno));
                } else {

                    iIncomingMagicNumber = recv(sockNewFd, &ssSnpConnect.iMagicNumber,
                                                sizeof(ssSnpConnect.iMagicNumber), 0);

                    if (ssSnpConnect.iMagicNumber != MAGIC_NUMBER_CONNECT) {
                        iStatus = -1;
                        printf("Invalid protocol: %d\n", ssSnpConnect.iMagicNumber);
                    } else {

                        printf("Matching protocol\n");

                        recv(sockNewFd, &ssSnpConnect.iIpAddress, sizeof(ssSnpConnect.iIpAddress), 0);
                        recv(sockNewFd, &ssSnpConnect.iPhoneNumber, sizeof(ssSnpConnect.iPhoneNumber), 0);

                        pthread_mutex_lock(serverThreadStructs.mutex);

                        iPhone = ssSnpConnect.iPhoneNumber;

                        for (int i = 0; i < THREADS; ++i) {
                            if (serverThreadStructs.iPhoneNumbers[i] == iPhone) {
                                iStatus = -1;
                                printf("Phone number already in use\n");
                                break;
                            }
                        }

                        if (iStatus != 0) {
                            pthread_mutex_unlock(serverThreadStructs.mutex);
                        } else {

                            for (int i = 0; i < THREADS; ++i) {
                                if (serverThreadStructs.iPhoneNumbers[i] == 0) {
                                    serverThreadStructs.iPhoneNumbers[i] = iPhone;
                                    break;
                                }
                            }

                            pthread_mutex_unlock(serverThreadStructs.mutex);

                            printf("Successfully accepted connection\n");
                            printf("========================================\n");


                            while (1) {
                                // Recv will return 0 or -1 on an orderly shutdown or -1 if an error occurs
                                iIncomingMagicNumber = recv(sockNewFd, &ssSnp.ssSnpHeader.iMagicNumber,
                                                            sizeof(ssSnp.ssSnpHeader.iMagicNumber), 0);
                                if (ssSnp.ssSnpHeader.iMagicNumber != MAGIC_NUMBER_SNP || iIncomingMagicNumber <= 0) {
                                    iStatus = -1;
                                    printf("Invalid magic number\n");
                                    break;
                                }
                                iSizeOfBody = recv(sockNewFd, &ssSnp.ssSnpHeader.iSizeOfBody,
                                                   sizeof(ssSnp.ssSnpHeader.iSizeOfBody), 0);

                                if (iSizeOfBody <= 0) {
                                    printf("Client: %d disconnected, or read error occurred.\n", iPhone);
                                    break;
                                }

                                pssSnp = (SNP *) malloc(sizeof(SNP) + ssSnp.ssSnpHeader.iSizeOfBody + 1);
                                if (pssSnp == NULL) {
                                    iStatus = -1;
                                    printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
                                } else {
                                    memset(pssSnp, 0, sizeof(SNP) + ssSnp.ssSnpHeader.iSizeOfBody + 1);

                                    memcpy(pssSnp, &ssSnp, sizeof(ssSnp.ssSnpHeader));

                                    recv(sockNewFd, pssSnp->body, pssSnp->ssSnpHeader.iSizeOfBody, 0);
                                    pssSnp->body[ssSnp.ssSnpHeader.iSizeOfBody] = '\0';

                                    if (strncmp(pssSnp->body, "exit", 4) == 0) {
                                        printf("Exiting\n");
                                        free(pssSnp);
                                        break;
                                    }

                                    printf("Nr %d: %s\n", iPhone, pssSnp->body);
                                    free(pssSnp);
                                }
                                if (iStatus == -1) {
                                    break;
                                }
                            }
                        }
                    }

                    printf("Closing socket\n");

                    close(sockNewFd);
                    shutdown(sockNewFd, SHUT_RDWR);
                    sockNewFd = -1;
                }
            }
        }
        close(sockFd);
        sockFd = -1;
    }
    return 0;

}
