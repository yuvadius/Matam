#ifndef GRADE_H_
#define GRADE_H_

#define MIN_GRADE 0
#define MAX_GRADE 100
#define PASS_GRADE 55
#define SEMESTER_MIN 1
#define MIN_POINTS_X2 0

#include "set.h"
#include "list.h"
#include "mtm_ex3.h"
#include "course_manager.h"
#include "student.h"
#include <stdbool.h>
/**
 * Manages the student operations
 *
 * The following functions are available:
 * createGrade             - Creates an instance of grade
 * isValidSemester         - Checks if the semester is valid
 * isValidPointsX2         - Checks if the points*2 for a course is valid
 * isValidGrade            - Checks if the grade is valid
 * addGrade                - Adds a grade to the logged in student
 * destroyGrade            - Destroys an instance of grade
 */

/** Type for defining the grade */
typedef struct grade_t *Grade;

/**
 * Creates an instance of a grade
 *
 * @param1 semester the semester when the grade was received
 * @param2 course_id the id of the course
 * @param3 points the points that the course is worth
 * @param4 grade the grade received in the course
 * @return
 * The grade created. if there was an allocation error return NULL
 */
Grade createGrade(int semester, int course_id, int points_x2, int grade);

/**
 * Checks if the semester is valid
 *
 * @param1 semester the semester that is checked for validity
 * @return
 * true if the semester is valid, false otherwise
 */
bool isValidSemester(int semester);

/**
 * Checks if the points*2 for a course is valid
 *
 * @param1 points_x2 the points*2 that is checked for validity
 * @return
 * true if the points is valid, false otherwise
 */
bool isValidPointsX2(int points_x2);

/**
 * Checks if the grade is valid
 *
 * @param1 grade the grade that is checked for validity
 * @return
 * true if the grade is valid, false otherwise
 */
bool isValidGrade(int grade);

/**
 * Adds a grade to the current student's semester
 *
 * @param1 course_manager the CourseManager that the student is in
 * @param2 semester the semester when the grade was received
 * @param3 course_id the id of the course
 * @param4 points_x2 the points*2 that the course is worth
 * @param5 grade the grade received in the course
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool addGrade(CourseManager course_manager, Student student_in, List grades,
		int semester, int course_id, int points_x2, int grade);

/**
 * Destroys an instance of grade
 *
 * @param1 grade the grade we destroy
 */
void destroyGrade(Grade grade);

#endif /* GRADE_H_ */
