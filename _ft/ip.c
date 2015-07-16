#include "fly.h"
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

void print(char *message);


void getIPAddress(char *IP, struct addrinfo  *servinfo)
{
    int                       rv;
    struct addrinfo           hints;                                            // Gives hints about getting IP

    memset(&hints, 0, sizeof hints);

    hints.ai_family         = AF_UNSPEC;                                        // Uses IPv4 or IPv6
    hints.ai_socktype       = SOCK_STREAM;                                      // Uses TCP


/*    if (IP == NULL)
    {
        hints.ai_flags      = AI_PASSIVE;                                       // Us my IP address
        rv = getaddrinfo(NULL, PORT_NUMBER, &hints, &servinfo);
    }

    else
    {
        rv = getaddrinfo(IP, PORT_NUMBER, &hints, &servinfo);
    }*/

        rv = getaddrinfo(IP, PORT_NUMBER, &hints, &servinfo);
    if (rv != 0)
    {
        printf("getaddrinfo: %s\n", gai_strerror(rv));
    }
    
}
        
