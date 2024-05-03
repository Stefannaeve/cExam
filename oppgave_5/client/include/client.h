#include <stdlib.h>

#ifndef __CLIENT_H__
#define __CLIENT_H__

#define BUFFER_SIZE 1024
#define MAGIC_NUMBER_CONNECT 0xCAFE
#define MAGIC_NUMBER_SNP 0xBABE

int client(int argc, char *argv[]);

int checkArguments(int argc, char *argv[]);

void userInput(char *strBuffer, int iSize);

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

#endif //__CLIENT_H__
