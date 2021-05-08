#include "echotcp_common.h"

#define DEFAULT_DEST_PORT 0xDEAF
#define DEFAULT_DEST_ADDR "127.0.0.1" // INADDR_LOOPBACK

void
str_cli (int sockfd) {

    char sendline[MAXLINE];
    char recvline[MAXLINE];

    // einlesen befehl
    while (fgets(sendline, MAXLINE, stdin) != NULL) {

        // senden befehl
        writen(sockfd, sendline, strlen(sendline)); // +1 wegen '\0'

        printf("ok2\n");

        // empfangen echo
        if (readline(sockfd, recvline, MAXLINE) == 0) { // MAXLINE-->strlen(sendline) erwartet?
            printf("str_cli: server terminated\n");
            exit(EXIT_FAILURE);
        }

        // Ausgabe echo
        printf("Received: %s\n", recvline);
        //fputs(recvline, stdout);
    }
}

int
main(int argc, char *argv[]) {

    SA_IN      dest_addr;
    uint16_t   dest_port;
    int        sockfd;
    char*      dest_addr_str;

    dest_addr_str = DEFAULT_DEST_ADDR;
    dest_port     = DEFAULT_DEST_PORT;

    //--------------------------------------------------------------------------
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERR_N_DIE("Socket creation error");

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family      = AF_INET;
    dest_addr.sin_port        = htons(dest_port);
    dest_addr.sin_addr.s_addr = inet_addr(dest_addr_str);

    connect(sockfd, (SA*) &dest_addr, sizeof(dest_addr));
    ERR_N_DIE("Connect error");

    printf("ok\n");

    //--------------------------------------------------------------------------

    str_cli(sockfd);

    printf("done\n");

    return EXIT_SUCCESS;
}
