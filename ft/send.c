#include <sys/types.h>                                                          // For send()
#include <sys/socket.h>
#include <unistd.h>                                                             // For close()
#include <stdlib.h>                                                             // For malloc()
#include <netinet/in.h>                                                         // For struct sockaddr_in
#include <arpa/inet.h>                                                          // For inet_ntop()
#include <stdio.h>                                                              // For sprintf();
#include <errno.h>
#include "message.h"                                                            // Message codes
#include "include.h"                                                            // For global error and message code
#include "file.h"                                                               // Buffer size

void printMessage();
void print();

void *getINAddress(struct sockaddr *sa)                                         // Get sockaddr IPv4 or IPv6
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int16_t sendFile(int *socketFD, const char *fileName)
{
    FILE                    *file;
    char                    *message;                                           // Stores message
    // ssize_t                 byteSend;
    int                     newReceiverFD;                                      // new FD of receiver
    char                    *buffer;                                            // For transferring characters
    socklen_t               receiverAddressSize;
    char                    ipAddress[INET6_ADDRSTRLEN];                        // Receiver IP address
    struct sockaddr_storage receiverAddress;                                    // Connector's address information
    unsigned long long      sequenceNumber;                                     // Packet sequence number

    message         = (char *)malloc(sizeof(char));
    sequenceNumber  = 0;

    while (1)
    {                                                                           // Wait for new connection
        receiverAddressSize = sizeof receiverAddress;
        newReceiverFD       = accept(*socketFD,
                                     (struct sockaddr *)&receiverAddress,
                                     &receiverAddressSize);                     // Accept new connection

        file = fopen(fileName, "r");                                            // Open file

        if (newReceiverFD == -1)
        {                                                                       // Error on new receiver FD
            errorCode   = ACCEPT;
            messageCode = ACCEPT | NEW_LINE;
            printMessage();
            continue;
        }

        inet_ntop(receiverAddress.ss_family,
                  getINAddress((struct sockaddr *)&receiverAddress), ipAddress,
                               sizeof ipAddress);                               // Receiver IP address

        sprintf(message, "Got connection from %s\n", ipAddress);
        print(message);

     /* Start handshake process */

     /* Ask for receiver is ready to receive file */
        buffer = (char *)malloc(2 * sizeof(char));
        buffer[0] = IS_RECEIVER_READY;
        buffer[1] = sequenceNumber++;
        if (send(newReceiverFD, buffer, BUFFER_SIZE, 0) == -1)
        {
            errorCode   = errno;
            messageCode = IS_RECEIVER_READY | NEW_LINE;
            printMessage();
        }
    }

    //     if (!fork())
    //     {                                                                       // This is the child process
    //         close(*socketFD);                                                   // Child doesn't need the listener

    //         char str[2];
    //         buffer[BUFFER_SIZE-1] = '\0';
    //         /* read one character at a time from file, stopping at EOF, which
    //            indicates the end of the file.  Note that the idiom of "assign
    //            to a variable, check the value" used below works because
    //            the assignment statement evaluates to the value assigned. */
    //         while  ( ( str[0] = fgetc( file ) ) != EOF )
    //         {
    //             byteSend = send(newReceiverFD, buffer, BUFFER_SIZE, 0);
    //             if (byteSend == -1)
    //             {
    //                 errorCode   = SEND;
    //                 messageCode = SEND | NEW_LINE;
    //                 printMessage();
    //             }
    //         }

    //         buffer[0] = EOF;
    //         byteSend = send(newReceiverFD, buffer, BUFFER_SIZE, 0);
    //         fclose( file );



    //         sprintf(message, "File successfully sent.");
    //         print(message);

    //         close(newReceiverFD);
    //         exit(0);                                                            // Destroy child
    //     }

    //     close(newReceiverFD);                                                   // Parent doesn't need this
    // }

    fclose(file);
    return OK;
}