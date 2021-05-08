#include "daytime_common.h"

int main(int argc, char *argv[]) {

    SA_IN      dest_addr;
    uint16_t   dest_port;
    int        sockfd;
    char       recvline[81];
    char       c;
    int        n;

    dest_port = 13;
    //addr_str  = INADDR_LOOPBACK;
    c = 'l';
    if (argc > 1) {
        c = *argv[1];
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERR_N_DIE("Socket creation error");

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family        = AF_INET;
    dest_addr.sin_port          = htons(SRVPORT);//htons(SRVPORT);
    dest_addr.sin_addr.s_addr   = htonl(SRVIPADDR); // !


    connect(sockfd, (SA*) &dest_addr, sizeof(dest_addr));
    ERR_N_DIE("Connect error");

    if (1) { // if not local host loopback
        write(sockfd, &c, 1);
        ERR_N_DIE("Write error");
    }

    n = read(sockfd, recvline, 80);
    ERR_N_DIE("Read error");
    recvline[n] = '\0';

    printf("received: %s\n", recvline);

    return EXIT_SUCCESS;
}
