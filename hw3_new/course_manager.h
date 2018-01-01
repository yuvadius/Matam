#ifndef COURSE_MANAGER_H_
#define COURSE_MANAGER_H_

#define COURSE_ID_MIN 1
#define COURSE_ID_MAX 999999

#include <stdbool.h>
#include <stdio.h>
#include "mtm_ex3.h"
#include "set.h"
/**
 * Manages the interactive transcripts system
 *
 * The following functions are available:
 *
 *   createCourseManager     - Creates an instance of CourseManager
 *   isCriticalError         - Checks if the error in course manager is critical
 *   studentInput            - Calls the appropriate student function
 *   gradeInput              - Calls the appropriate grade function
 *   reportInput             - Calls the appropriate report function
 *   handleInput             - Receives an input command and processes it
 *   getCourseManagerError   - Return the last "error" in the course manager
 *   isValidCourseID         - Checks if the course id is valid
 *   loginStudent            - Logs in a student into the system
 *   getStudents             - Gets the set of students from the system
 *   logoutStudent           - Logs out a student from the system
 *   addStudent              - Add a student to the system
 *   removeStudent           - Remove a student from the system
 *   setError                - Set the error of the course manager
 *   getOutputChannel        - Get the system's output channel
 *   destroyCourseManager    - Destroy an instance of CourseManager
 */

/** Type for defining the system */
typedef struct course_manager_t *CourseManager;

/** Type used for requsts for the faculty */
typedef enum FacultyRequest_t {
	REMOVE_COURSE,
	REGISTER_COURSE,
	CANCEL_COURSE,
	NO_REQUEST,
} FacultyRequest;

/**
 * Creates an empty CourseManager
 *
 * @param1 output_file the output stream that will be written to
 * @return
 * The CourseManager created. if there was an allocation error return NULL
 */
CourseManager createCourseManager(FILE* output_file);

/**
 * Checks if the error in course manager is critical
 *
 * @param1 course_manager the course manager that contains the error
 * @return
 * true if the error is critical, false if not
 */
bool isCriticalError(CourseManager course_manager);

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
bool studentInput(CourseManager course_manager, char* token, const char del[2]);

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
bool gradeInput(CourseManager course_manager, char* token, const char del[2]);

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
bool reportInput(CourseManager course_manager, char* token, const char del[2]);

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
bool handleInput(CourseManager course_manager, char* input_line);

/**
 * return the last "error" produced by one of the functions
 *
 * @param1 course_manager CourseManager that we take the error from.
 * @return 
 * returns course_manager->error.
 */
MtmErrorCode getCourseManagerError(CourseManager course_manager);

/**
 * Checks if the course id is valid
 *
 * @param1 course_id the course id that will be checked
 * @return
 * true if the course id is valid false otherwise
 */
bool isValidCourseID(int course_id);

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
bool loginStudent(CourseManager course_manager, int student_id);

/**
 * Gets the set of students from the system
 *
 * @param1 course_manager the CourseManager that the students are in
 * @return
 * NULL on failure, otherwise return the set of students
 */
Set getStudents(CourseManager course_manager);

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
bool logoutStudent(CourseManager course_manager);

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
				char* last_name);

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
bool removeStudent(CourseManager course_manager, int student_id);

/**
 * Remove the student from the system
 *
 * @param1 course_manager CourseManager that has the error variable
 * @param2 error the error to insert into the course manager
 */
void setError(CourseManager course_manager, MtmErrorCode error);

/**
 * Get the system's output channel
 *
 * @param1 course_manager CourseManager that has the output channel
 * @return
 * the output file, if there was an error then NULL
 */
FILE* getOutputChannel(CourseManager course_manager);

/**
 * Destroy an instance of CourseManager
 *
 * @param1 course_manager the CourseManager we destroy
 */
void destroyCourseManager(CourseManager course_manager);

#endif /* COURSE_MANAGER_H_ */
