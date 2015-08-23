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
    ssize_t                 byteReceive;
    char                    *buffer;

    buffer = (char *)malloc(sizeof(char));


    FILE *fp;
    fp = fopen("tmp", "w+");


    if ((byteReceive = recv(*socketFD, buffer, BUFFER_SIZE, 0)) == -1)
    {
        errorCode   = errno;
        messageCode = RECEIVE | NEW_LINE;
        return RECEIVE;
    }
        // printf("%c", buf[0] );
       fwrite(buffer, 1, sizeof(buffer), fp);
    while (*buffer != EOF)
    {
        byteReceive = recv(*socketFD, buffer, BUFFER_SIZE, 0);
       fwrite(buffer, 1, sizeof(buffer), fp);
        // printf("%c", buf[0] );
        // errorCode   = errno;
        // messageCode = RECEIVE | NEW_LINE;
        // return RECEIVE;
    }

    fclose(fp);
    printf("file received\n");

    close(*socketFD);
    return OK;
}