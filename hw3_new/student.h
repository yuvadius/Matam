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
 * destroyStudent          - Destroys an instance of student
 *
 * The following functions are available:
 *
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
 * Destroys an instance of student
 *
 * @param1 student the student we destroy
 */
void destroyStudent(SetElement student);


#endif /* STUDENT_H_ */
