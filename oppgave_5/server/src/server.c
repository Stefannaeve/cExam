#define BUFFERSIZE 1024

#define THREADS 2

typedef struct _SNP_HEADER {
    int32_t contentType;
    int32_t sizeOfBody;
} SNP_HEADER;

typedef struct _SNP {
    SNP_HEADER snpHeader;
    char body[];
} SNP;

typedef struct THREAD_STRUCT{
    struct sockaddr_in saAddr;
    struct sockaddr_in saConClient;
    SNP_HEADER clientHeader;
};

int server(int argc, char *argv[]) {


    for (int i = 0; i < THREADS; ++i) {

    }

    struct sockaddr_in saAddr = {0};
    struct sockaddr_in saConClient = {0};
    SNP_HEADER clientHeader = {0};

    int sockFd;
    int sockNewFd = 0;
    int iPort = 8080;
    int addrLen = sizeof(saAddr);
    char buffer[BUFFERSIZE];

    saAddr.sin_family = AF_INET;
    saAddr.sin_port = htons(iPort);
    saAddr.sin_addr.s_addr = INADDR_ANY;

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        printf("Failed to create endpoint and retrieve file descriptor - Error message %s\n", strerror(errno));
    } else {
        printf("Successfully created endpoint with socket\n");

        if (bind(sockFd, (struct sockaddr *) &saAddr, sizeof(saAddr)) < 0) {
            printf("Failed to bind address and port to socket - Error message: %s\n", strerror(errno));
        } else {
            printf("Socket successfully bound to IP address and port\n");
        }
    }
}
