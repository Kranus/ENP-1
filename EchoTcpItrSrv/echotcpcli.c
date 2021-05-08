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

    memset(&dest_addr, 0, sizeof(dest_addr));

    dest_port = DEFAULT_DEST_PORT;
    inet_aton(DEFAULT_DEST_ADDR, &dest_addr.sin_addr);

    //--------------------------------------------------------------------------

    if (argc >= 2) {

        if (strcmp(argv[1], "h") || strcmp(argv[1], "--help")) {
            printf("usage: %s [IPv4] [PORT]\n", argv[0]);
            exit(EXIT_SUCCESS);
        }

        if(inet_aton(argv[1], &dest_addr.sin_addr) == 0) {

            inet_aton(DEFAULT_DEST_ADDR, &dest_addr.sin_addr);

            fprintf(stderr,
                    "Invalid IP argument. Using default IP %s\n.",
                    inet_ntoa(dest_addr.sin_addr));
        }
    }

    if (argc > 2) {

        char* endptr;
        dest_port = strtol(argv[2], &endptr, 10);

        if (*endptr != '\0') {

            dest_port = DEFAULT_DEST_PORT;

            fprintf(stderr,
                    "Invalid port argument. Using default port %d\n.",
                    DEFAULT_DEST_PORT);
        }
    }

    //--------------------------------------------------------------------------

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERR_N_DIE("Socket creation error");

    dest_addr.sin_family      = AF_INET;
    dest_addr.sin_port        = htons(dest_port);
    //dest_addr.sin_addr.s_addr = dest_ip;

    printf("port:\t%d\n", dest_port);
    printf("ip:\t%s\n", inet_ntoa(dest_addr.sin_addr));

    connect(sockfd, (SA*) &dest_addr, sizeof(dest_addr));
    ERR_N_DIE("Connect error");

    //--------------------------------------------------------------------------

    str_cli(sockfd);

    printf("Done\n");

    return EXIT_SUCCESS;
}
