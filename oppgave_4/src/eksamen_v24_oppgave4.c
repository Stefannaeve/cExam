#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define BYTE_RANGE 256

typedef struct _SEND_THREAD {
    int count[BYTE_RANGE];
    unsigned char buffer[BUFFER_SIZE];
    pthread_mutex_t mutex;
    pthread_cond_t cond_full, cond_empty;
    sem_t bufferFull, bufferCleared;
    int bytes_in_buffer;
    int isDone;
} SEND_THREAD;

void *thread_A(void *sendThreadArg) {
    SEND_THREAD *sendThread = (SEND_THREAD *) sendThreadArg;

    FILE *fp = fopen("eksamen_v24_oppgave4_pg2265.txt", "r");
    if (!fp) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
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

        for (int i = 0; i < sendThread->bytes_in_buffer; ++i) {
            printf("%c", sendThread->buffer[i]);
        }

        pthread_mutex_unlock(&sendThread->mutex);
        sem_post(&sendThread->bufferFull);

        // Signal the other thread that the file has ended
        if (read_bytes == 0 && feof(fp)) {
            sendThread->isDone = 1;
            sem_post(&sendThread->bufferFull);
            pthread_mutex_unlock(&sendThread->mutex);
            break;
        }
    }

    // Close the file
    // Exit the thread
    fclose(fp);
    pthread_exit(NULL);
}

void *thread_B(void *sendThreadArg) {
    SEND_THREAD *sendThread = (SEND_THREAD *) sendThreadArg;

    int count = 0;

    memset(sendThread->count, 0, (BYTE_RANGE - 1) * sizeof(int));

    while (1) {
        // Lock the mutex
        if (sendThread->bytes_in_buffer == 0) {
            printf("Before %d\n", count);
            sem_wait(&sendThread->bufferFull);
            printf("%d\n", sendThread->isDone);
            printf("After %d\n", count);
            // Check for signal from thread A that the file has ended
        }
        pthread_mutex_lock(&sendThread->mutex);
        // Wait for the buffer to fill

        // Count the bytes in the buffer
        for (int i = 0; i < sendThread->bytes_in_buffer; i++) {
            sendThread->count[sendThread->buffer[i]]++;
        }

        // Signal the other thread that the buffer is empty
        sendThread->bytes_in_buffer = 0;

        pthread_mutex_unlock(&sendThread->mutex);

        sem_post(&sendThread->bufferCleared);

        if (sendThread->isDone) {
            break;
        }
        count++;


    }
    for (int i = 0; i < BYTE_RANGE; i++) {
        printf("%d: %d\n", i, sendThread->count[i]);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
/*
    if (argc > 2) {
        printf("%s only has one argument\n", argv[0]);
        return -1;
    } else if (argc == 1) {
        printf("%s has an argument for the file to use\n", argv[0]);
        return -1;
    }
    */

    SEND_THREAD *sendThread = (SEND_THREAD *) malloc(sizeof(SEND_THREAD));
    if (sendThread == NULL) {
        printf("Failed to allocate memory for send_thread\n");
        return -1;
    }

    if (pthread_mutex_init(&sendThread->mutex, NULL) != 0) {
        perror("Could not initialize mutex");
        return -1;
    }
    if (pthread_cond_init(&sendThread->cond_full, NULL) != 0) {
        perror("Could not initialize cond_full");
        return -1;
    }
    if (pthread_cond_init(&sendThread->cond_empty, NULL) != 0) {
        perror("Could not initialize cond_empty");
        return -1;
    }

    if(sem_init(&sendThread->bufferFull, 0, 0) != 0) {
        perror("Could not initialize bufferFull");
        return -1;
    }
    if(sem_init(&sendThread->bufferCleared, 0, 0) != 0) {
        perror("Could not initialize bufferCleared");
        return -1;
    }

    sendThread->bytes_in_buffer = 0;
    sendThread->isDone = 0;


    pthread_t threadA, threadB;

    if (pthread_create(&threadA, NULL, thread_A, (void *) sendThread) != 0) {
        perror("Could not create thread A");
        return -1;
    }

    if (pthread_create(&threadB, NULL, thread_B, (void *) sendThread) != 0) {
        perror("Could not create thread B");
        return -1;
    }

    if (pthread_join(threadA, NULL) != 0) {
        perror("Could not join thread A");
        return -1;
    }
    if (pthread_join(threadB, NULL) != 0) {
        perror("Could not join thread B");
        return -1;
    }

    sem_destroy(&sendThread->bufferFull);
    sem_destroy(&sendThread->bufferCleared);
    pthread_mutex_destroy(&sendThread->mutex);
    pthread_cond_destroy(&sendThread->cond_full);
    pthread_cond_destroy(&sendThread->cond_empty);
    free(sendThread);

    return 0;
}










