#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "set.h"
#include "mtm_ex3.h"
#include "course_manager.h"
#include "student.h"

struct course_manager_t {
	Set students;
	Student current_student;
	MtmErrorCode error;
	FILE* output_channel;
};

/**
 * Creates an empty CourseManager
 *
 * @param1 output_file the output stream that will be written to
 * @return
 * The CourseManager created. if there was an allocation error return NULL
 */
CourseManager createCourseManager(FILE* output_file) {
	//create an instance of a course manager
	CourseManager course_manager = malloc(sizeof(*course_manager));
	if(course_manager == NULL) { //if there was a memory failure return NULL
		return NULL;
	}
	course_manager->students = setCreate(copyStudent, destroyStudent,
										 compareStudents);
	//if memory allocation failed
	if(course_manager->students == NULL) {
		destroyCourseManager(course_manager);
		return NULL;
	}
	course_manager->current_student = NULL;
	course_manager->output_channel = output_file;
	return course_manager;
}

/**
 * Checks if the error in course manager is critical
 *
 * @param1 course_manager the course manager that contains the error
 * @return
 * true if the error is critical or if course manager is NULL, false otherwise
 */
bool isCriticalError(CourseManager course_manager) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return true;
	}
	//check if the error is critical
	switch(course_manager->error) {
		case MTM_OUT_OF_MEMORY:
		case MTM_INVALID_COMMAND_LINE_PARAMETERS:
		case MTM_CANNOT_OPEN_FILE:
			return true;
		default:
			return false;
	}
}

/**
 * Calls the appropriate student function
 *
 * @param1 course_manager the course manager that contains the system
 * @param2 token contains the sub-command(second parameter) received in the
 * command line. to receive the next argument from the command line
 * use: "token = strtok(NULL, del);". token will contain the next parameter
 * that was sent in the command line
 * @param3 del the delimiter of the input line(should be " ")
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool studentInput(CourseManager course_manager, char* token, const char del[2]){
	if(strcmp(token, "login") == 0) {
		token = strtok(NULL, del); //get the third argument(id)

		//loginStudent()
	}
	return true;
}

/**
 * Calls the appropriate grade function
 *
 * @param1 course_manager the course manager that contains the system
 * @param2 token contains the sub-command(second parameter) received in the
 * command line. to receive the next argument from the command line
 * use: "token = strtok(NULL, del);". token will contain the next parameter
 * that was sent in the command line
 * @param3 del the delimiter of the input line(should be " ")
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool gradeInput(CourseManager course_manager, char* token, const char del[2]) {
	return true;
}

/**
 * Calls the appropriate report function
 *
 * @param1 course_manager the course manager that contains the system
 * @param2 token contains the sub-command(second parameter) received in the
 * command line. to receive the next argument from the command line
 * use: "token = strtok(NULL, del);". token will contain the next parameter
 * that was sent in the command line
 * @param3 del the delimiter of the input line(should be " ")
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool reportInput(CourseManager course_manager, char* token, const char del[2]) {
	return true;
}

/**
 * Processes an input line command
 *
 * @param1 course_manager the CourseManager that the student will be logged into
 * @param2 input_line a line received from the input stream
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool handleInput(CourseManager course_manager, char* input_line) {
	//nothing to do if the parameters aren't set
	if(course_manager == NULL || input_line == NULL) {
		return true;
	}
	const char delimiter[2] = " "; //the delimiter, break words between spaces
	char *token = strtok(input_line, delimiter); // get the first token
	if(token == NULL || token[0] == '#') { //if empty line or comment line
		return true;
	}
	if(strcmp(token, "student") == 0) {
		token = strtok(NULL, delimiter); //get the sub command
		return studentInput(course_manager, token, delimiter);
	}
	else if(strcmp(token, "grade_sheet") == 0) {
		token = strtok(NULL, delimiter); //get the sub command
		return gradeInput(course_manager, token, delimiter);
	}
	else if(strcmp(token, "report") == 0) {
		token = strtok(NULL, delimiter); //get the sub command
		return reportInput(course_manager, token, delimiter);
	}
	else { //invalid command, shouldn't happen
		return true;
	}
}

/**
 * return the last "error" produced by one of the funcs.
 *
 * @param1 course_manager CourseManager that we take the error from.
 * @return 
 * returns course_manager->error.
 * NULL if something went wrong.
 */
MtmErrorCode getCourseManagerError(CourseManager course_manager) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	return course_manager->error;
}

/**
 * Checks if the course id is valid
 *
 * @param1 course_id the course id that will be checked
 * @return
 * true if the course id is valid false otherwise
 */
bool isValidCourseID(char* course_id) {
	int course_id_int = atoi(course_id); //string to int function
	if(course_id_int >= COURSE_ID_MIN && course_id_int <= COURSE_ID_MAX) {
		return true;
	}
	else {
		return false;
	}
}

/**
 * Destroy an instance of CourseManager
 *
 * @param1 course_manager the CourseManager we destroy
 */
void destroyCourseManager(CourseManager course_manager) {
	//nothing to do if course_manager isn't set
	if(course_manager == NULL) {
		return;
	}
	//setDestroy(course_manager->students); //destroy all students in the system
	free(course_manager);
}
