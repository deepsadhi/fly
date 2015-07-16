#include <sys/types.h>                                                          // For socket()
#include <sys/socket.h>
#include <errno.h>
#include <stddef.h>                                                             // For NULL
#include <sys/types.h>                                                          // getaddrinfo() and it's structure
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>                                                             // close()
#include <stdlib.h>                                                             // For malloc()
#include "message.h"                                                            // Message codes
#include "include.h"                                                            // For global error and message code

void printMessage();

int16_t prepareSocket(struct addrinfo *addressInfo, int *socketFD)
{
    int                 yes = 1;
    struct addrinfo     *address;                                               // List of possible address information

    address = (struct addrinfo*)malloc(sizeof(struct addrinfo));                // Allocate memory

 /* Loop through all address until we can bind
  */
    for (address = addressInfo; address != NULL; address = address->ai_next)
    {
        if ((*socketFD = socket(address->ai_family, address->ai_socktype,
                                address->ai_protocol)) == -1)                   // Trying to create socket
        {                                                                       // socket creation failed
            messageCode = SOCKET | NEW_LINE;
            errorCode   = errno;
            printMessage();
            continue;
        }

        if (messageCode == RECEIVER)                                            // Check if user is receiver or sender
        {                                                                       // Make connection to socket

            while (connect(*socketFD, address->ai_addr, address->ai_addrlen)
                   == -1);// Trying to connect
            // if (connect(*socketFD, address->ai_addr, address->ai_addrlen) == -1)// Trying to connect
            // {                                                                   // Connection failed
            //     close(*socketFD);
            //     errorCode   = errno;
            //     messageCode = CONNECT | NEW_LINE;
            //     printMessage();
            //     continue;
            // }
        }
        else
        {                                                                       // setsockopt and bind socket
            if (setsockopt(*socketFD, SOL_SOCKET, SO_REUSEADDR, &yes,
                           sizeof(int)) == -1)                                  // Trying for setsockopt
            {                                                                   // setsockopt failed
                messageCode = SET_SOCK_OPT | NEW_LINE;
                errorCode   = errno;
                printMessage();
                return 1;
            }

            if (bind(*socketFD, address->ai_addr, address->ai_addrlen) == -1)   // Trying to bind
            {                                                                   // Binging failed
                close(*socketFD);
                messageCode = BIND | NEW_LINE;
                errorCode   = errno;
                printMessage();
                continue;
            }
        }

        break;
    }

    freeaddrinfo(addressInfo);                                                  // Release memory

    if (address == NULL)
    {
        if (messageCode == RECEIVER)
        {                                                                       // Receiver connection failed
            messageCode = CONNECTION_FAILED | NEW_LINE;
            return CONNECTION_FAILED;
        }
        else
        {                                                                       // Sender binding failed
            messageCode = BIND_FAIL | NEW_LINE;
            return BIND_FAIL;
        }
    }
    else
    {                                                                           // Binding success
        return OK;
    }

}
