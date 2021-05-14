#include "daytime_common.h"

#define DEFAULT_DEST_PORT 0xDEAF
#define DEFAULT_DEST_ADDR "127.0.0.1"

int main(int argc, char *argv[]) {

    SA_IN          dest_addr;
    int            sockfd;
    char           recvline[MAXLINE];
    struct timeval timeout;
    char           c;
    int            n;

    timeout.tv_sec  = UDP_RECV_TIMEOUT_SEC;
    timeout.tv_usec = 0;

    c = 'l';
    if (argc >= 2) {
        c = *argv[1];
    }

    //--------------------------------------------------------------------------

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port   = htons(DEFAULT_DEST_PORT);
    inet_aton(DEFAULT_DEST_ADDR, &dest_addr.sin_addr);

    printf("Dest Port:\t%d\n", ntohs(dest_addr.sin_port));
    printf("Dest IP:\t%s\n",   inet_ntoa(dest_addr.sin_addr));

    //--------------------------------------------------------------------------

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    ERR_N_DIE("Socket creation error");

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    // c)
    connect(sockfd, (SA*) &dest_addr, sizeof(dest_addr));
    ERR_N_DIE("Connect error");

    // c)
    send(sockfd, &c, 1, 0);
    ERR_N_DIE("Send error");

    // c)
    n = recv(sockfd, recvline, MAXLINE - 1, 0);

    if (errno) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            ERR_N_DIE("Timeout reached");
        }
        ERR_N_DIE("Recv error");
    }

    recvline[n] = '\0';

    printf("Received: %s\n", recvline);

    close(sockfd);
    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
// Ausgabe Client
// > ./cliwconnect
/*
Dest Port: 57007
Dest IP:   127.0.0.1
Received:  Fri May 14 18:19:24 2021
*/

// Ausgabe Server
// > ./srv
/*
clientlen:  16
clientip:   127.0.0.1
clientport: 53247 --> vgl a) --> hier zufälliger Port, da kein bind
*/

// Für den Fall, dass der Server nicht gestartet ist, geschieht kein timeout
// wie bei a) und b) (Z. 55), sondern es kommt sofort der Fehler
// "Recv error: Connection refused"
