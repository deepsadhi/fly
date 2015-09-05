#include <stdio.h>                                                              // For fopen();
#include <errno.h> 
#include <stdint.h>
#include "message.h"                                                            // Message codes
#include "include.h"                                                            // For global error and message code

int16_t fileOpen(const char *fileName)
{
    FILE *file;

    file = fopen(fileName, "r");

    if (file == 0)
    {                                                                           // Could not open file
        errorCode   = errno;
        messageCode = FILE_OPEN | NEW_LINE;
        return FILE_OPEN;
    }

    return OK;
}

