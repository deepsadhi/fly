#include <sys/types.h>                                                          // For recv()
#include <sys/socket.h>
#include <stdlib.h>                                                             // For malloc()
#include <errno.h>
#include <stdio.h>                                                              // For sprintf();
#include <unistd.h>                                                             // For close()
#include "message.h"                                                            // Message codes
#include "include.h"                                                            // For global error and message code
#include "file.h"                                                               // Buffer size

#include <stdint.h>

#define MAXDATASIZE 2

void print(char *message);

int16_t receiveFile(int *socketFD)
{
    // char buf[MAXDATASIZE];
    // char                    *message;                                           // Stores message
    // ssize_t                 byteReceive;
    char                    *buffer;

    buffer = (char *)malloc(2 * sizeof(char));
    if (recv(*socketFD, buffer, 1, 0) == -1)
    {
        errorCode   = errno;
        messageCode = IS_RECEIVER_READY;
        return IS_RECEIVER_READY;
    }
    printf("message %d\n", (uint8_t)buffer[0]);
    printf("sequence %d\n", (uint8_t)buffer[1]);



    // message = (char *)malloc(sizeof(char));

    // FILE *fp;
    // fp = fopen("tmp", "w+");


    // if ((byteReceive = recv(*socketFD, buf, MAXDATASIZE-1, 0)) == -1)
    // {
    //     errorCode   = errno;
    //     messageCode = RECEIVE | NEW_LINE;
    //     return RECEIVE;
    // }
    //     // printf("%c", buf[0] );
    //    fputc(buf[0], fp);
    // while (buf[0] != EOF)
    // {
    //     byteReceive = recv(*socketFD, buf, MAXDATASIZE-1, 0);
    //    fputc(buf[0], fp);
    //     // printf("%c", buf[0] );
    //     // errorCode   = errno;
    //     // messageCode = RECEIVE | NEW_LINE;
    //     // return RECEIVE;
    // }

    // fclose(fp);
    // printf("file received\n");

    // if ((byteReceive = recv(*socketFD, buf, MAXDATASIZE-1, 0)) == -1)
    // {
    //     errorCode   = errno;
    //     messageCode = RECEIVE | NEW_LINE;
    //     return RECEIVE;
    // }

    // buf[byteReceive] = '\0';

    // sprintf(message, "received %s\n", buf);
    // print(message);

    close(*socketFD);
    return OK;
}