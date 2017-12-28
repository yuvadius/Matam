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
 *   handleFriendRequest     - Logs in a student into the system
 */

/** Type for defining the student */
typedef struct student_t *Student;

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
 * course_manager->error.
 * true if there was no error
 */
bool friendRequest(CourseManager course_manager, char* id);

/**
 * Terminate a friendship between two students
 *
 * @param1 course_manager the CourseManager that the student is in
 * @param2 student the student that is logged in
 * @param3 id the student, which is being unfriended, ID
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool unFriend(CourseManager course_manager, Student student, char* id);

/**
 * Terminate a friendship between two students
 *
 * @param1 course_manager the CourseManager that the student is in
 * @param2 student the student that is logged in
 * @param3 id the student, which is being unfriended, ID
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool handleFriendRequest(CourseManager course_manager, char* student_id, 
						 char* request);

#endif /* STUDENT_H_ */
