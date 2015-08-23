#include <stddef.h>                                                             // For NULL
#include <sys/types.h>                                                          // For addrinfo structure
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>                                                             // For malloc()
#include <stdint.h>
#include "message.h"                                                            // Message codes

#include <stdio.h>                                                              // remove it

void printMessage();
int16_t getAddress(char *ip, struct addrinfo *addressInfo);
int16_t prepareSocket(struct addrinfo *addressInfo, int *socketFD);
int16_t prepareListen(int *socketFD);
int16_t sendFile(int *socketFD, const char *fileName);
int16_t receiveFile(int *socketFD);
int16_t fileOpen(const char *fileName);
int16_t init();                                                                 // Initialize app

int         errorCode         = OK;                                             // Global variable to hold error code
int8_t      messageCode       = OK;                                             // Global variable to hold message code
char        *appPath;                                                           // Get app path

int main(void)
{


    return 0;
}

