#include <sys/types.h>                                                          // For recv()
#include <sys/socket.h>
#include <stdlib.h>                                                             // For malloc()
#include <errno.h>
#include <stdio.h>                                                              // For sprintf();
#include <unistd.h>                                                             // For close()
#include "message.h"                                                            // Message codes
#include "include.h"                                                            // For global error and message code

#define MAXDATASIZE 100

void print(char *message);

int16_t receiveFile(int *socketFD)
{
    char buf[MAXDATASIZE];
    char                    *message;                                           // Stores message
    ssize_t                 byteReceive;

    message = (char *)malloc(sizeof(char));


    if ((byteReceive = recv(*socketFD, buf, MAXDATASIZE-1, 0)) == -1)
    {
        errorCode   = errno;
        messageCode = RECEIVE | NEW_LINE;
        return RECEIVE;
    }

    buf[byteReceive] = '\0';

    sprintf(message, "received %s\n", buf);
    print(message);

    close(*socketFD);
    return OK;
}