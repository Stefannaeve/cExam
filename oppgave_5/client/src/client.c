#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFERSIZE 1024

#define THREADS 1

typedef struct _SNP_CONNECT {
    int32_t iMagicNumber;
    int32_t iIpAddress;
    int32_t iPhoneNumber;
} SNP_CONNECT;

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

typedef struct _THREAD_STRUCT {
    struct sockaddr_in saAddr;
    SNP snp;
    struct _SNP_CONNECT snpConnect;
} THREAD_STRUCT;

void *threadClient(void *arg);

int userInput(char *buffer, int size);

int client(int argc, char *argv[]) {
    int iPort = atoi(argv[1]);
    int status = 0;
    int i;
    int sockFd;
    char buffer[BUFFERSIZE];
    int sizeOfBuffer;
    srand((unsigned int) time(NULL));
    int randomNumber = rand() % 100000000;

    if (argc != 7) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[0123]\n", argv[0]);
        return -1;
    }
    if (strcmp(argv[1], "-server") != 0) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[0123]\n", argv[0]);
        printf("Second argument is not \"-server\"\n");
        return -1;
    }
    if (strcmp(argv[2], "127.0.0.1") != 0) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[0123]\n", argv[0]);
        printf("IP address is not the loopback address\n");
    }

    if (strcmp(argv[3], "-port") != 0) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[0123]\n", argv[0]);
        printf("Third argument is not \"-port\"\n");
        return -1;
    }
    if (atoi(argv[4]) < 0 || atoi(argv[5]) > 65535) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[0123]\n", argv[0]);
        printf("Port number is not in the range of 0-65535\n");
        return -1;
    }

    if (strcmp(argv[5], "-phone") != 0) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[0123]\n", argv[0]);
        printf("Fifth argument is not \"-phone\"\n");
        return -1;
    }
    if (atoi(argv[6]) < 1000 || atoi(argv[6]) > 9999) {
        printf("Usage: %s -server <ip> -port <port> -phone <phoneNumber>[0123]\n", argv[0]);
        printf("Phone number is not in the range of 1000-9999\n");
        return -1;
    }

    // oppgave_5 -server 127.0.0.1 -port 42 -telefon 1234

    SNP snp = {0};

    printf("\n");
    printf("%02X\n", atoi(argv[2]));
    printf("%d\n", atoi("7F000001"));
    printf("%02X\n", inet_addr(argv[2]));
    printf("\n");
    printf("%02X\n", htonl(0x7F000001));
    printf("%02X\n", inet_addr(argv[2]));
    printf("%d\n", atoi(argv[4]));

    struct sockaddr_in saAddr = {0};
    saAddr.sin_family = AF_INET;
    saAddr.sin_port = htons(atoi(argv[4]));
    saAddr.sin_addr.s_addr = inet_addr(argv[2]); //Home

    snp.ssSnpHeader.iPhoneNumber = atoi(argv[6]);
    snp.ssSnpHeader.iIpAddress = saAddr.sin_addr.s_addr;
    snp.ssSnpHeader.iMagicNumber = randomNumber;

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        printf("Failed to create endpoint and retrieve file descriptor - Error message %s\n", strerror(errno));

    } else {
        printf("Successfully created endpoint with socket\n");

        if (connect(sockFd, (struct sockaddr *) &saAddr, sizeof(saAddr)) < 0) {
            printf("Connection failed: Error message: %s\n", strerror(errno));

        } else {

            printf("Connect successfully handled\n");

            memset(buffer, 0, BUFFERSIZE);

            printf("Write your message\n");

            userInput(buffer, BUFFERSIZE);

            sizeOfBuffer = strlen(buffer) + 1;

            SNP *pssSnp = (SNP *) malloc(sizeof(SNP) + sizeOfBuffer * sizeof(char));

            memset(pssSnp, 0, sizeof(SNP) + sizeOfBuffer * sizeof(char));

            pssSnp->ssSnpHeader.iMagicNumber = snp.ssSnpHeader.iMagicNumber;
            pssSnp->ssSnpHeader.iIpAddress = snp.ssSnpHeader.iIpAddress;
            pssSnp->ssSnpHeader.iPhoneNumber = snp.ssSnpHeader.iPhoneNumber;

            pssSnp->ssSnpHeader.iSizeOfBody = sizeOfBuffer - 1;

            strncpy(pssSnp->body, buffer, sizeOfBuffer);

            pssSnp->body[sizeOfBuffer - 1] = '\0';

            printf("Magic number: %d\n", pssSnp->ssSnpHeader.iMagicNumber);
            printf("Ip address: %d\n", pssSnp->ssSnpHeader.iIpAddress);
            printf("Phone number: %d\n", pssSnp->ssSnpHeader.iPhoneNumber);

            send(sockFd, pssSnp, sizeof(SNP) + pssSnp->ssSnpHeader.iSizeOfBody, 0);

            printf("Closing socket\n");

            free(pssSnp);

            close(sockFd);
            sockFd = -1;
        }
    }

    return 0;
}

void *threadClient(void *arg) {

    return NULL;
}

int userInput(char *buffer, int size) {
    int bufferLength;
    fgets(buffer, size - 1, stdin);

    bufferLength = strlen(buffer);

    while (buffer[bufferLength - 1] == '\r' || buffer[bufferLength - 1] == '\n') {
        buffer[bufferLength - 1] = '\0';
        bufferLength = strlen(buffer);
    }

    buffer[size - 1] = '\0';
}
