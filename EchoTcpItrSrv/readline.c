// readline.c

#include "echotcp_common.h"

size_t
readline (int fd, void *vptr, size_t maxlen) {

    size_t  n;
    ssize_t rc;
    char    c;
    char*   ptr;

    ptr = vptr;

    for (n = 1; n < maxlen; n++) {

again:
        rc = read(fd, &c, 1);

        if (rc == 1) {
            *ptr = c;
            ptr++;

            if (c == '\n') {
                break;
            }
        }

        else if (rc == 0) {
            if (n == 1)
                return 0; // keine Daten geschrieben (erste iteration)
            else
                break;    // Daten geschrieben, fertig --> break for
        }

        else {
            if (errno == EINTR) {
                goto again;
            }
            else {
                perror("readline error");
                exit(EXIT_FAILURE);
            }
        }

    }

    *ptr = '\0';

    return n;

}
