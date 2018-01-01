#ifndef GRADE_H_
#define GRADE_H_

#define MIN_GRADE 0
#define MAX_GRADE 100
#define PASS_GRADE 55
#define SEMESTER_MIN 1
#define MIN_POINTS_X2 0
#define MIN_SPORT_COURSE_ID 390000
#define MAX_SPORT_COURSE_ID 399999

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
 * copyGrade               - Copies a grade
 * removeGrade             - Removes a grade from the logged in student
 * updateGrade             - Update a grade for the logged in student
 * reportFull              - Print the whole matriculation
 * getTotalPointsX2        - Get the total points for a semester times 2
 * getFailedPointsX2       - Get the failed points for a semester times 2
 * isPassingGrade          - Checks if a grade is a passing grade
 * getEffectivePointsX2    - Get the effective points for a semester times 2
 * getEffectiveGradeSumX2  - Get the effective sum for a semester times 2
 * isEffectiveGrade        - Checks if the grade is effective for a semester
 * isSportCourse           - Checks if the course is a sport course
 * reportClean             - Print the clean matriculation
 * compareCoursesSemesters - Compare between two courses and two semesters
 * isEffectiveCleanGrade   - Checks if the grade is effective for a clean report
 * reportBest              - Print the best grades
 * compareBest             - Compare between two grades/courses/semesters
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
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @patam3 grades the list of grades of the logged in student
 * @param4 semester the semester when the grade was received
 * @param5 course_id the id of the course
 * @param6 points_x2 the points*2 that the course is worth
 * @param7 grade the grade received in the course
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool addGrade(CourseManager course_manager, Student student_in, List grades,
			  int semester, int course_id, int points_x2, int grade);

/**
 * Copies a grade
 *
 * @param1 grade the grade we want to copy
 * @return
 * NULL if there was an allocation failure or grade = NULL
 * otherwise the grade
 */
ListElement copyGrade(ListElement grade);

/**
 * Removes a grade from the logged in student
 *
 * @param1 course_manager the CourseManager that the student is logged in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @patam3 grades the list of grades of the logged in student
 * @param4 semester the semester when the grade was received
 * @param5 course_id the id of the course
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_COURSE_DOES_NOT_EXIST
 * true if there was no error
 */
bool removeGrade(CourseManager course_manager, Student student_in, List grades,
				 int semester, int course_id);

/**
 * Update a grade for the logged in student
 *
 * @param1 course_manager the CourseManager that the student is logged in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @patam3 grades the list of grades of the logged in student
 * @param4 course_id the id of the course
 * @param5 grade the grade received in the course
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_COURSE_DOES_NOT_EXIST,
 * MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool updateGrade(CourseManager course_manager, Student student_in, List grades,
				 int course_id, int grade);

/**
 * Print the whole matriculation
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @patam3 grades the list of grades of the logged in student
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN
 * true if there was no error
 */
bool reportFull(CourseManager course_manager, Student student_in, List grades);

/**
 * Get The number of total course points multiplied by 2, for a semester
 * Courses taken multiple times are counted multiple times
 * if the semester isn't specified then all grades will be taken into account
 *
 * @param1 grades the list of grades that contains all the grades
 * @param2 semester the semester of the grades. if semester is NULL(0) then all
 * the grades will be taken into account
 * @return
 * if there was an error then -1, otherwise the total points times 2
 */
int getTotalPointsX2(List grades, int semester);

/**
 * Get the number of failed course points, multiplied by 2, for a semester
 * A failed course is a course with a grade lower than PASS_GRADE
 * Courses failed multiple times are counted multiple times
 * if the semester isn't specified then all grades will be taken into account
 *
 * @param1 grades the list of grades that contains all the grades
 * @param2 semester the semester of the grades. if semester is NULL(0) then all
 * the grades will be taken into account
 * @return
 * if there was an error then -1, otherwise the failed points times 2
 */
int getFailedPointsX2(List grades, int semester);

/**
 * Checks if a grade is a passing grade
 *
 * @param1 grade the grade that is checked
 * @return
 * true if the grade is a passing grade, false otherwise
 */
bool isPassingGrade(int grade);

/**
 * Get the number of total effective course points, multiplied by 2, for a
 * semester. Effective points are points of courses whose grade counts.
 * A grade doesn't count if another grade of the same course id appears later
 * in the semester
 * if the semester isn't specified then all grades will be taken into account
 *
 * @param1 grades the list of grades that contains all the grades
 * @param2 semester the semester of the grades. if semester is NULL(0) then all
 * the grades will be taken into account
 * @return
 * if there was an error then -1, otherwise the effective points times 2
 */
int getEffectivePointsX2(List grades, int semester);

/**
 * Get the sum of the effective course grades, when
 * each grade is multiplied by the number of course points, multiplied by 2
 * if the semester isn't specified then all grades will be taken into account
 *
 * @param1 grades the list of grades that contains all the grades
 * @param2 semester the semester of the grades. if semester is NULL(0) then all
 * the grades will be taken into account
 * @return
 * if there was an error then -1, otherwise the effective grade sum times 2
 */
int getEffectiveGradeSumX2(List grades, int semester);

/**
 * Checks if the grade is effective for a semester
 *
 * @param1 grades the list of grades that contains all the grades
 * @param2 grade the grade that is checked for effectiveness
 * NOTE: the grade that is sent should be a pointer to a grade in the list of
 * grades
 * @param3 checkSemester if false then the grade is effective if it is effective
 * for all the grades and not just for a certain semester. if true the the grade
 * is effective if it is effective for the semester that it is in
 * @return
 * true if the grade is effective, false otherwise(if the grade sent is not a
 * pointer to one of the grades in the list then false will be returned)
 */
bool isEffectiveGrade(List grades, Grade grade, bool checkSemester);

/**
 * Checks if the course is a sport course
 *
 * @param1 course_id the course that will be checked
 * @return
 * true if the course is a sport course, false otherwise
 */
bool isSportCourse(int course_id);

/**
 * Print the clean matriculation
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @patam3 grades the list of grades of the logged in student
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN
 * true if there was no error
 */
bool reportClean(CourseManager course_manager, Student student_in, List grades);

/**
 * Compare between two course ids and semesters
 *
 * @param1 grade1 the grade that contains the course_id/semester to compare
 * @param2 grade2 the grade that contains the course_id/semester to compare
 * @param3 key the list of grades containing grade1 and grade2
 * @return
 * 		if one of the parameters is NULL return 0
 * 		else if the courses are not equal return compareCourses
 * 		else return compareSemesters
 */
int compareCoursesSemesters(ListElement grade1, ListElement grade2,
							ListSortKey key);

/**
 * Checks if the grade is effective for a clean report
 *
 * @param1 grade the grade that is checked for effectiveness
 * NOTE: the grade that is sent should be a pointer to a grade in the list of
 * grades
 * @param2 grades the list of grades that contains both the grade
 * @return
 * true if the grade is effective, false otherwise(if the grade sent is not a
 * pointer to one of the grades in the list then false will be returned)
 */
bool isEffectiveCleanGrade(ListElement grade, ListFilterKey grades);

/**
 * Print the best grades
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @patam3 grades the list of grades of the logged in student
 * @param4 amount the amount of grades to print
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool reportBest(CourseManager course_manager, Student student_in, List grades,
				int amount);

/**
 * Compare between two grades then two semesters then two courses
 *
 * @param1 grade1 the grade that contains the course_id/semester to compare
 * @param2 grade2 the grade that contains the course_id/semester to compare
 * @param3 key the list of grades containing grade1 and grade2
 * @return
 * 		if one of the parameters is NULL return 0
 * 		then compare grades
 * 		if the grades are equal compare by semesters
 * 		if the semesters are equal compare by courses
 */
int compareBest(ListElement grade1, ListElement grade2, ListSortKey key);

/**
 * Destroys an instance of grade
 *
 * @param1 grade the grade we destroy
 */
void destroyGrade(ListElement grade);

#endif /* GRADE_H_ */
