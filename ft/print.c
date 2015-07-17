#include <errno.h>                                                              // Error number
#include <stdio.h>                                                              // For fprintf()
#include <string.h>                                                             // For strerror()
#include <stdlib.h>                                                             // For malloc()
#include <sys/types.h>                                                          // For gai_strerror()
#include <sys/socket.h>
#include <netdb.h>

#include "include.h"                                                            // For global error and message code
#include "message.h"                                                            // Message codes

#define     MAX_MESSAGE     100                                                 // Max number of message

void printMessage()
{
    static char **message;                                                      // Message storage array

    message = (char **)malloc(MAX_MESSAGE * sizeof(char *));                    // Allocate memory for message

 /* List of all the possible message to be displayed are listed here
  * Message code is enlisted in message.h
  */
    message[ARGC]               = "Usage: fly [IP] [Filename]";
    message[ADDRESS]            = "getaddrinfo";
    message[SOCKET]             = "socket";
    message[SET_SOCK_OPT]       = "setsockopt";
    message[BIND]               = "bind";
    message[BIND_FAIL]          = "sender binding failed";
    message[LISTEN]             = "backlog";
    message[SIGACTION]          = "sigaction";
    message[SEND]               = "send";
    message[ACCEPT]             = "accept";
    message[RECEIVE]            = "receive";
    message[CONNECT]            = "connect";
    message[CONNECTION_FAILED]  = "receiver connection failed";
    message[FILE_OPEN]          = "fopen";
    message[IS_RECEIVER_READY]  = "Is receiver ready?";
    message[RECEIVER_IS_READY]  = "Receiver is ready";
    message[DATA_SEND]          = "Data send";
    message[DATA_RECEIVED]      = "Data received";

    if (errorCode == OK)
    {
        fprintf(stdout, "%s",                                                   // Prints message
                (char *)message[(messageCode & REMOVE_NEW_LINE)]);
    }
    else
    {
        if ((messageCode & REMOVE_NEW_LINE) == ADDRESS)
        {
            fprintf(stdout, "%s: %s",
                    (char *)message[(messageCode & REMOVE_NEW_LINE)],           // Message
                    gai_strerror(errorCode));                                   // Error message
        }
        else
        {
            fprintf(stdout, "%s: %s",
                    (char *)message[(messageCode & REMOVE_NEW_LINE)],           // Message
                    (char *)strerror(errorCode));                               // Error description
        }
    }

    if ((messageCode & NEW_LINE) == NEW_LINE)                                   // New line is printed if required
    {
        fprintf(stdout, "%s", "\n");
    }

    errorCode   = OK;
    messageCode = OK;
}

void print(char *message)
{
    fprintf(stdout, "%s", (char *)message);
}
