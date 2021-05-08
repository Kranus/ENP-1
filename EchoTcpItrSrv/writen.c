// writen.c
// write n bytes from ptr to socket fd

#include "echotcp_common.h"

ssize_t
writen (int fd, const void* ptr, size_t n) {

    size_t  nleft;
    ssize_t nwritten;

    nleft = n;

    //int i = 0;
    while (nleft > 0) {

        nwritten = write(fd, ptr, nleft);
        if ( nwritten <= 0){
            // Fehlerunterscheidung
            if (errno == EINTR) { // interrupt by signal
                nwritten = 0;
            } else {
                perror("writen error");
                exit(EXIT_FAILURE);
            }

        }

        printf("written: ");
        for (int i = 0; i < nwritten; i++) {
            printf("%c", *((char*)(ptr + i)));
        }
        printf("\n");

        nleft -= nwritten;
        ptr   += nwritten;

        //i++;
    }


    //printf("writen while iterations: %d\n", i);

    return n;
}
