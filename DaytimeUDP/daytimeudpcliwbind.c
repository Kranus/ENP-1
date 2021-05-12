#include "daytime_common.h"

#define DEFAULT_DEST_PORT 0xDEAF
#define DEFAULT_DEST_ADDR "127.0.0.1"

#define CLI_PORT 0xBEEF
#define MAXLINE  4096

int main(int argc, char *argv[]) {

    SA_IN      dest_addr;
    uint16_t   dest_port; // variable u.U. nicht benötigt
    SA_IN      cli_addr;
    int        sockfd;
    char       recvline[MAXLINE];
    char       c;
    int        n;

    memset(&dest_addr, 0, sizeof(dest_addr));

    dest_port = DEFAULT_DEST_PORT;
    inet_aton(DEFAULT_DEST_ADDR, &dest_addr.sin_addr);

    c = 'l';

    //--------------------------------------------------------------------------

    if (argc >= 2) {

        if (!strcmp(argv[1], "h") || !strcmp(argv[1], "--help")) {

            fprintf(stderr,
                    "usage: %s [IPv4] [PORT] [l]\n",
                    argv[0]);

            exit(EXIT_FAILURE);
        }

        if (inet_aton(argv[1], &dest_addr.sin_addr) == 0) {

            inet_aton(DEFAULT_DEST_ADDR, &dest_addr.sin_addr);

            fprintf(stderr,
                    "Invalid IP argument. Using default IP %s.\n",
                    inet_ntoa(dest_addr.sin_addr));
        }

        if (argc > 2) {

            char* endptr;
            dest_port = strtol(argv[2], &endptr, 10);

            if (*endptr != '\0') {

                dest_port = DEFAULT_DEST_PORT;

                fprintf(stderr,
                        "Invalid port argument. Using default port %d.\n",
                        DEFAULT_DEST_PORT);
            }

            if (argc > 3) {
                c = *argv[3];
            }
        }
    }

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port   = htons(dest_port);

    printf("Port:\t%d\n", dest_port);
    printf("IP:\t%s\n",   inet_ntoa(dest_addr.sin_addr));

    // Client addr init
    memset(&cli_addr, 0, sizeof(cli_addr));
    cli_addr.sin_family       = AF_INET;
    cli_addr.sin_port         = htons(CLI_PORT);
    cli_addr.sin_addr.s_addr  = htonl(INADDR_ANY);

    //--------------------------------------------------------------------------

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    ERR_N_DIE("Socket creation error");

    bind(sockfd, (SA*) &cli_addr, sizeof(cli_addr));

    /* connect(sockfd, (SA*) &dest_addr, sizeof(dest_addr)); */
    /* ERR_N_DIE("Connect error"); */

    sendto(sockfd, &c, 1, 0, (SA*) &dest_addr, sizeof(dest_addr));
    ERR_N_DIE("Sendto error");

    n = recvfrom(sockfd, recvline, MAXLINE - 1, 0, NULL, 0);
    ERR_N_DIE("Recvfrom error");
    recvline[n] = '\0';

    printf("Received: %s\n", recvline);

    close(sockfd);
    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
