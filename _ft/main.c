/**
 * project Fly
 */

#include "fly.h"
#include "message.h"
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

void print(UNSIGNED_CHAR messageCode);
void getIPAddress(char *IP, struct addrinfo *servinfo);
void prepareReceiver(struct addrinfo *servinfo, int socketFd);

int main(int argc, char *argv[])
{
    struct addrinfo *servinfo;
    int socketFd;

    memset(&servinfo, 0, sizeof servinfo);
    switch (argc)
    {
        case 1:                                                                 // Two arugments provided. Fly acts as receiver. 
        {
            getIPAddress(NULL, servinfo);
            prepareReceiver(servinfo, socketFd);
            break;
        }

        case 3:                                                                 // Three arguments provided. Fly acts as sender.
        {
            getIPAddress(argv[1], servinfo);
            break;
        }

        default:                                                                // Invalid! number of arguments. Cannot perform operation. 
        {
            print(INVALID_NUMBER_OF_ARGUMENT);
        }
    }  

    return 0;
}
