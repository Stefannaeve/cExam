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
        while (sendThread->bytes_in_buffer > 0) {
            pthread_cond_wait(&sendThread->cond_empty, &sendThread->mutex);
        }

        // Read the file into the buffer
        int read_bytes = fread(sendThread->buffer + sendThread->bytes_in_buffer, 1,
                               BUFFER_SIZE - sendThread->bytes_in_buffer, fp);
        sendThread->bytes_in_buffer += read_bytes;

        for (int i = 0; i < sendThread->bytes_in_buffer; ++i) {
            printf("%c", sendThread->buffer[i]);
        }

        pthread_cond_signal(&sendThread->cond_full);
        pthread_mutex_unlock(&sendThread->mutex);

        // Signal the other thread that the file has ended
        if (read_bytes == 0 && feof(fp)) {
            sendThread->bytes_in_buffer = -1;
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

    memset(sendThread->count, 0, (BYTE_RANGE - 1) * sizeof(int));

    while (1) {
        // Lock the mutex
        // Wait for the buffer to fill
        pthread_mutex_lock(&sendThread->mutex);
        if (sendThread->bytes_in_buffer == 0) {
            pthread_cond_wait(&sendThread->cond_full, &sendThread->mutex);
            // Check for signal from thread A that the file has ended
            if (sendThread->bytes_in_buffer == -1) {
                break;
            }
        }

        // Count the bytes in the buffer
        for (int i = 0; i < sendThread->bytes_in_buffer; i++) {
            sendThread->count[sendThread->buffer[i]]++;
        }

        // Signal the other thread that the buffer is empty
        sendThread->bytes_in_buffer = 0;
        pthread_cond_signal(&sendThread->cond_empty);
        pthread_mutex_unlock(&sendThread->mutex);


    }
    for (int i = 0; i < BYTE_RANGE; i++) {
        printf("%d: %d\n", i, sendThread->count[i]);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
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
    sendThread->bytes_in_buffer = 0;


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

    pthread_mutex_destroy(&sendThread->mutex);
    pthread_cond_destroy(&sendThread->cond_full);
    pthread_cond_destroy(&sendThread->cond_empty);
    free(sendThread);

    return 0;
}










