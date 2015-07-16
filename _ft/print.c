#include "fly.h"
#include "message.h"
#include <stdio.h>

void print(UNSIGNED_CHAR messageCode)
{
    fprintf(stdout, "%s", char[messageCode]);
}

void prtintError(UNSIGNED_CHAR messageCode)
{
    fprintf(stderr, "%s", char[messageCode]);
}
