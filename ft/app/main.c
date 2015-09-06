#include <stddef.h>                                                             // For NULL
#include <sys/types.h>                                                          // For addrinfo structure
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>                                                             // For malloc()
#include <stdint.h>
#include "message.h"                                                            // Message codes

#include <stdio.h>                                                              // remove it
#include <unistd.h>                                                             // close()
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#define PORT    "3490"      // the port users wil be connecting to



void printMessage();
int16_t getAddress(char *ip, struct addrinfo *addressInfo);
int16_t prepareSocket(struct addrinfo *addressInfo, int *socketFD);
int16_t prepareListen(int *socketFD);
int16_t sendFile(int *socketFD, const char *fileName);
int16_t receiveFile(int *socketFD);
int16_t fileOpen(const char *fileName);
int16_t init();                                                                 // Initialize app

int         errorCode         = OK;                                             // Global variable to hold error code
int8_t      messageCode       = OK;                                             // Global variable to hold message code
char        *appPath;                                                           // Get app path

int main(int argc, char *argv[])
{
    // int                         socketFD;                                       // Socket file descriptor
    // struct addrinfo             *addressInfo;                                   // Address information

    // struct addrinfo         *p;
    // int                     yes = 1;
    // int                     sockfd;     // listen on sockd_fd, new connection on new_fd

    // addressInfo = (struct addrinfo *)malloc(sizeof(struct addrinfo));           // Allocate memory

    // if (argc < 2 || argc > 3)
    // {
    //     messageCode = ARGC | NEW_LINE;
    //     printMessage();
    //     return ARGC;
    // }

    // switch (argc)
    // {                                                                           // Receiving mode
    //     case 2:
    //     {
    //         if (getAddress(argv[1], addressInfo) != OK)
    //         {
    //             printMessage();
    //             return ADDRESS;
    //         }

    //         messageCode = RECEIVER;
    //         if (prepareSocket(addressInfo, &socketFD)  != OK)
    //         {
    //             printMessage();
    //             return BIND_FAIL;
    //         }

    //         if (receiveFile(&socketFD))
    //         {
    //             printMessage();
    //             return RECEIVE;
    //         }

    //         break;
    //     }

    //     case 3:
    //     {                                                                       // Sending mode
    //         if (getAddress(argv[1], addressInfo) != OK)
    //         {
    //             printMessage();
    //             return ADDRESS;
    //         }


    int                     sockfd;     // listen on sockd_fd, new connection on new_fd
    struct addrinfo         hints, *servinfo, *p;
    // struct sockaddr_storage their_addr;         // connector's address information
    // socklen_t               sin_size;
    // struct sigaction        sa;
    int                     yes = 1;
    // char                    s[INET6_ADDRSTRLEN];
    int                     rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_flags      = AI_PASSIVE;        // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }


    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }







            // if (prepareSocket(addressInfo, &socketFD) != OK)
            // {
            //     printMessage();
            //     return BIND_FAIL;
            // }

            // if (prepareListen(&socketFD) != OK)
            // {
            //     printMessage();
            //     return LISTEN;
            // }

            // if (sendFile(&socketFD, argv[2]) != OK)
            // {
            //     printMessage();
            //     return SEND;
            // }

    //         break;
    //     }

    //     default:
    //     {
    //         messageCode = ARGC;
    //         printMessage();
    //         break;
    //     }
    // }

    return 0;
}

