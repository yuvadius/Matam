#include <stdlib.h>
#include <assert.h>
#include "set.h"
#include "student.h"

struct grade_t {
	char* grade;
	char* course_id;
	char* points;
	char* semester;
};

/**
 * deep copies a list into a newly created list in reversed order
 *
 * @param1 course_manager the course_manager contains the error output
 * @param2 list the list that will be copied and then reversed
 * @return
 * The list created. if there was an allocation error return NULL
 */
static List listCopyReversed(CourseManager course_manager, List list) {
    if(course_manager == NULL || list == NULL) {
        return NULL; //return an empty list
    }
    List new_list = listCreate(list->CopyListElement, list->FreeListElement);
    if(new_list == NULL) { //if there was a memory failure
    	course_manager->error = MTM_OUT_OF_MEMORY;
    	return NULL;
    }
    LIST_FOREACH(ListElement, element, list) {
    	ListResult result = listInsertFirst(new_list, element);
    	if(result == LIST_OUT_OF_MEMORY) {
    		listDestroy(new_list);
    		course_manager->error = MTM_OUT_OF_MEMORY;
    		return NULL;
    	}
    }
    return new_list;
}

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
Grade createGrade(char* semester, char* course_id, char* points, char* grade) {
	//create an instance of a grade
	Grade grade = malloc(sizeof(*grade));
	if(grade == NULL) { //if there was a memory failure return NULL
		return NULL;
	}
	grade->semester = semester;
	grade->course_id = course_id;
	grade->points = points;
	grade->grade = grade;
	return grade;
}

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
			  char* points, char* grade) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//check if one of the paramaters isn't valid
	if(!isValidSemester(semester) || !isValidCourseID(course_id) ||
	   !isValidPoints(points) || !isValidGrade(grade)) {
		course_manager->error = MTM_INVALID_PARAMETERS;
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(course_manager->current_student == NULL) {
		course_manager->error = MTM_NOT_LOGGED_IN;
		return false;
	}
	//create an instance of grade
	Grade grade = createGrade(semester, course_id, points, grade);
	if(grade == NULL) { //if there was an allocation failure
		course_manager->error = MTM_OUT_OF_MEMORY;
		return false;
	}
	//insert the grade in the list in its appropriate position according to date
	LIST_FOREACH(Grade, list_grade, course_manager->current_student->grades) {
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
bool removeGrade(CourseManager course_manager, char* semester, char* course_id){
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(course_manager->current_student == NULL) {
		course_manager->error = MTM_NOT_LOGGED_IN;
		return false;
	}
	//insert the grade in the list in its appropriate position according to date
	LIST_FOREACH(Grade, grade, course_manager->current_student->grades) {
		//if the semester and course are equal to current semester in the list 
		if(compareSemesters(semester, list_grade->semester) == 0 &&
		   compareCourses(course_id, grade->course_id) == 0) {
		   	//remove the newest grade in the list that has same semester/course
			ListResult result = listRemoveCurrent(
								course_manager->current_student->grades);
			return true;
		}
	}
	//no grade was found
	course_manager->error = MTM_COURSE_DOES_NOT_EXIST;
	return false;
}

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
bool updateGrade(CourseManager course_manager, char* course_id, char* grade) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//check if the grade is valid
	if(isValidGrade(grade) == false) {
		course_manager->error = MTM_INVALID_PARAMETERS;
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(course_manager->current_student == NULL) {
		course_manager->error = MTM_NOT_LOGGED_IN;
		return false;
	}
	//update the newest grade of the course in the student's list of grades
	LIST_FOREACH(Grade, grade, course_manager->current_student->grades) {
		//if the course_id is equal to the current course id in the list 
		if(compareCourses(course_id, grade->course_id) == 0) {
			grade->course_id = course_id;
			grade->grade = grade;
			return true;
		}
	}
	//no grade was found
	course_manager->error = MTM_COURSE_DOES_NOT_EXIST;
	return false;
}

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
bool reportFull(CourseManager course_manager) {
	//can't do anything if course_manager or output_channel isn't set
	if(course_manager == NULL || course_manager->output_channel == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(course_manager->current_student == NULL) {
		course_manager->error = MTM_NOT_LOGGED_IN;
		return false;
	}
	mtmPrintStudentInfo(course_manager->output_channel,
						getStudentID(course_manager->current_student),
						course_manager->current_student->first_name,
						course_manager->current_student->last_name);
	List reversed_grades = listCopyReversed(course_manager, 
									course_manager->current_student->grades);
	if(reversed_grades == NULL) { //if there was an allocation failure
		course_manager->error = MTM_OUT_OF_MEMORY;
		return false;
	}
	//get the first element(NOT A COPY)
	Grade temp_grade = listGetFirst(course_manager->current_student->grades);
	char* semester = temp_grade->semester;

	//update the newest grade of the course in the student's list of grades
	LIST_FOREACH(Grade, grade, course_manager->current_student->grades) {
		mtmPrintGradeInfo(course_manager->output_channel, 
						  getCourseID(grade->course_id), 
						  getPointsX2(grade->points), getGrade(grade->grade));
		//if the course_id is equal to the current course id in the list 
		if(compareSemesters(semester, grade->semester) != 0) {
			mtmPrintSemesterInfo(course_manager->output_channel, 
								 getSemester(semester), )
			semester = grade->semester;
		}
	}
}