#include <stddef.h>                                                             // For NULL
#include <netdb.h>                                                              // For getaddrinfo()
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "address.h"
#include "message.h"                                                            // Message codes
#include "include.h"                                                            // For global error and message code

void printMessage();

int8_t getAddress(char *ip, struct addrinfo *addressInfo)
{
    int                 response;                                               // Response on calling getaddrinfo()
    struct addrinfo     addressHints, *addressInformation;                      // Hints to get address
                                                                                // Holds address information

    memset(&addressHints, 0, sizeof addressHints);

    addressHints.ai_family          = AF_UNSPEC;                                // Uses IPv4 or IPv6
    addressHints.ai_socktype        = SOCK_STREAM;                              // Uses TCP

    if (ip == NULL)
    {
        addressHints.ai_flags       = AI_PASSIVE;                               // Use machine IP address
        response = getaddrinfo(NULL, PORT, &addressHints, &addressInformation);
    }
    else
    {
        response = getaddrinfo(ip  , PORT, &addressHints, &addressInformation); // Use IP provided by machine
    }

    if (response != 0)
    {                                                                           // Could not get address
        errorCode       = response;
        messageCode     = ADDRESS | NEW_LINE;
        return ADDRESS;
    }
    else
    {                                                                           // Address success
        *addressInfo    = *addressInformation;
        return OK;
    }
}