#ifndef STUDENT_H_
#define STUDENT_H_

#include <stdbool.h>
/**
 * Manages the student operations
 *
 * The following functions are available:
 *
 *   createStudent           - Creates an instance of student
 *   friendRequest           - Send a friend request from one student to another
 *   unFriend                - End a student's friendship with another student
 *   handleFriendRequest     - Handle the request of the student with the "id"
 *   copyStudent             - Copys a student
 *   compareStudents         - Compare two students's ids
 *   getGrade                - Get the student's valid grade for clean report
 *   isValidStudentID        - Checks if a student id is valid
 *   addFriendship           - Create a friendship between two students
 *   removeFriendship        - Terminate a friendship between two students
 *   destroyStudent          - Destroys an instance of student
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
Student createStudent(char* student_id, char* first_name, char* last_name);

/**
 * Sends a friend request from the logged in student to the student with the id
 *
 * @param1 course_manager the CourseManager that the student is in
 * @param2 id the student, which is receiving the friend request, ID
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_ALREADY_FRIEND,
 * MTM_STUDENT_DOES_NOT_EXIST, MTM_ALREADY_REQUESTED
 * true if there was no error
 */
bool friendRequest(CourseManager course_manager, char* id);

/**
 * Terminate a friendship between two students
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 id the student, which is being unfriended, ID
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_NOT_FRIEND,
 * MTM_STUDENT_DOES_NOT_EXIST
 * true if there was no error
 */
bool unFriend(CourseManager course_manager, char* id);

/**
 * Handle the request of the student with the "id"
 *
 * @param1 course_manager the CourseManager that the logged in student is in
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
bool handleFriendRequest(CourseManager course_manager, char* id, char* action);

/**
 * Copys a student
 *
 * @param1 student the student we want to copy
 * @return
 * NULL if there was an allocation failure or student = NULL
 * otherwise the student
 */
Student copyStudent(Student student);

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
int compareStudents(Student student1, Student student2);

/**
 * Get the logged in student's valid grade for clean report
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 course_id the course ID of the grade
 * @return
 * the valid grade of the course
 * if there is no grade then return -1
 */
int getGrade(CourseManager course_manager, char* course_id);

/**
 * Checks if a student id is valid
 *
 * @param1 student_id the id of the student that is checked
 * @return
 * true if the id is valid, false otherwise
 */
bool isValidStudentID(char* student_id);

/**
 * Create a friendship between two students, the logged in student and the
 * student sent in the parameters
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student the student that will become friends with the logged in
 * student
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors:
 * true if there was no error
 */
bool addFriendship(CourseManager course_manager, Student student);

/**
 * Terminate a friendship between two students, the logged in student and the
 * student sent in the parameters
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student the student that will cease to be friends with the logged in
 * student
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors:
 * true if there was no error
 */
bool removeFriendship(CourseManager course_manager, Student student);

/**
 * Destroys an instance of student
 *
 * @param1 student the student we destroy
 */
void destroyStudent(Student student);

#endif /* STUDENT_H_ */
