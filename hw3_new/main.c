#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "mtm_ex3.h"

/**
* setInputOutputFiles - Sets the input file and output file according to the
* parameters received in the command line
*
* @param input_file - the input file that will be used across the program
* @param output_file - the output file that will be used across the program
* @param argc - the amount of parameters received from the command line
* @param argv - an array of oarameters received in the command line
* @return true if no error occured and false if invalid command line parameters
* were received(in this case we should stop the execution of the program)
*/
bool setInputOutputFiles(FILE** input_file, FILE** output_file, int argc,
						 char *argv[]) {
	*input_file = stdin; //default as standard input file
	*output_file = stdout; //default as standard output file
	//not a valid amount of parameters
	if(argc != 1 && argc != 3 && argc != 5) {
		mtmPrintErrorMessage(stdout, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return false;
	}
	if(argc >= 3) {
		if(strcmp(argv[1], "-i") != 0) {
			mtmPrintErrorMessage(stdout, MTM_INVALID_COMMAND_LINE_PARAMETERS);
			return false;
		}
		else {
			*input_file = fopen(argv[2], "r");
		}
	}
	if(argc == 5) {
		if(strcmp(argv[3], "-o") != 0) {
			mtmPrintErrorMessage(stdout, MTM_INVALID_COMMAND_LINE_PARAMETERS);
			return false;
		}
		else {
			*output_file = fopen(argv[4], "w");
		}
	}
	return true;
}

int main(int argc, char *argv[])
{
	//the input and output file that will be used in the program
	FILE* input_file = NULL;
	FILE* output_file = NULL;
	//if the command line parameters are invalid
	if(setInputOutputFiles(&input_file, &output_file, argc, argv) == false) {
		return 0;
	}
    return 0;
}
