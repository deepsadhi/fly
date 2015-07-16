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

int16_t sendFile(int *socketFD)
{
    char                    *message;                                           // Stores message
    ssize_t                 byteSend;
    int                     newReceiverFD;                                      // new FD of receiver
    socklen_t               receiverAddressSize;
    char                    ipAddress[INET6_ADDRSTRLEN];                        // Receiver IP address 
    struct sockaddr_storage receiverAddress;                                    // Connector's address information 

    message = (char *)malloc(sizeof(char));

    while (1)
    {                                                                           // Wait for new connection
        receiverAddressSize = sizeof receiverAddress;
        newReceiverFD       = accept(*socketFD, 
                                     (struct sockaddr *)&receiverAddress,
                                     &receiverAddressSize);                     // Accept new connection
        
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

            byteSend = send(newReceiverFD, "Hello, world!", 13, 0);
            if (byteSend == -1)
            {
                errorCode   = SEND;
                messageCode = SEND | NEW_LINE;
                printMessage();
            }

            sprintf(message, "%d byte send\n", (int)byteSend);
            print(message);

            close(newReceiverFD);
            exit(0);                                                            // Destroy child
        }

        close(newReceiverFD);                                                   // Parent doesn't need this
    }

    return OK;
}