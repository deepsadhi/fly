#include <stddef.h>                                                             // For NULL
#include <sys/types.h>                                                          // For addrinfo structure
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>                                                             // For malloc()
#include <stdint.h>
#include "message.h"                                                            // Message codes

#include <stdio.h>                                                              // remove it

void printMessage();
int16_t getAddress(char *ip, struct addrinfo *addressInfo);
int16_t prepareSocket(struct addrinfo *addressInfo, int *socketFD);
int16_t prepareListen(int *socketFD);
int16_t sendFile(int *socketFD);
int16_t receiveFile(int *socketFD);

int         errorCode         = OK;                                             // Global variable to hold error code
int8_t      messageCode       = OK;                                             // Global variable to hold message code

int main(int argc, char *argv[])
{
    int                         socketFD;                                       // Socket file descriptor
    struct addrinfo             *addressInfo;                                   // Address information

    addressInfo = (struct addrinfo *)malloc(sizeof(struct addrinfo));           // Allocate memory

    if (argc < 2 || argc > 3)
    {
        messageCode = ARGC | NEW_LINE;
        printMessage();
        return ARGC;
    }

    switch (argc)
    {                                                                           // Receiving mode 
        case 2:
        {
            if (getAddress(argv[1], addressInfo) != OK)
            {
                printMessage();
                return ADDRESS;
            }

            messageCode = RECEIVER;
            if (prepareSocket(addressInfo, &socketFD)  != OK)
            {
                printMessage();
                return BIND_FAIL;
            }

            if (receiveFile(&socketFD))
            {
                printMessage();
                return RECEIVE;
            }

            break;
        }

        case 3:
        {                                                                       // Sending mode
            if (getAddress(argv[1], addressInfo) != OK)
            {
                printMessage();
                return ADDRESS;
            }

            if (prepareSocket(addressInfo, &socketFD) != OK)
            {
                printMessage();
                return BIND_FAIL;
            }

            if (prepareListen(&socketFD) != OK)
            {
                printMessage();
                return LISTEN;
            }

            if (sendFile(&socketFD) != OK)
            {
                printMessage();
                return SEND;
            }

            break;
        }

        default:
        {
            messageCode = ARGC;
            printMessage();
            break;
        }
    }

    return 0;
}

