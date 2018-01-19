#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "mtm_ex3.h"
#include "course_manager.h"

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
	if(argc == 2 || argc == 4) { //there must be files specified after -i and -o
		mtmPrintErrorMessage(stdout, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return false;
	}
	if(argc >= 2 && strcmp(argv[1], "-i") != 0) {
		mtmPrintErrorMessage(stdout, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return false;
	}
	if(argc >= 4 && strcmp(argv[3], "-o") != 0) {
		mtmPrintErrorMessage(stdout, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return false;
	}
	if(argc >= 3) {
		*input_file = fopen(argv[2], "r");
		if(*input_file == NULL) { //if file couldn't be opened
			mtmPrintErrorMessage(stdout, MTM_CANNOT_OPEN_FILE);
			return false;
		}
	}
	if(argc >= 5) {
		*output_file = fopen(argv[4], "w");
		if(*output_file == NULL) { //if file couldn't be opened
			mtmPrintErrorMessage(stdout, MTM_CANNOT_OPEN_FILE);
			return false;
		}
	}
	return true;
}

/**
* closeFiles - Closes the input and output files opened with fopen
*
* @param input_file - the input file that will be closed if needed
* @param output_file -the output file that will be closed if needed
*/
void closeFiles(FILE* input_file, FILE* output_file) {
	//if input_file was successfully opened with fopen, close the input file
	if(input_file != NULL && input_file != stdin) {
		fclose(input_file);
	}
	//if output_file was successfully opened with fopen, close the input file
	if(output_file != NULL && output_file != stdout) {
		fclose(output_file);
	}
}

int main(int argc, char *argv[])
{
	//the input and output file that will be used in the program
	FILE* input = NULL;
	FILE* output = NULL;
	FILE* error = stderr;
	//if the command line parameters are invalid
	if(setInputOutputFiles(&input, &output, argc, argv) == false) {
		closeFiles(input, output);
		return 0;
	}
	else {
		char line[MAX_LEN];
		CourseManager manager = createCourseManager(output);
		while(fgets(line, MAX_LEN, input) != NULL) { //read line into "line"
			if(handleInput(manager, line) == false) { //if there was an error
				mtmPrintErrorMessage(error, getCourseManagerError(manager));
				if(isCriticalError(manager) == true) { //exit if error critical
					return 0;
				}
			}
		}
		destroyCourseManager(manager);
	}
	closeFiles(input, output);
    return 0;
}
