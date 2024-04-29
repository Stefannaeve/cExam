#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include "../include/eksamen_v24_oppgave4.h"
#include "../include/sha1.h"

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define BYTE_RANGE 256

typedef struct _SEND_THREAD {
    int count[BYTE_RANGE];
    unsigned char buffer[BUFFER_SIZE];
    char *filename;
    pthread_mutex_t mutex;
    sem_t bufferFull, bufferCleared;
    int bytes_in_buffer;
    int isDone;
} SEND_THREAD;

void *thread_A(void *sendThreadArg) {
    SEND_THREAD *sendThread = (SEND_THREAD *) sendThreadArg;

    FILE *fp = fopen(sendThread->filename, "r");
    if (!fp) {
        perror("Failed to open file");
        pthread_exit(NULL);
    }

    while (1) {

        // Lock the mutex
        // Wait for the buffer to empty
        while (sendThread->bytes_in_buffer > 0) {
            sem_wait(&sendThread->bufferCleared);
        }
        pthread_mutex_lock(&sendThread->mutex);

        // Read the file into the buffer
        int read_bytes = fread(sendThread->buffer + sendThread->bytes_in_buffer, 1,
                               BUFFER_SIZE - sendThread->bytes_in_buffer, fp);
        sendThread->bytes_in_buffer += read_bytes;

        /*
        for (int i = 0; i < sendThread->bytes_in_buffer; ++i) {
            printf("%c", sendThread->buffer[i]);
        }
         */

        // Unlock the mutex
        // Signal the other thread that the buffer is full
        pthread_mutex_unlock(&sendThread->mutex);
        sem_post(&sendThread->bufferFull);

        // Signal the other thread that the file has ended
        if (read_bytes == 0 && feof(fp)) {
            sendThread->isDone = 1;
            sem_post(&sendThread->bufferFull);
            break;
        }
    }

    // Close the file
    // Exit the thread
    fclose(fp);
    printf("File closed\n");
    fp = NULL;
    return 0;
    // Pthread_exit made leaks in program
    // pthread_exit(NULL);
}

void *thread_B(void *sendThreadArg) {
    SEND_THREAD *sendThread = (SEND_THREAD *) sendThreadArg;

    int count = 0;
    int i;
    int j;
    unsigned char digest[20];
    unsigned char array[64];

    memset(sendThread->count, 0, BYTE_RANGE * sizeof(int));
    SHA1_CTX ctx;
    SHA1Init(&ctx);

    while (1) {
        // If buffer is read, wait for it to be full again
        if (sendThread->bytes_in_buffer == 0) {
            // Check for signal from thread A that the file has ended
            sem_wait(&sendThread->bufferFull);

        }
        // Locks the mutex
        pthread_mutex_lock(&sendThread->mutex);

        // Count the bytes in the buffer
        for (i = 0; i < sendThread->bytes_in_buffer; i++) {
            sendThread->count[sendThread->buffer[i]]++;
        }

        j = 0;

        SHA1Update(&ctx, sendThread->buffer, sendThread->bytes_in_buffer);


        // Signal the other thread that the buffer is empty
        sendThread->bytes_in_buffer = 0;

        // Unlock the mutex
        // Signal the other thread that the buffer has been counted
        pthread_mutex_unlock(&sendThread->mutex);
        sem_post(&sendThread->bufferCleared);

        // If the file has ended, break the loop
        if (sendThread->isDone) {
            break;
        }
        count++;
    }

    printf("\n\n");
    SHA1Final(digest, &ctx);
    for (j = 0; j < sizeof(digest); ++j) {
        printf("%02X", digest[j]);
    }
    printf("\n\n");


    for (i = 0; i < BYTE_RANGE; i++) {
        if (sendThread->count[i] > 0) {
            printf("%d: %d\n", i, sendThread->count[i]);
        }
    }
    printf("Thread B is done\n");
    return 0;
// Pthread_exit made leaks in program
// pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    if (argc > 2) {
        printf("%s only has one argument\n", argv[0]);
        return -1;
    } else if (argc == 1) {
        printf("%s has an argument for the file to use\n", argv[0]);
        return -1;
    }

    SEND_THREAD *sendThread = (SEND_THREAD *) malloc(sizeof(SEND_THREAD));
    if (sendThread == NULL) {
        printf("Failed to allocate memory for send_thread\n");
    } else {

        if (pthread_mutex_init(&sendThread->mutex, NULL) != 0) {
            perror("Could not initialize mutex");
        } else {

            if (sem_init(&sendThread->bufferFull, 0, 0) != 0) {
                perror("Could not initialize bufferFull");
            } else {
                if (sem_init(&sendThread->bufferCleared, 0, 0) != 0) {
                    perror("Could not initialize bufferCleared");
                } else {

                    sendThread->bytes_in_buffer = 0;
                    sendThread->isDone = 0;
                    sendThread->filename = argv[1];

                    pthread_t threadA, threadB;

                    if (pthread_create(&threadA, NULL, thread_A, (void *) sendThread) != 0) {
                        perror("Could not create thread A");
                    } else {

                        printf("Created thread A\n");
                        if (pthread_create(&threadB, NULL, thread_B, (void *) sendThread) != 0) {
                            perror("Could not create thread B");
                        } else {
                            printf("Created thread B\n");
                            printf("Thread IDs: A: %lu, B: %lu\n", threadA, threadB);
                            if (pthread_join(threadB, NULL) != 0) {
                                perror("Could not join thread B");
                            }
                            printf("Joined thread B\n");
                        }

                        if (pthread_join(threadA, NULL) != 0) {
                            perror("Could not join thread A");
                        }
                        printf("Joined thread A\n");
                        printf("Thread IDs: A: %lu, B: %lu\n", threadA, threadB);

                    } // Created thread A

                    sem_destroy(&sendThread->bufferCleared);
                    printf("Destroyed bufferCleared\n");

                } // Made semaphore bufferCleared
                sem_destroy(&sendThread->bufferFull);
                printf("Destroyed bufferFull\n");
            } // Made semaphore bufferFull
            pthread_mutex_destroy(&sendThread->mutex);
            printf("Destroyed mutex\n");
        } // Made mutex
        free(sendThread);
        sendThread = NULL;
        printf("Freed sendThread\n");
    } // Made sendThread
    printf("Done\n");
    return 0;
}