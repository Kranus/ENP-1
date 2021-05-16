#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

#define ERR_N_DIE(msg) if(errno){perror(msg);exit(EXIT_FAILURE);}

void sighandler () {

}


int main(int argc, char *argv[]) {

    signal(SIGINT, );
    ERR_N_DIE("signal error");

    printf("This program will run until you abort with ^C\n");

    while (1);

    return EXIT_SUCCESS;
}
