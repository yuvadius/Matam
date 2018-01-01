#include <stdlib.h>
#include <assert.h>
#include "set.h"
#include "list.h"
#include "mtm_ex3.h"
#include "course_manager.h"
#include "student.h"
#include "grade.h"

struct grade_t {
	int grade;
	int course_id;
	int points_x2;
	int semester;
};

/**
 * Creates an instance of a grade
 *
 * @param1 semester the semester when the grade was received
 * @param2 course_id the id of the course
 * @param3 points_x2 the points*2 that the course is worth
 * @param4 grade the grade received in the course
 * @return
 * The grade created. if there was an allocation error return NULL or if
 * one of the parameters isn't valid
 */
Grade createGrade(int semester, int course_id, int points_x2, int grade) {
	//create an instance of a grade
	Grade grade_instance = malloc(sizeof(*grade_instance));
	if(grade_instance == NULL) { //if there was a memory failure return NULL
		return NULL;
	}
	if(isValidSemester(semester) && isValidCourseID(semester) &&
	   isValidPointsX2(points_x2) && isValidGrade(grade)) {
		grade_instance->semester = semester;
		grade_instance->course_id = course_id;
		grade_instance->points_x2 = points_x2;
		grade_instance->grade = grade;
		return grade_instance;
	}
	else {
		destroyGrade(grade_instance);
		return NULL;
	}
}

/**
 * Checks if the semester is valid
 *
 * @param1 semester the semester that is checked for validity
 * @return
 * true if the semester is valid, false otherwise
 */
bool isValidSemester(int semester) {
	if(semester >= SEMESTER_MIN) {
		return true;
	}
	else {
		return false;
	}
}

/**
 * Checks if the points*2 for a course is valid
 *
 * @param1 points_x2 the points*2 that is checked for validity
 * @return
 * true if the points is valid, false otherwise
 */
bool isValidPointsX2(int points_x2) {
	if(points_x2 >= MIN_POINTS_X2) {
		return true;
	}
	else {
		return false;
	}
}

/**
 * Checks if the grade is valid
 *
 * @param1 grade the grade that is checked for validity
 * @return
 * true if the grade is valid, false otherwise
 */
bool isValidGrade(int grade) {
	if(grade >= MIN_GRADE && grade <= MAX_GRADE) {
		return true;
	}
	else {
		return false;
	}
}

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
bool addGrade(CourseManager course_manager, Student student_in, int semester,
			  int course_id, int points_x2, int grade) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	List g = student_in->grades;
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		course_manager->error = MTM_NOT_LOGGED_IN;
		return false;
	}
	//check if one of the paramaters isn't valid
	if(!isValidSemester(semester) || !isValidCourseID(course_id) ||
	   !isValidPoints(points) || !isValidGrade(grade)) {
		course_manager->error = MTM_INVALID_PARAMETERS;
		return false;
	}
	//create an instance of grade
	Grade grade = createGrade(semester, course_id, points, grade);
	if(grade == NULL) { //if there was an allocation failure
		course_manager->error = MTM_OUT_OF_MEMORY;
		return false;
	}
	//insert the grade in the list in its appropriate position according to date
	LIST_FOREACH(Grade, list_grade, student_in->grades) {
		//if the semester is newer or equal to the current semester in the list
		if(compareSemesters(semester, list_grade->semester) >= 0) {
			ListResult result = listInsertBeforeCurrent(
								course_manager->current_student->grades, grade);
			destroyGrade(grade);
			if(result == LIST_OUT_OF_MEMORY) {//if the was an allocation failure
				course_manager->error = MTM_OUT_OF_MEMORY;
				return false;
			}
			else {
				return true;
			}
		}
	}
	//if there is no grade older then the received grade insert as last in list
	ListResult result = listInsertLast(course_manager->current_student->grades,
									   grade);
	destroyGrade(grade);
	if(result == LIST_OUT_OF_MEMORY) { //if the was an allocation failure
		course_manager->error = MTM_OUT_OF_MEMORY;
		return false;
	}
	else {
		return true;
	}
}

/**
 * Destroys an instance of grade
 *
 * @param1 grade the grade we destroy
 */
void destroyGrade(Grade grade) {
	free(grade);
}
