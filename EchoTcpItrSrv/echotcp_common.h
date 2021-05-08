#ifndef ECHOTCP_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define SA    struct sockaddr
#define SA_IN struct sockaddr_in

#define MAXLINE 50

#define ERR_N_DIE(ERR_STR) if(errno) {perror(ERR_STR); exit(EXIT_FAILURE);}

/* int  init_srv_socket (SA_IN* srvaddr); */
/* void handle_connection (int connfd);  // name vllt etwas allgemein? */

ssize_t writen(int fd, const void* ptr, size_t n);
size_t  readline(int fd, void* ptr, size_t n);

#endif
