#ifndef STUDENT_H_
#define STUDENT_H_

#define STUDENT_ID_MIN 1
#define STUDENT_ID_MAX 999999999

#include <stdbool.h>
#include "set.h"
#include "course_manager.h"
/**
 * Manages the student operations
 *
 * createStudent           - Creates an instance of student
 * copyStudent             - Copies a student
 * compareStudents         - Compare two students's ids
 * getStudent              - Get a student with a certain id from the system
 * isValidStudentID        - Checks if a student id is valid
 * getStudentID            - Get the students id
 * friendRequest           - Send a friend request from one student to another
 * handleFriendRequest     - Handle the request of the student with the "id"
 * addFriendship           - Create a friendship between two students
 * unFriend                - End a student's friendship with another student
 * removeFriendship        - Terminate a friendship between two students
 * destroyStudent          - Destroys an instance of student
 *
 * The following functions are available:
 *
 */

/** Type for defining the student */
typedef struct student_t *Student;

/** Type used for actions for the requests */
typedef enum action_t {
	ACCEPT,
	REJECT,
	NO_ACTION,
} Action;

/**
 * Creates a student
 *
 * @param1 student_id the students ID
 * @param2 first_name the first name of the student
 * @param3 last_name the last name of the student
 * @return
 * The student created. if there was an allocation error return NULL
 */
Student createStudent(int student_id, char* first_name, char* last_name);

/**
 * Copies a student
 *
 * @param1 student the student we want to copy
 * @return
 * NULL if there was an allocation failure or student = NULL
 * otherwise the student
 */
SetElement copyStudent(SetElement student);

/**
 * Compare two students's ids
 *
 * @param1 student the student we want to compare
 * @param2 student the student we want to compare
 * @return
 * 		A positive integer if the first student id is greater;
 * 		0 if they're equal or if one of the parameters is NULL;
 * 		A negative integer if the second student id is greater.
 */
int compareStudents(SetElement student1, SetElement student2);

/**
 * Get a student(NOT A COPY) with a certain id from the system
 *
 * @param1 student the student to retrieve from the system
 * @param2 student_id the id of the student
 * @return
 * the student if he exists, NULL otherwise
 */
Student getStudent(CourseManager course_manager, int student_id);

/**
 * Checks if a student id is valid
 *
 * @param1 student_id the id of the student that is checked
 * @return
 * true if the id is valid, false otherwise
 */
bool isValidStudentID(int student_id);

/**
 * Get the students id
 *
 * @param1 student the student which contains the id
 * @return
 * the id if valid, 0 otherwise
 */
int getStudentID(Student student);

/**
 * Sends a friend request from the logged in student to the student with the id
 *
 * @param1 course_manager the CourseManager that the student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @param3 id the student, which is receiving the friend request, ID
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_ALREADY_FRIEND,
 * MTM_STUDENT_DOES_NOT_EXIST, MTM_ALREADY_REQUESTED
 * true if there was no error
 */
bool friendRequest(CourseManager course_manager, Student student_in, int id);

/**
 * Handle the request of the student with the "id"
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @param2 id the student, which sent the friend request, ID
 * @param3 action if is "accept" then the two become friends, if "reject" then
 * decline the friend request
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_ALREADY_FRIEND,
 * MTM_STUDENT_DOES_NOT_EXIST, MTM_NOT_REQUESTED, MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool handleFriendRequest(CourseManager course_manager, Student student_in,
						 int id, char* action);

/**
 * Create a friendship between two students, the logged in student and the
 * student sent in the parameters
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @param3 student the student that will become friends with the logged in
 * student
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_INVALID_PARAMETERS, MTM_NOT_LOGGED_IN,
 * MTM_ALREADY_FRIEND
 * true if there was no error
 */
bool addFriendship(CourseManager course_manager, Student student_in,
				   Student student);

/**
 * End a student's friendship with another student
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @param3 id the student, which is being unfriended, ID
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_NOT_FRIEND,
 * MTM_STUDENT_DOES_NOT_EXIST
 * true if there was no error
 */
bool unFriend(CourseManager course_manager, Student student_in, int id);

/**
 * Terminate a friendship between two students, the logged in student and the
 * student sent in the parameters
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @param3 student the student that will cease to be friends with the logged in
 * student
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors:
 * true if there was no error
 */
bool removeFriendship(CourseManager course_manager, Student student_in,
					  Student student);

/**
 * Destroys an instance of student
 *
 * @param1 student the student we destroy
 */
void destroyStudent(SetElement student);


#endif /* STUDENT_H_ */
