#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"

noreturn void panic(const char *message)
{
    fprintf(stderr, "[PANIC] %s\n", message);
    exit(EXIT_FAILURE);
}
