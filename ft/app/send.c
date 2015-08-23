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
    // uint16_t                i;
    // size_t                  byteRead;                                           // Byte read from file
    ssize_t                 byteSend;                                           // Byte send to receiver
    int                     newReceiverFD;                                      // new FD of receiver
    char                    *buffer;                                            // For transferring characters
    socklen_t               receiverAddressSize;
    char                    ipAddress[INET6_ADDRSTRLEN];                        // Receiver IP address
    struct sockaddr_storage receiverAddress;                                    // Connector's address information
    // unsigned long long      sequenceNumber;                                     // Packet sequence number

    message         = (char *)malloc(BUFFER_SIZE * sizeof(char));
    // sequenceNumber  = 0;
    // byteRead        = 0;
    buffer          = (char *)malloc(BUFFER_SIZE * sizeof(char));

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

        if (!fork())
        {                                                                       // This is the child process
            close(*socketFD);                                                   // Child doesn't need the listener

            fseek(file, SEEK_SET, 0);
            while (!feof(file))
            {
                fread(buffer, sizeof(char), BUFFER_SIZE, file);
                byteSend = send(newReceiverFD, buffer, BUFFER_SIZE, 0);
                if (byteSend == -1)
                {
                    errorCode   = SEND;
                    messageCode = SEND | NEW_LINE;
                    printMessage();
                }

            }
            *buffer = EOF;
            byteSend = send(newReceiverFD, buffer, BUFFER_SIZE, 0);


            sprintf(message, "File successfully sent.");
            print(message);

            close(newReceiverFD);
            exit(0);                                                            // Destroy child
        }

        close(newReceiverFD);                                                   // Parent doesn't need this
    }

    fclose(file);
    return OK;
}