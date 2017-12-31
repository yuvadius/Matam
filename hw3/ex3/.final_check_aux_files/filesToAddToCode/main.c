#include <stdio.h>
#include "mtm_ex3.h"

int main(int argc, char *argv[])
{
    if(argc != 1 && argc != 3 && argc != 5) {
        mtmPrintErrorMessage(stdout, MTM_INVALID_COMMAND_LINE_PARAMETERS);
    }
    for(int i = 1; i < argc; ++i) {
        printf("%s", argv[i]);
    }
    return 0;
}
