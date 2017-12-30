#ifndef GRADE_H_
#define GRADE_H_

#define PASS_GRADE 55

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
 *   isValidGrade            - Checks if the grade is valid
 *   isValidPoints           - Checks if the points for a course are valid
 *   isValidSemester         - Checks if the semester is valid
 *   getGrade                - Get the grade
 *   getPointsX2             - Get the points times 2
 *   getSemester             - Get the semester
 *   getCourseID             - Get the course ID
 *   compareSemesters        - Compare between two semesters
 *   compareCourses          - Compare between two course ids
 *   copyGrade               - Copys a grade
 *   getTotalPointsX2        - Get the total points for a semester times 2
 *   getFailedPointsX2       - Get the failed points for a semester times 2
 *   getEffectivePointsX2    - Get the effective points for a semester times 2
 *   getEffectiveGradeSumX2  - Get the effective sum for a semester times 2
 *   compareCoursesSemesters - Compare between two courses and two semesters
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
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_INVALID_PARAMETERS
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
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_COURSE_DOES_NOT_EXIST
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
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_COURSE_DOES_NOT_EXIST,
 * MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool updateGrade(CourseManager course_manager, char* course_id, char* grade);

/**
 * Print the whole matriculation
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN
 * true if there was no error
 */
bool reportFull(CourseManager course_manager);

/**
 * Print the clean matriculation
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN
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
 * Checks if the grade is valid
 *
 * @param1 grade the grade that is checked for validity
 * @return
 * true if the grade is valid, false otherwise
 */
bool isValidGrade(char* grade);

/**
 * Checks if the points for a course are valid
 *
 * @param1 points the points that is checked for validity
 * @return
 * true if the points is valid, false otherwise
 */
bool isValidPoints(char* points);

/**
 * Checks if the semester is valid
 *
 * @param1 semester the semester that is checked for validity
 * @return
 * true if the semester is valid, false otherwise
 */
bool isValidSemester(char* semester);

/**
 * Get the grade
 *
 * @param1 grade the grade that contains the "grade"
 * @return
 * if there was an error then -1, otherwise the grade
 */
int getGrade(char* grade);

/**
 * Get the points times 2
 *
 * @param1 points the points that contains the "points"
 * @return
 * if there was an error then -1, otherwise the points
 */
int getPointsX2(char* points);

/**
 * Get the semester
 *
 * @param1 semester the semester that contains the "semester"
 * @return
 * if there was an error then -1, otherwise the semester
 */
int getSemester(char* semester);

/**
 * Get the course ID
 *
 * @param1 course_id the course_id that contains the "course id"
 * @return
 * if there was an error then -1, otherwise the course id
 */
int getCourseID(char* course_id);

/**
 * Compare between two semesters
 *
 * @param1 semester1 the semester we want to compare with semester2
 * @param2 semester2 the semester we want to compare with semester1
 * @return
 * 		A positive integer if the first semester is greater;
 * 		0 if they're equal or if one of the parameters is NULL;
 * 		A negative integer if the second semester is greater.
 */
int compareSemesters(char* semester1, char* semester2);

/**
 * Compare between two course ids
 *
 * @param1 course_id1 the course id we want to compare with course_id2
 * @param2 course_id2 the course id we want to compare with course_id1
 * @return
 * 		A positive integer if the first course id is greater;
 * 		0 if they're equal or if one of the parameters is NULL;
 * 		A negative integer if the second course id is greater.
 */
int compareCourses(char* course_id1, char* course_id2);

/**
 * Copys a grade
 *
 * @param1 grade the grade we want to copy
 * @return
 * NULL if there was an allocation failure or grade = NULL
 * otherwise the grade
 */
Grade copyGrade(Grade grade);

/**
 * Get The number of total course points multiplied by 2, for a semester
 * Courses taken multiple times are counted multiple times
 * if the semester isn't specified then all grades will be taken into account
 *
 * @param1 grades the list of grades that contains all the grades
 * @param2 semester the semester of the grades. if semester is NULL then all
 * the grades will be taken into account
 * @return
 * if there was an error then -1, otherwise the total points times 2
 */
int getTotalPointsX2(List grades, char* semester);

/**
 * Get the number of failed course points, multiplied by 2, for a semester
 * A failed course is a course with a grade lower than PASS_GRADE
 * Courses failed multiple times are counted multiple times
 * if the semester isn't specified then all grades will be taken into account
 *
 * @param1 grades the list of grades that contains all the grades
 * @param2 semester the semester of the grades. if semester is NULL then all
 * the grades will be taken into account
 * @return
 * if there was an error then -1, otherwise the failed points times 2
 */
int getFailedPointsX2(List grades, char* semester);

/**
 * Get the number of total effective course points, multiplied by 2, for a
 * semester. Effective points are points of courses whose grade counts.
 * A grade doesn't count if another grade of the same course id appears later
 * in the semester
 * if the semester isn't specified then all grades will be taken into account
 *
 * @param1 grades the list of grades that contains all the grades
 * @param2 semester the semester of the grades. if semester is NULL then all
 * the grades will be taken into account
 * @return
 * if there was an error then -1, otherwise the effective points times 2
 */
int getEffectivePointsX2(List grades, char* semester);

/**
 * Get the sum of the effective course grades, when
 * each grade is multiplied by the number of course points, multiplied by 2
 * if the semester isn't specified then all grades will be taken into account
 *
 * @param1 grades the list of grades that contains all the grades
 * @param2 semester the semester of the grades. if semester is NULL then all
 * the grades will be taken into account
 * @return
 * if there was an error then -1, otherwise the effective grade sum times 2
 */
int getEffectiveGradeSumX2(List grades, char* semester);

/**
 * Compare between two course ids and semesters
 *
 * @param1 grade1 the grade that contains the course_id/semester to compare
 * @param2 grade2 the grade that contains the course_id/semester to compare
 * @param3 key a pointless parameter so the listSort could work
 * @return
 * 		if the courses are not equal return compareCourses
 * 		else return compareSemesters
 */
int compareCoursesSemesters(Grade grade1, Grade grade2, ListSortKey key);

/**
 * Destroys an instance of grade
 *
 * @param1 grade the grade we destroy
 */
void destroyGrade(Grade grade);

#endif /* GRADE_H_ */
