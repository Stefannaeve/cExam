#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define BYTE_RANGE 256

typedef struct _SEND_THREAD {
    int count[BYTE_RANGE];
    unsigned char buffer[BUFFER_SIZE];
    pthread_mutex_t mutex;
    pthread_cond_t cond_full, cond_empty;
    int bytes_in_buffer;
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
        pthread_mutex_lock(&sendThread->mutex);
        while (sendThread->bytes_in_buffer == BUFFER_SIZE) {
            pthread_cond_wait(&sendThread->cond_empty, &sendThread->mutex);
        }

        // Read the file into the buffer
        int read_bytes = fread(sendThread->buffer + sendThread->bytes_in_buffer, 1, BUFFER_SIZE - sendThread->bytes_in_buffer, fp);
        sendThread->bytes_in_buffer += read_bytes;

        // Signal the other thread that the buffer is full
        if (read_bytes < BUFFER_SIZE - sendThread->bytes_in_buffer) {
            pthread_mutex_unlock(&sendThread->mutex);
            break;
        }

        // Signal the other thread that the buffer is full
        // Unlock the mutex
        pthread_cond_signal(&sendThread->cond_full);
        pthread_mutex_unlock(&sendThread->mutex);
    }

    // Close the file
    // Exit the thread
    fclose(fp);
    pthread_exit(NULL);
}

void *thread_B(void *sendThreadArg) {
    SEND_THREAD *sendThread = (SEND_THREAD *) sendThreadArg;

    memset(sendThread->count, 0, BYTE_RANGE - 1);

    while (1) {
        // Lock the mutex
        // Wait for the buffer to fill
        pthread_mutex_lock(&sendThread->mutex);
        while (sendThread->bytes_in_buffer == 0) {
            pthread_cond_wait(&sendThread->cond_full, &sendThread->mutex);
        }

        // Count the bytes in the buffer
        for (int i = 0; i < sendThread->bytes_in_buffer; i++) {
            sendThread->count[sendThread->buffer[i]]++;
        }

        
        sendThread->bytes_in_buffer = 0;
        pthread_cond_signal(&sendThread->cond_empty);
        pthread_mutex_unlock(&sendThread->mutex);

        if (sendThread->bytes_in_buffer == 0) {
            break;
        }
    }
    for (int i = 0; i < BYTE_RANGE; i++)
        printf("%d: %d\n", i, sendThread->count[i]);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    SEND_THREAD *sendThread = (SEND_THREAD *) malloc(sizeof(SEND_THREAD));
    if (sendThread == NULL) {
        printf("Failed to allocate memory for send_thread\n");
        return -1;
    }

    pthread_mutex_init(&sendThread->mutex, NULL);
    pthread_cond_init(&sendThread->cond_full, NULL);
    pthread_cond_init(&sendThread->cond_empty, NULL);
    sendThread->bytes_in_buffer = 0;


    pthread_t threadA, threadB;

    if (pthread_create(&threadA, NULL, thread_A, (void *) sendThread) != 0) {
        perror("Could not create thread A");
        exit(1);
    }

    if (pthread_create(&threadB, NULL, thread_B, (void *) sendThread) != 0) {
        perror("Could not create thread B");
        exit(1);
    }

    if (pthread_join(threadA, NULL) != 0) {
        perror("Could not join thread A");
        exit(1);
    }
    if (pthread_join(threadB, NULL) != 0) {
        perror("Could not join thread B");
        exit(1);
    }

    free(sendThread);

    return 0;
}










