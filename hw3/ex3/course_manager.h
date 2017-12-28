#ifndef COURSE_MANAGER_H_
#define COURSE_MANAGER_H_

#include <stdbool.h>
/**
 * Manages the interactive transcripts system
 *
 * The following functions are available:
 *
 *   courseManagerCreate      - Creates an instance of CourseManager
 *   handleInput              - Receives an input command and processes it
 *   loginStudent             - Logs in a student into the system
 *   logoutStudent            - Logs out a student from the system
 *   removeStudent            - Remove a student from the system
 *   addStudent               - Add a student to the system
 *   facultyRequest           - Ignore the student faculty request
 */

/** Type for defining the system */
typedef struct course_manager_t *CourseManager;

/** Type used for requsts for the faculty */
typedef enum FacultyRequest_t {
	REMOVE_COURSE,
	REGISTER_COURSE,
	CANCEL_COURSE,
} FacultyRequest;

/**
 * Creates an empty CourseManager
 *
 * @return
 * The CourseManager created. if there was an allocation error return NULL
 * course_manager->error.
 */
CourseManager courseManagerCreate();

/**
 * Processes an input line command
 *
 * @param1 course_manager the CourseManager that the student will be logged into
 * @param2 input_line A line received from the input stream
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool handleInput(CourseManager course_manager, char* input_line);

/**
 * Logs in a student to the system
 *
 * @param1 course_manager the CourseManager that the student will be logged into
 * @param2 student_id the students ID
 * @return 
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool loginStudent(CourseManager course_manager, char* student_id);

/**
 * Logs out a student from the system
 *
 * @param1 course_manager CourseManager that the student will be logged out of
 * @param2 student_id the students ID
 * @return 
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool logoutStudent(CourseManager course_manager, char* student_id);

/**
 * Remove the student from the system
 *
 * @param1 course_manager CourseManager that the student will be removed from
 * @param2 student_id the students ID
 * @return 
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool removeStudent(CourseManager course_manager, char* student_id);

/**
 * add the student to the system
 *
 * @param1 course_manager the CourseManager that the student will be added to
 * @param2 student_id the students ID
 * @param3 first_name the first name of the student
 * @param4 last_name the last name of the student
 * @return 
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool addStudent(CourseManager course_manager, char* student_id, 
				char* first_name, char* last_name);

/**
 * Ignore the student faculty request
 *
 * @param1 course_manager CourseManager that the student will be removed from
 * @param2 student_id the students ID
 * @param3 requst the request of the student which can be "remove_course" or
 * "register_course" or "cancel_course"
 * @return 
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool facultyRequest(CourseManager course_manager, char* student_id,
					FacultyRequest requst);

#endif /* LIST_H_ */
