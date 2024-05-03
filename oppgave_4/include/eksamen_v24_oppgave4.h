#ifndef __OPPGAVE_4_EKSAMEN_V24_OPPGAVE4_H__
#define __OPPGAVE_4_EKSAMEN_V24_OPPGAVE4_H__

#define BUFFER_SIZE 4096
#define BYTE_RANGE 256

typedef struct _SEND_THREAD {
    int aiCount[BYTE_RANGE];
    unsigned char strBuffer[BUFFER_SIZE];
    char *pszFilename;
    pthread_mutex_t mutex;
    sem_t semBufferFull, semBufferCleared;
    int iBytes_in_buffer;
    int iIsDone;
} SEND_THREAD;

int main(int argc, char *argv[]);

#endif //__OPPGAVE_4_EKSAMEN_V24_OPPGAVE4_H__
