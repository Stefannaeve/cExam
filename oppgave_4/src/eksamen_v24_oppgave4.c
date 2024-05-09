#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include "../include/eksamen_v24_oppgave4.h"
#include "../include/sha1.h"

void *thread_A(void *sendThreadArg) {
    SEND_THREAD *pssSendThread = (SEND_THREAD *) sendThreadArg;
    int read_bytes = 0;

    FILE *fp = fopen(pssSendThread->pszFilename, "r");
    if (!fp) {
        perror("Failed to open file");
        return 0;
    }

    while (1) {

        // Lock the mutex
        // Wait for the strBuffer to empty
        while (pssSendThread->iBytes_in_buffer > 0) {
            sem_wait(&pssSendThread->semBufferCleared);
        }
        pthread_mutex_lock(&pssSendThread->mutex);

        // Read the file into the strBuffer
        read_bytes = fread(pssSendThread->strBuffer + pssSendThread->iBytes_in_buffer, 1,
                               BUFFER_SIZE - pssSendThread->iBytes_in_buffer, fp);
        pssSendThread->iBytes_in_buffer += read_bytes;
        pssSendThread->strBuffer[pssSendThread->iBytes_in_buffer] = '\0';


        // Unlock the mutex
        // Signal the other thread that the strBuffer is full
        pthread_mutex_unlock(&pssSendThread->mutex);
        sem_post(&pssSendThread->semBufferFull);

        // Signal the other thread that the file has ended
        if (read_bytes == 0 && feof(fp)) {
            pssSendThread->iIsDone = 1;
            sem_post(&pssSendThread->semBufferFull);
            break;
        }
    }

    // Close the file
    // Exit the thread
    fclose(fp);
    printf("Thread A is done\n");
    fp = NULL;
    return 0;
    // Pthread_exit made leaks in program (more in documentation)
    // pthread_exit(NULL);
}

void *thread_B(void *sendThreadArg) {
    SEND_THREAD *pssSendThread = (SEND_THREAD *) sendThreadArg;
    SHA1_CTX ssCtx;

    int iCount = 0;
    int iSizeOfFinalSha1 = 20;
    unsigned char strDigest[iSizeOfFinalSha1];

    memset(pssSendThread->aiCount, 0, BYTE_RANGE * sizeof(int));
    memset(&ssCtx, 0, sizeof(SHA1_CTX));
    memset(strDigest, 0, sizeof(strDigest));

    SHA1Init(&ssCtx);

    while (1) {
        // If strBuffer is read, wait for it to be full again
        if (pssSendThread->iBytes_in_buffer == 0) {
            // Check for signal from thread A that the file has ended
            sem_wait(&pssSendThread->semBufferFull);

        }
        // Locks the mutex
        pthread_mutex_lock(&pssSendThread->mutex);

        // Count the bytes in the strBuffer
        for (int i = 0; i < pssSendThread->iBytes_in_buffer; i++) {
            pssSendThread->aiCount[pssSendThread->strBuffer[i]]++;
        }

        // Update sha1 context with new data
        SHA1Update(&ssCtx, pssSendThread->strBuffer, pssSendThread->iBytes_in_buffer);


        // Signal the other thread that the strBuffer is empty
        pssSendThread->iBytes_in_buffer = 0;

        // Unlock the mutex
        // Signal the other thread that the strBuffer has been counted
        pthread_mutex_unlock(&pssSendThread->mutex);
        sem_post(&pssSendThread->semBufferCleared);

        // If the file has ended, break the loop
        if (pssSendThread->iIsDone) {
            break;
        }
        iCount++;
    }

    // Finalize the sha1 context and print it
    printf("\n");
    SHA1Final(strDigest, &ssCtx);

    printf("Hashing done\n");
    for (int j = 0; j < sizeof(strDigest); ++j) {
        printf("%X", strDigest[j]);
    }
    printf("\n");


    // Print the iCount of each byte, excluding numbers which has not been counted
    /*
    for (int i = 0; i < BYTE_RANGE; i++) {
        if (pssSendThread->aiCount[i] > 0) {
            printf("%d: %d\n", i, pssSendThread->aiCount[i]);
        }
    }
     */
    printf("Thread B is done\n");
    return 0;
// Pthread_exit made leaks in program (More in documentation)
// pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    SEND_THREAD *pssSendThread = NULL;
    pthread_t threadA, threadB;

    if (argc > 2) {
        printf("%s only has one argument\n", argv[0]);
        return -1;
    } else if (argc == 1) {
        printf("%s has an argument for the file to use\n", argv[0]);
        return -1;
    }

    pssSendThread = (SEND_THREAD *) malloc(sizeof(SEND_THREAD));
    if (pssSendThread == NULL) {
        printf("Failed to allocate memory for send_thread\n");
    } else {
        memset(pssSendThread, 0, sizeof(SEND_THREAD));
        if (pthread_mutex_init(&pssSendThread->mutex, NULL) != 0) {
            perror("Could not initialize mutex");
        } else {

            if (sem_init(&pssSendThread->semBufferFull, 0, 0) != 0) {
                perror("Could not initialize semBufferFull");
            } else {
                if (sem_init(&pssSendThread->semBufferCleared, 0, 0) != 0) {
                    perror("Could not initialize semBufferCleared");
                } else {

                    pssSendThread->iBytes_in_buffer = 0;
                    pssSendThread->iIsDone = 0;
                    pssSendThread->pszFilename = argv[1];


                    if (pthread_create(&threadA, NULL, thread_A, (void *) pssSendThread) != 0) {
                        perror("Could not create thread A");
                    } else {

                        printf("Created thread A\n");
                        if (pthread_create(&threadB, NULL, thread_B, (void *) pssSendThread) != 0) {
                            perror("Could not create thread B");
                        } else {
                            printf("Created thread B\n");
                            if (pthread_join(threadB, NULL) != 0) {
                                perror("Could not join thread B");
                            }
                        }
                        if (pthread_join(threadA, NULL) != 0) {
                            perror("Could not join thread A");
                        }
                    } // Created thread A
                    sem_destroy(&pssSendThread->semBufferCleared);
                } // Made semaphore semBufferCleared
                sem_destroy(&pssSendThread->semBufferFull);
            } // Made semaphore semBufferFull
            pthread_mutex_destroy(&pssSendThread->mutex);
        } // Made mutex
        free(pssSendThread);
        pssSendThread = NULL;
    } // Made pssSendThread
    printf("Done\n");
    return 0;
}