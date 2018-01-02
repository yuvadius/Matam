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
	int current_student; //the id of the logged in student
	MtmErrorCode error;
	FILE* output_channel;
};

/**
 * Converts a faculty request string to a constant
 *
 * @param1 request the faculty request string
 * @return
 * The FacultyRequest representing the faculty request string
 */
static FacultyRequest getFacultyRequst(char* request) {
	if(strcmp(request, "remove_course") == 0) {
		return REMOVE_COURSE;
	}
	else if(strcmp(request, "register_course") == 0) {
		return REGISTER_COURSE;
	}
	else if(strcmp(request, "cancel_course") == 0) {
		return CANCEL_COURSE;
	}
	else {
		return NO_REQUEST;
	}
}

/**
 * Replaces an occurrence of a character in a string with another character
 *
 * @param1 str the string which will have a character replaced
 * @param2 find the character in str that will be replaced
 * @param3 replace the character that will replace the character "find"
 */
static void replaceChar(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos){
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
}

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
	course_manager->current_student = NO_STUDENT_LOGGED_IN;
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
	else if(strcmp(token, "remove") == 0) {
		token = strtok(NULL, del); //get the third argument(id)
		return removeStudent(course_manager, atoi(token));
	}
	else if(strcmp(token, "friend_request") == 0) {
		token = strtok(NULL, del); //get the third argument(id)
		return friendRequest(course_manager, getCurrentStudent(course_manager),
							 atoi(token));
	}
	else if(strcmp(token, "handle_request") == 0) {
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
										  getCurrentStudent(course_manager),
										  id, action);
		free(action);
		return result;
	}
	else if(strcmp(token, "unfriend") == 0) {
		token = strtok(NULL, del); //get the third argument(id)
		return unFriend(course_manager, getCurrentStudent(course_manager),
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
		char* points = malloc(strlen(token) + 1);
		if(points == NULL) { //if memory allocation failed
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		strcpy(points, token); //get the action
		token = strtok(NULL, del); //get the sixth argument(grade)
		int grade = atoi(token);
		bool result = addGrade(course_manager,getCurrentStudent(course_manager),
						getStudentGrades(getCurrentStudent(course_manager)),
						semester, course_id, validatePoints(points), grade);
		free(points);
		return result;
	}
	else if(strcmp(token, "remove") == 0) {
		token = strtok(NULL, del); //get the third argument(semester)
		int semester = atoi(token);
		token = strtok(NULL, del); //get the fourth argument(course_id)
		int course_id = atoi(token);
		return removeGrade(course_manager, getCurrentStudent(course_manager),
						   getStudentGrades(getCurrentStudent(course_manager)),
						   semester, course_id);
	}
	else if(strcmp(token, "update") == 0) {
		token = strtok(NULL, del); //get the third argument(course_id)
		int course_id = atoi(token);
		token = strtok(NULL, del); //get the fourth argument(grade)
		int grade = atoi(token);
		return updateGrade(course_manager, getCurrentStudent(course_manager),
						   getStudentGrades(getCurrentStudent(course_manager)),
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
		return reportFull(course_manager, getCurrentStudent(course_manager),
						  getStudentGrades(getCurrentStudent(course_manager)));
	}
	else if(strcmp(token, "clean") == 0) {
		return reportClean(course_manager, getCurrentStudent(course_manager),
						   getStudentGrades(getCurrentStudent(course_manager)));
	}
	else if(strcmp(token, "best") == 0) {
		token = strtok(NULL, del); //get the third argument(amount)
		int amount = atoi(token);
		return reportBest(course_manager, getCurrentStudent(course_manager),
						  getStudentGrades(getCurrentStudent(course_manager)),
						  amount);
	}
	else if(strcmp(token, "worst") == 0) {
		token = strtok(NULL, del); //get the third argument(amount)
		int amount = atoi(token);
		return reportWorst(course_manager, getCurrentStudent(course_manager),
						   getStudentGrades(getCurrentStudent(course_manager)),
						   amount);
	}
	else if(strcmp(token, "reference") == 0) {
		token = strtok(NULL, del); //get the third argument(course id)
		int course_id = atoi(token);
		token = strtok(NULL, del); //get the fourth argument(amount)
		int amount = atoi(token);
		return reportReference(course_manager,getCurrentStudent(course_manager),
						   getStudentFriends(getCurrentStudent(course_manager)),
						   course_id, amount);
	}
	else if(strcmp(token, "faculty_request") == 0) {
		token = strtok(NULL, del); //get the third argument(course id)
		int course_id = atoi(token);
		token = strtok(NULL, del); //get the fourth argument(request)
		char* request = malloc(strlen(token) + 1);
		if(request == NULL) { //if memory allocation failed
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		strcpy(request, token); //get the request
		bool result = facultyRequest(course_manager, course_id, request);
		free(request);
		return result;
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
	replaceChar(input_line, '\n', ' '); //remove the newlines from line
	replaceChar(input_line, '\t', ' '); //replace tabs with spaces
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
 * return the last "error" produced by one of the functions.
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
	if(course_manager->current_student != NO_STUDENT_LOGGED_IN) {
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
		course_manager->current_student = getStudentID(student);
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
	if(course_manager->current_student == NO_STUDENT_LOGGED_IN) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	course_manager->current_student = NO_STUDENT_LOGGED_IN; //logout the student
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
	if(course_manager->current_student == student_id) {
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
 * Ignore the student faculty request
 *
 * @param1 course_manager CourseManager that the student will be removed from
 * @param2 course_id the course ID
 * @param3 request the request of the student which can be "remove_course" or
 * "register_course" or "cancel_course"
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN,
 * MTM_COURSE_DOES_NOT_EXIST, MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool facultyRequest(CourseManager course_manager, int course_id, char* request){
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if no student is logged in to the system
	if(course_manager->current_student == NO_STUDENT_LOGGED_IN) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	//if the request is to remove a course that does not exist
	if(getNewestGrade(getCurrentStudent(course_manager), course_id) == -1
			&& getFacultyRequst(request) == REMOVE_COURSE) {
		setError(course_manager, MTM_COURSE_DOES_NOT_EXIST);
		return false;
	}
	//if the parameters aren't valid return MTM_INVALID_PARAMETERS
	if(getFacultyRequst(request) == NO_REQUEST) {
		setError(course_manager, MTM_INVALID_PARAMETERS);
		return false;
	}
	mtmFacultyResponse(course_manager->output_channel,"your request was rejected");
	return true;
}

/**
 * Get the logged in student
 *
 * @param1 course_manager CourseManager that the student is in
 * @return
 * on success the student. on failure NULL
 */
void* getCurrentStudent(CourseManager course_manager) {
	if(course_manager == NULL) {
		return NULL;
	}
	else if(course_manager->current_student == NO_STUDENT_LOGGED_IN) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return NULL;
	}
	else {
		SET_FOREACH(Student, student, course_manager->students) {
			if(getStudentID(student) == course_manager->current_student) {
				return student;
			}
		}
		return NULL; //no student was found
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
