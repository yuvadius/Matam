#ifndef GRADE_H_
#define GRADE_H_

#include <stdbool.h>
/**
 * Manages the student operations
 *
 * The following functions are available:
 *
 *   createGrade             - Creates an instance of grade
 *   addGrade                - Adds a grade to the logged in student
 *   removeGrade             - Removes a grade from the logged in student
 *   updateGrade             - Update a grade for the logged in student
 *   reportFull              - Print the whole matriculation
 *	 reportClean             - Print the clean matriculation
 *   reportBest              - Print the best grades
 *   reportWorst             - Print the worst grades
 *   reportReference         - Print students with highest the grades for course
 *   destroyGrade            - Destroys an instance of grade
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
Grade createGrade(char* semester, char* course_id, char* points, char* grade);

/**
 * Adds a grade to the current student's semester
 *
 * @param1 course_manager the CourseManager that the student is in
 * @param2 semester the semester when the grade was received
 * @param3 course_id the id of the course
 * @param4 points the points that the course is worth
 * @param5 grade the grade received in the course
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool addGrade(CourseManager course_manager, char* semester, char* course_id,
				 char* points, char* grade);

/**
 * Removes a grade from the logged in student
 *
 * @param1 course_manager the CourseManager that the student is logged in
 * @param2 semester the semester when the grade was received
 * @param3 course_id the id of the course
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool removeGrade(CourseManager course_manager, char* semester, char* course_id);

/**
 * Update a grade for the logged in student
 *
 * @param1 course_manager the CourseManager that the student is logged in
 * @param2 course_id the id of the course
 * @param3 grade the grade received in the course
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool updateGrade(CourseManager course_manager, char* course_id, char* grade);

/**
 * Print the whole matriculation
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool reportFull(CourseManager course_manager);

/**
 * Print the clean matriculation
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool reportClean(CourseManager course_manager);

/**
 * Print the best grades
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 amount the amount of grades to print
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool reportBest(CourseManager course_manager, char* amount);

/**
 * Print the worst grades
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 amount the amount of grades to print
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool reportWorst(CourseManager course_manager, char* amount);

/**
 * Print students with highest the grades for course
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 course_id the id of the course
 * @param3 amount the amount of names to print
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool reportReference(CourseManager course_manager, char* course_id, 
					 char* amount);

/**
 * Destroys an instance of grade
 *
 * @param1 grade the grade we destroy
 */
void destroyGrade(Grade grade);

#endif /* GRADE_H_ */
