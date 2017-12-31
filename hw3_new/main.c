#include <stdio.h>
#include <string.h>
#include "mtm_ex3.h"

int main(int argc, char *argv[])
{
	//not a valid amount of paramaters
    if(argc != 1 && argc != 3 && argc != 5) {
    	mtmPrintErrorMessage(stdout, MTM_INVALID_COMMAND_LINE_PARAMETERS);
    }
    if(argc >= 3 && strcmp(argv[1], "-i") != 0) {
    	mtmPrintErrorMessage(stdout, MTM_INVALID_COMMAND_LINE_PARAMETERS);
    }
    if(argc == 5) {

    }
    for(int i = 0; i < argc; ++i) {
        printf("%s", argv[i]);
    }
    return 0;
}
