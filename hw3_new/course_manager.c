#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "set.h"
#include "mtm_ex3.h"
#include "course_manager.h"
#include "student.h"
#include "grade.h"

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
		return loginStudent(course_manager, atoi(token));
	}
	else if(strcmp(token, "logout") == 0) {
		return logoutStudent(course_manager);
	}
	else if(strcmp(token, "add") == 0) {
		token = strtok(NULL, del); //get the third argument(id)
		int id = atoi(token);
		token = strtok(NULL, del); //get the fourth argument(first name)
		char* first_name = malloc(strlen(token) + 1);
		if(first_name == NULL) { //if memory allocation failed
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		strcpy(first_name, token); //get the first name
		token = strtok(NULL, del); //get the fifth argument(last name)
		char* last_name = malloc(strlen(token) + 1);
		if(last_name == NULL) { //if memory allocation failed
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		strcpy(last_name, token); //get the last name
		bool result = addStudent(course_manager, id, first_name, last_name);
		free(last_name);
		free(first_name);
		return result;
	}
	else if(strcmp(token, "remove")) {
		token = strtok(NULL, del); //get the third argument(id)
		return removeStudent(course_manager, atoi(token));
	}
	else if(strcmp(token, "friend_request")) {
		token = strtok(NULL, del); //get the third argument(id)
		return friendRequest(course_manager, course_manager->current_student,
							 atoi(token));
	}
	else if(strcmp(token, "handle_request")) {
		token = strtok(NULL, del); //get the third argument(id)
		int id = atoi(token);
		token = strtok(NULL, del); //get the fourth argument(action)
		char* action = malloc(strlen(token) + 1);
		if(action == NULL) { //if memory allocation failed
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		strcpy(action, token); //get the action
		bool result = handleFriendRequest(course_manager,
						  	  	  	  	  course_manager->current_student,
										  id, action);
		free(action);
		return result;
	}
	else if(strcmp(token, "unfriend")) {
		token = strtok(NULL, del); //get the third argument(id)
		return unFriend(course_manager, course_manager->current_student,
							 atoi(token));
	}
	else { //invalid sub-command, shouldn't reach this place
		return true;
	}
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
	if(strcmp(token, "add") == 0) {
		token = strtok(NULL, del); //get the third argument(semester)
		int semester = atoi(token);
		token = strtok(NULL, del); //get the fourth argument(course_id)
		int course_id = atoi(token);
		token = strtok(NULL, del); //get the fifth argument(points)
		int points_x2 = (int)(atof(token) * 2);
		token = strtok(NULL, del); //get the sixth argument(grade)
		int grade = atoi(token);
		return addGrade(course_manager, course_manager->current_student,
						getStudentGrades(course_manager->current_student),
						semester, course_id, points_x2, grade);
	}
	else if(strcmp(token, "remove") == 0) {
		token = strtok(NULL, del); //get the third argument(semester)
		int semester = atoi(token);
		token = strtok(NULL, del); //get the fourth argument(course_id)
		int course_id = atoi(token);
		return removeGrade(course_manager, course_manager->current_student,
						   getStudentGrades(course_manager->current_student),
						   semester, course_id);
	}
	else if(strcmp(token, "update") == 0) {
		token = strtok(NULL, del); //get the third argument(course_id)
		int course_id = atoi(token);
		token = strtok(NULL, del); //get the fourth argument(grade)
		int grade = atoi(token);
		return removeGrade(course_manager, course_manager->current_student,
						   getStudentGrades(course_manager->current_student),
						   course_id, grade);
	}
	else { //invalid sub-command, shouldn't reach this place
		return true;
	}
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
	if(strcmp(token, "full") == 0) {
		return reportFull(course_manager, course_manager->current_student,
						  getStudentGrades(course_manager->current_student));
	}
	else { //invalid sub-command, shouldn't reach this place
		return true;
	}
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
bool isValidCourseID(int course_id) {
	if(course_id >= COURSE_ID_MIN && course_id <= COURSE_ID_MAX) {
		return true;
	}
	else {
		return false;
	}
}

/**
 * Logs in a student to the system
 *
 * @param1 course_manager the CourseManager that the student will be logged into
 * @param2 student_id the students ID
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * Possible Non Critical Errors: MTM_ALREADY_LOGGED_IN,
 * MTM_STUDENT_DOES_NOT_EXIST
 * true if there was no error
 */
bool loginStudent(CourseManager course_manager, int student_id) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student is logged in return MTM_ALREADY_LOGGED_IN
	if(course_manager->current_student != NULL) {
		setError(course_manager, MTM_ALREADY_LOGGED_IN);
		return false;
	}
	//find the student in the set
	Student student = getStudent(course_manager, student_id);
	//if the student wasn't found
	if(student == NULL) {
		setError(course_manager, MTM_STUDENT_DOES_NOT_EXIST);
		return false;
	}
	else {
		course_manager->current_student = student;
		return true;
	}
}

/**
 * Gets the set of students from the system
 *
 * @param1 course_manager the CourseManager that the students are in
 * @return
 * NULL on failure, otherwise return the set of students
 */
Set getStudents(CourseManager course_manager) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return NULL;
	}
	return course_manager->students;
}

/**
 * Logs out a student from the system
 *
 * @param1 course_manager CourseManager that the student will be logged out of
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN
 * true if there was no error
 */
bool logoutStudent(CourseManager course_manager) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student is not logged in return MTM_NOT_LOGGED_IN
	if(course_manager->current_student == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	course_manager->current_student = NULL; //logout the student
	return true;
}

/**
 * Add the student to the system
 *
 * @param1 course_manager the CourseManager that the student will be added to
 * @param2 student_id the students ID
 * @param3 first_name the first name of the student
 * @param4 last_name the last name of the student
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_STUDENT_ALREADY_EXISTS,
 * MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool addStudent(CourseManager course_manager, int student_id, char* first_name,
				char* last_name) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//find the student in the set
	SET_FOREACH(Student, student, course_manager->students) {
		//if the student was found
		if(getStudentID(student) == student_id) {
			setError(course_manager, MTM_STUDENT_ALREADY_EXISTS);
			return false;
		}
	}
	//student doesn't exist
	if(isValidStudentID(student_id) == false) { //if the id isn't valid
		setError(course_manager, MTM_INVALID_PARAMETERS);
		return false;
	}
	//create a student
	Student student = createStudent(student_id, first_name, last_name);
	//if there was a memory allocation failure
	if(student == NULL) {
		setError(course_manager, MTM_OUT_OF_MEMORY);
		return false;
	}
	//add a copy of student to the set
	SetResult result = setAdd(course_manager->students, student);
	destroyStudent(student); //because a copy was added
	if(result == SET_OUT_OF_MEMORY) {
		setError(course_manager, MTM_OUT_OF_MEMORY);
		return false;
	}
	else {
		return true;
	}
}

/**
 * Remove the student from the system
 *
 * @param1 course_manager CourseManager that the student will be removed from
 * @param2 student_id the students ID
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * Possible Non Critical Errors: MTM_STUDENT_DOES_NOT_EXIST
 * true if there was no error
 */
bool removeStudent(CourseManager course_manager, int student_id) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//find the student in the set
	Student student = getStudent(course_manager, student_id);
	//if the student wasn't found
	if(student == NULL) {
		setError(course_manager, MTM_STUDENT_DOES_NOT_EXIST);
		return false;
	}
	//log out the student if the student is logged in
	if(getStudentID(course_manager->current_student) == student_id) {
		logoutStudent(course_manager);
	}
	if(setRemove(course_manager->students, student) == SET_SUCCESS) {
		return true;
	}
	else { //something went horribly wrong
		return false;
	}
}

/**
 * Remove the student from the system
 *
 * @param1 course_manager CourseManager that has the error variable
 * @param2 error the error to insert into the course manager
 */
void setError(CourseManager course_manager, MtmErrorCode error) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return;
	}
	course_manager->error = error;
}

/**
 * Get the system's output channel
 *
 * @param1 course_manager CourseManager that has the output channel
 * @return
 * the output file, if there was an error then NULL
 */
FILE* getOutputChannel(CourseManager course_manager) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return NULL;
	}
	else {
		return course_manager->output_channel;
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
