#ifndef __SERVER_SERVER_H__
#define __SERVER_SERVER_H__

#define THREADS 2

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
    char strBody[];
} SNP;

typedef struct _SERVER_THREAD_STRUCT {
    int iStatus;
    struct sockaddr_in saAddr;
    SNP ssSnp;
    int32_t *iPhoneNumbers;
    pthread_mutex_t *mutex;
} SERVER_THREAD_STRUCT;

void *threadServer(void *arg);

int server();

#endif //__SERVER_SERVER_H__
