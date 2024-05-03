#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>
#include "../include/server.h"

int server() {
    int iPort = 8080;
    int iStatus = 0;
    int i;

    int32_t aiPhoneNumbers[THREADS] = {0};

    SERVER_THREAD_STRUCT assServerThreadStructs[THREADS];
    pthread_t aspThreads[THREADS];

    pthread_mutex_t phoneNumbersMutex;
    pthread_mutex_init(&phoneNumbersMutex, NULL);

    // Initialize the server thread structs
    for (i = 0; i < THREADS; ++i) {
        memset(&assServerThreadStructs[i], 0, sizeof(SERVER_THREAD_STRUCT));
        assServerThreadStructs[i].iStatus = 0;
        assServerThreadStructs[i].saAddr.sin_family = AF_INET;
        assServerThreadStructs[i].saAddr.sin_port = htons(iPort + i);
        assServerThreadStructs[i].saAddr.sin_addr.s_addr = INADDR_ANY;
        assServerThreadStructs[i].iPhoneNumbers = aiPhoneNumbers;
        assServerThreadStructs[i].mutex = &phoneNumbersMutex;
    }

    // Create the aspThreads
    for (i = 0; i < THREADS; ++i) {
        if (pthread_create(&aspThreads[i], NULL, threadServer, (void *) &assServerThreadStructs[i]) != 0) {
            iStatus = -1;
            printf("Failed to create thread: %d - Error message: %s\n", i, strerror(errno));
            break;
        }
    }
    // Cancel the aspThreads if an error occurred
    if (iStatus == -1) {
        for (int j = 0; j <= i; ++j) {
            pthread_cancel(aspThreads[j]);
        }
    } else {
        // Join the aspThreads after they have finished
        for (int j = 0; j < THREADS; ++j) {
            if (pthread_join(aspThreads[j], NULL) != 0) {
                iStatus = -1;
                printf("Failed to join thread: %d - Error message: %s\n", j, strerror(errno));
            }
        }
    }
    // Check if any error messages in the aspThreads
    for (int j = 0; j < THREADS; ++j) {
        if (assServerThreadStructs[j].iStatus != 0) {
            iStatus = -1;
        }
    }
    // Destroy the mutex and return
    pthread_mutex_destroy(&phoneNumbersMutex);
    if (iStatus != 0) {
        return -1;
    }
    return 0;
}

void *threadServer(void *arg) {
    SERVER_THREAD_STRUCT serverThreadStructs = *((SERVER_THREAD_STRUCT *) arg);

    ssize_t iSizeOfBody = 0;
    int sockFd;
    int sockNewFd = 0;
    int irc = 0;
    int iStatus = serverThreadStructs.iStatus;
    int iPhone = 0;
    int iIncomingMagicNumber = 0;
    int iBytes = 0;

    int iAddrLen = sizeof(serverThreadStructs.saAddr);

    SNP ssSnp = {0};
    SNP *pssSnp = NULL;
    SNP_CONNECT ssSnpConnect = {0};

    struct sockaddr_in saConClient = {0};

    // Create a socket
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        iStatus = -1;
        printf("Failed to create endpoint and retrieve file descriptor - Error message %s\n", strerror(errno));
    } else {
        printf("Successfully created endpoint with socket\n");

        // Bind the socket to the IP address and port
        if (bind(sockFd, (struct sockaddr *) &serverThreadStructs.saAddr, sizeof(serverThreadStructs.saAddr)) < 0) {
            iStatus = -1;
            printf("Failed to bind address and port to socket - Error message: %s\n", strerror(errno));
        } else {
            printf("Socket successfully bound to IP address and port\n");

            // Listen for incoming connections
            irc = listen(sockFd, 5);

            if (irc < 0) {
                iStatus = -1;
                printf("Failed to find incoming connection through listen - Error message %s", strerror(errno));
            } else {

                // Accept the first connection request and create a new socket
                sockNewFd = accept(sockFd, (struct sockaddr *) &saConClient, (socklen_t *) &iAddrLen);
                if (sockNewFd < 0) {
                    iStatus = -1;
                    printf("Failed to extract first connection request and create new socket - Error message: %s\n",
                           strerror(errno));
                } else {

                    // Receive the magic number to make sure the protocol is right
                    iIncomingMagicNumber = recv(sockNewFd, &ssSnpConnect.iMagicNumber,
                                                sizeof(ssSnpConnect.iMagicNumber), 0);

                    // Error handling, if the client disconnects or the protocol is wrong
                    if (iIncomingMagicNumber <= 0) {
                        iStatus = -1;
                        printf("Client disconnected\n");
                    } else if (ssSnpConnect.iMagicNumber != MAGIC_NUMBER_CONNECT) {
                        iStatus = -1;
                        printf("Invalid protocol: %d\n", ssSnpConnect.iMagicNumber);
                    } else {

                        printf("Matching protocol\n");

                        // Receive the IP address and phone number
                        iBytes = recv(sockNewFd, &ssSnpConnect.iIpAddress, sizeof(ssSnpConnect.iIpAddress), 0);
                        if (iBytes <= 0) {
                            iStatus = -1;
                            printf("Client disconnected\n");
                        }

                        iBytes = recv(sockNewFd, &ssSnpConnect.iPhoneNumber, sizeof(ssSnpConnect.iPhoneNumber), 0);
                        if (iBytes <= 0) {
                            iStatus = -1;
                            printf("Client disconnected\n");
                        }

                        // Check if the phone number is already in use, lock the mutex to ensure no other thread is using the phone number list
                        pthread_mutex_lock(serverThreadStructs.mutex);

                        iPhone = ssSnpConnect.iPhoneNumber;

                        // Loop through all the phone numbers to check if the phone number is already in use
                        for (int i = 0; i < THREADS; ++i) {
                            if (serverThreadStructs.iPhoneNumbers[i] == iPhone) {
                                iStatus = -1;
                                printf("Phone number already in use\n");
                                break;
                            }
                        }

                        // If the phone number is in use, unlock the mutex and exit
                        if (iStatus != 0) {
                            pthread_mutex_unlock(serverThreadStructs.mutex);
                        } else {

                            // Add phone number if it is not in use
                            for (int i = 0; i < THREADS; ++i) {
                                if (serverThreadStructs.iPhoneNumbers[i] == 0) {
                                    serverThreadStructs.iPhoneNumbers[i] = iPhone;
                                    break;
                                }
                            }

                            pthread_mutex_unlock(serverThreadStructs.mutex);

                            printf("\n========================================\n");
                            printf("%d joined the party\n", iPhone);
                            printf("========================================\n\n");


                            while (1) {
                                // Recv will return 0 on an orderly shutdown or -1 if an error occurs
                                // First recieve the magic number to make sure the protocol is right
                                iIncomingMagicNumber = recv(sockNewFd, &ssSnp.ssSnpHeader.iMagicNumber,
                                                            sizeof(ssSnp.ssSnpHeader.iMagicNumber), 0);
                                if (iIncomingMagicNumber <= 0) {
                                    printf("Client: %d disconnected, or read error occurred.\n", iPhone);
                                    break;
                                }
                                if (ssSnp.ssSnpHeader.iMagicNumber != MAGIC_NUMBER_SNP) {
                                    iStatus = -1;
                                    printf("Invalid magic number\n");
                                    break;
                                }

                                // Receive the size of the body to be bale to read the next part
                                iSizeOfBody = recv(sockNewFd, &ssSnp.ssSnpHeader.iSizeOfBody,
                                                   sizeof(ssSnp.ssSnpHeader.iSizeOfBody), 0);

                                if (iSizeOfBody <= 0) {
                                    printf("Client: %d disconnected, or read error occurred.\n", iPhone);
                                    break;
                                }

                                // Allocate memory based on the size we got from the user
                                pssSnp = (SNP *) malloc(sizeof(SNP) + ssSnp.ssSnpHeader.iSizeOfBody + 1);
                                if (pssSnp == NULL) {
                                    iStatus = -1;
                                    printf("Failed to allocate memory - Error message: %s\n", strerror(errno));
                                } else {
                                    memset(pssSnp, 0, sizeof(SNP) + ssSnp.ssSnpHeader.iSizeOfBody + 1);

                                    memcpy(pssSnp, &ssSnp, sizeof(ssSnp.ssSnpHeader));

                                    // Receive the body of the message
                                    iBytes = recv(sockNewFd, pssSnp->strBody, pssSnp->ssSnpHeader.iSizeOfBody, 0);
                                    if (iBytes <= 0) {
                                        printf("Client: %d disconnected, or read error occurred.\n", iPhone);
                                        free(pssSnp);
                                        break;
                                    }

                                    pssSnp->strBody[ssSnp.ssSnpHeader.iSizeOfBody] = '\0';

                                    // Check if the user wants to exit the program
                                    if (strncmp(pssSnp->strBody, "exit", 4) == 0) {
                                        printf("Exiting\n");
                                        free(pssSnp);
                                        break;
                                    }

                                    // Print the message from the user
                                    printf("Nr %d: %s\n", iPhone, pssSnp->strBody);
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

    // Set the status of the thread for the server function to check
    serverThreadStructs.iStatus = iStatus;

    return 0;

}
