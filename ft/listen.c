#include <sys/types.h>                                                          // For listen()
#include <sys/socket.h>
#include <signal.h>                                                             // For sigaction()
#include <sys/wait.h>                                                           // For waitpid()
#include <stddef.h>                                                             // For NULL

#include "include.h"                                                            // For global error and message code
#include "message.h"                                                            // Message codes
#include "listen.h"

void printMessage();

void sigChildHandler(int s)
{
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int16_t prepareListen(int *socketFD)
{
    struct sigaction        sigAction;

    if (listen(*socketFD, BACK_LOG) == -1)                                      // Start listening
    {                                                                           // Listen failed
        errorCode   = LISTEN;
        messageCode = LISTEN | NEW_LINE;
        return LISTEN;
    }

    sigAction.sa_handler    = sigChildHandler;                                  // Reap all dead process
    sigemptyset(&sigAction.sa_mask);
    sigAction.sa_flags      = SA_RESTART;

    if (sigaction(SIGCHLD, &sigAction, NULL) == -1)
    {
        errorCode   = SIGACTION;
        messageCode = SIGACTION | NEW_LINE;
        return SIGACTION;
    }


    return OK;
}

