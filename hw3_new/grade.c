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
 * deep copies a list into a newly created list in reversed order
 *
 * @param1 course_manager the course_manager contains the error output
 * @param2 list the list that will be copied and then reversed
 * @return
 * The list created. if there was an allocation error return NULL
 */
static List listCopyReversed(CourseManager course_manager, List list) {
	//nothing to do if the course manager or the list aren't set
    if(course_manager == NULL || list == NULL) {
        return NULL;
    }
    List new_list = listCreate(copyGrade, destroyGrade);
    if(new_list == NULL) { //if there was a memory failure
    	setError(course_manager, MTM_OUT_OF_MEMORY);
    	return NULL;
    }
    LIST_FOREACH(ListElement, element, list) {
    	ListResult result = listInsertFirst(new_list, element);
    	if(result == LIST_OUT_OF_MEMORY) {
    		listDestroy(new_list);
    		setError(course_manager, MTM_OUT_OF_MEMORY);
    		return NULL;
    	}
    }
    return new_list;
}

/**
 * deep copies a set into a newly created list
 *
 * @param1 course_manager the course_manager contains the error output
 * @param2 set the set that will be copied
 * @return
 * The list created. if there was an allocation error return NULL or if one
 * of the parameters was NULL then return NULL
 */
static List setToList(CourseManager course_manager, Set set) {
	if(set == NULL) {
		return NULL;
	}
	List list = listCreate(copyGrade, destroyGrade);
	if(list == NULL) { //if there was a memory failure
		setError(course_manager, MTM_OUT_OF_MEMORY);
		return NULL;
	}
	SET_FOREACH(SetElement, element, set) {
		ListResult result = listInsertFirst(list, element);
		if(result == LIST_OUT_OF_MEMORY) {
			listDestroy(list);
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return NULL;
		}
	}
	return list;
}

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
			  int semester, int course_id, int points_x2, int grade){
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	//check if one of the parameters isn't valid
	if(!isValidSemester(semester) || !isValidCourseID(course_id) ||
	   !isValidPointsX2(points_x2) || !isValidGrade(grade)) {
		setError(course_manager, MTM_INVALID_PARAMETERS);
		return false;
	}
	//create an instance of grade
	Grade grade_instance = createGrade(semester, course_id, points_x2, grade);
	if(grade_instance == NULL) { //if there was an allocation failure
		setError(course_manager, MTM_OUT_OF_MEMORY);
		return false;
	}
	//insert the grade in the list in its appropriate position according to date
	LIST_FOREACH(Grade, list_grade, grades) {
		//if the semester is newer or equal to the current semester in the list
		if((semester - list_grade->semester) >= 0) {
			ListResult result = listInsertBeforeCurrent(grades, grade_instance);
			destroyGrade(grade_instance); //destroy because a copy was stored
			if(result == LIST_OUT_OF_MEMORY) {//if the was an allocation failure
				setError(course_manager, MTM_OUT_OF_MEMORY);
				return false;
			}
			else {
				return true;
			}
		}
	}
	//if there is no grade older then the received grade insert as last in list
	ListResult result = listInsertLast(grades, grade_instance);
	destroyGrade(grade_instance); //destroy because a copy was stored in grades
	if(result == LIST_OUT_OF_MEMORY) { //if the was an allocation failure
		setError(course_manager, MTM_OUT_OF_MEMORY);
		return false;
	}
	else {
		return true;
	}
}

/**
 * Copies a grade
 *
 * @param1 grade the grade we want to copy
 * @return
 * NULL if there was an allocation failure or grade = NULL
 * otherwise the grade
 */
ListElement copyGrade(ListElement grade) {
	if(grade == NULL) { //nothing to copy if grade isn't set
		return grade;
	}
	return createGrade(((Grade)grade)->semester, ((Grade)grade)->course_id,
					   ((Grade)grade)->points_x2, ((Grade)grade)->grade);
}

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
				 int semester, int course_id) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	//insert the grade in the list in its appropriate position according to date
	LIST_FOREACH(Grade, grade, grades) {
		//if the semester and course are equal to current semester in the list
		if(semester == grade->semester && course_id == grade->course_id) {
		   	//remove the newest grade in the list that has same semester/course
			listRemoveCurrent(grades);
			return true;
		}
	}
	//no grade was found
	setError(course_manager, MTM_COURSE_DOES_NOT_EXIST);
	return false;
}

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
				 int course_id, int grade) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	//update the newest grade of the course in the student's list of grades
	LIST_FOREACH(Grade, list_grade, grades) {
		//if the course_id is equal to the current course id in the list
		if(course_id == list_grade->course_id) {
			//check if the grade is valid
			if(isValidGrade(grade) == false) {
				setError(course_manager, MTM_INVALID_PARAMETERS);
				return false;
			}
			list_grade->grade = grade;
			return true;
		}
	}
	//no grade was found
	setError(course_manager, MTM_COURSE_DOES_NOT_EXIST);
	return false;
}

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
bool reportFull(CourseManager course_manager, Student student_in, List grades) {
	//can't do anything if course_manager or output_channel or grades aren't set
	if(course_manager == NULL || getOutputChannel(course_manager) == NULL ||
	   grades == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	mtmPrintStudentInfo(getOutputChannel(course_manager),
						getStudentID(student_in),
						getStudentFirstName(student_in),
						getStudentLastName(student_in));
	//if the list is empty then there is nothing to print
	if(listGetSize(grades) > 0) {
		//get the grades from smallest semester num to biggest semester num
		List reversed_grades = listCopyReversed(course_manager, grades);
		if(reversed_grades == NULL) { //if there was an allocation failure
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		//get the first element(NOT A COPY)
		Grade temp_grade = listGetFirst(reversed_grades);
		int semester = temp_grade->semester; //get the first semester
		LIST_FOREACH(Grade, grade, reversed_grades) { //loop over all the grades
			//if all grades of a semester have been printed print SemesterInfo
			if(semester != grade->semester) {
				mtmPrintSemesterInfo(getOutputChannel(course_manager), semester,
									 getTotalPointsX2(grades, semester),
									 getFailedPointsX2(grades, semester),
									 getEffectivePointsX2(grades, semester),
									 getEffectiveGradeSumX2(grades, semester));
				semester = grade->semester;
			}
			mtmPrintGradeInfo(getOutputChannel(course_manager),grade->course_id,
							  grade->points_x2, grade->grade);
		}
		//print the last semester
		mtmPrintSemesterInfo(getOutputChannel(course_manager), semester,
							 getTotalPointsX2(grades, semester),
							 getFailedPointsX2(grades, semester),
							 getEffectivePointsX2(grades, semester),
							 getEffectiveGradeSumX2(grades, semester));
		listDestroy(reversed_grades); //destroy instance of reversed_grades
	}
	//print the grade summary
	mtmPrintSummary(getOutputChannel(course_manager),
					getTotalPointsX2(grades, 0),
					getFailedPointsX2(grades, 0),
					getEffectivePointsX2(grades, 0),
					getEffectiveGradeSumX2(grades, 0));
	return true;
}
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
int getTotalPointsX2(List grades, int semester) {
	//if the grades isn't set or semester isn't valid there is nothing to check
	if(grades == NULL || (!isValidSemester(semester) && semester != 0)) {
		return -1;
	}
	else {
		int total_points_x2 = 0;
		//copy list so the iterator wouldn't change while looping
		List grades_copy = listCopy(grades);
		LIST_FOREACH(Grade, grade, grades_copy) { //loop over all the grades
			if(grade->semester == semester || semester == 0) {
				total_points_x2 += grade->points_x2;
			}
		}
		listDestroy(grades_copy);
		return total_points_x2;
	}
}

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
int getFailedPointsX2(List grades, int semester) {
	//if the grades isn't set or semester isn't valid there is nothing to check
	if(grades == NULL || (!isValidSemester(semester) && semester != 0)) {
		return -1;
	}
	else {
		int failed_points_x2 = 0;
		//copy list so the iterator wouldn't change while looping
		List grades_copy = listCopy(grades);
		LIST_FOREACH(Grade, grade, grades_copy) { //loop over all the grades
			if(grade->semester == semester || semester == 0) {
				if(isPassingGrade(grade->grade) == false) {
					failed_points_x2 += grade->points_x2;
				}
			}
		}
		listDestroy(grades_copy);
		return failed_points_x2;
	}
}

/**
 * Checks if a grade is a passing grade
 *
 * @param1 grade the grade that is checked
 * @return
 * true if the grade is a passing grade, false otherwise
 */
bool isPassingGrade(int grade) {
	if(grade >= PASS_GRADE && grade <= MAX_GRADE) {
		return true;
	}
	else {
		return false;
	}
}

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
int getEffectivePointsX2(List grades, int semester) {
	//if the grades isn't set or semester isn't valid there is nothing to check
	if(grades == NULL || (!isValidSemester(semester) && semester != 0)) {
		return -1;
	}
	else {
		//copy list so the iterator wouldn't change while looping
		List grades_copy = listCopy(grades);
		int effective_grade_points_x2 = 0;
		LIST_FOREACH(Grade, grade, grades_copy) { //loop over all the grades
			if(grade->semester == semester || semester == 0) {
				if(isEffectiveGrade(grades, grade, (semester != 0))) {
					effective_grade_points_x2 += grade->points_x2;
				}
			}
		}
		listDestroy(grades_copy);
		return effective_grade_points_x2;
	}
}

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
int getEffectiveGradeSumX2(List grades, int semester) {
	//if the grades isn't set or semester isn't valid there is nothing to check
	if(grades == NULL || (!isValidSemester(semester) && semester != 0)) {
		return -1;
	}
	else {
		//copy list so the iterator wouldn't change while looping
		List grades_copy = listCopy(grades);
		int effective_grade_sum_x2 = 0;
		LIST_FOREACH(Grade, grade, grades_copy) { //loop over all the grades
			if(grade->semester == semester || semester == 0) {
				if(isEffectiveGrade(grades, grade, (semester != 0))) {
					effective_grade_sum_x2 += grade->grade;
				}
			}
		}
		listDestroy(grades_copy);
		return effective_grade_sum_x2;
	}
}

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
bool isEffectiveGrade(List grades, Grade grade, bool checkSemester) {
	//if the grades isn't set or semester isn't valid there is nothing to check
	if(grades == NULL || grade == NULL) {
		return false;
	}
	//copy list so the iterator wouldn't change while looping
	List grades_copy = listCopy(grades);
	//if the grade will be checked for effectiveness in a semester
	//sport grades are always checked for effectiveness in a semester
	if(checkSemester == true || isSportCourse(grade->course_id)) {
		LIST_FOREACH(Grade, list_grade, grades_copy) { //loop over all the grades
			if(list_grade->semester == grade->semester &&
			   list_grade->course_id == grade->course_id) {
				listDestroy(grades_copy);
				//if this effective grade was the grade sent return true
				if(list_grade == grade) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		return false; //no grade was found
	}
	else {
		LIST_FOREACH(Grade, list_grade, grades_copy) { //loop over all the grades
			if(list_grade->course_id == grade->course_id) {
				listDestroy(grades_copy);
				//if this effective grade was the grade sent return true
				if(list_grade == grade) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		return false; //no grade was found
	}
}

/**
 * Checks if the course is a sport course
 *
 * @param1 course_id the course that will be checked
 * @return
 * true if the course is a sport course, false otherwise
 */
bool isSportCourse(int course_id) {
	if(course_id >= MIN_SPORT_COURSE_ID && course_id <= MAX_SPORT_COURSE_ID) {
		return true;
	}
	else {
		return false;
	}
}

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
bool reportClean(CourseManager course_manager, Student student_in, List grades){
	//can't do anything if course_manager or output_channel or grades aren't set
	if(course_manager == NULL || getOutputChannel(course_manager) == NULL ||
	   grades == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	mtmPrintStudentInfo(getOutputChannel(course_manager),
						getStudentID(student_in),
						getStudentFirstName(student_in),
						getStudentLastName(student_in));
	//if the list is empty then there is nothing to print
	if(listGetSize(grades) > 0) {
		//Remove all non effective grades for the clean report
		List filter_grades = listFilter(grades, isEffectiveCleanGrade, grades);
		if(filter_grades == NULL) { //if there was an allocation failure
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		//sort filter_grades by courses/semesters
		ListResult result = listSort(filter_grades, compareCoursesSemesters,
									 filter_grades);
		if(result == LIST_OUT_OF_MEMORY) {
			listDestroy(filter_grades);
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		LIST_FOREACH(Grade, grade, filter_grades) { //loop over all the grades
			mtmPrintGradeInfo(getOutputChannel(course_manager),grade->course_id,
							  grade->points_x2, grade->grade);
		}
		listDestroy(filter_grades);
	}
	//print the clean grade summary
	mtmPrintCleanSummary(getOutputChannel(course_manager),
						 getEffectivePointsX2(grades, 0),
						 getEffectiveGradeSumX2(grades, 0));
	return true;
}

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
							ListSortKey key) {
	if(grade1 == NULL || grade2 == NULL || key == NULL) {
		return 0;
	}
	else if(((Grade)grade1)->course_id != ((Grade)grade2)->course_id) {
		return (((Grade)grade1)->course_id - ((Grade)grade2)->course_id);
	}
	else {
		return (((Grade)grade1)->semester - ((Grade)grade2)->semester);
	}
}

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
bool isEffectiveCleanGrade(ListElement grade, ListFilterKey grades) {
	if(grade == NULL || grades == NULL) {
		return false;
	}
	else {
		return isEffectiveGrade((List)grades, (Grade)grade, false);
	}
}

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
				int amount) {
	//can't do anything if course_manager or output_channel or grades aren't set
	if(course_manager == NULL || getOutputChannel(course_manager) == NULL ||
	   grades == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	if(amount <= 0) {
		setError(course_manager, MTM_INVALID_PARAMETERS);
		return false;
	}
	//if the list is empty then there is nothing to print
	if(listGetSize(grades) > 0) {
		//Remove all non effective grades for the clean report
		List filter_grades = listFilter(grades, isEffectiveCleanGrade, grades);
		if(filter_grades == NULL) { //if there was an allocation failure
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		//sort filter_grades by courses/semesters
		ListResult result = listSort(filter_grades, compareBest, filter_grades);
		if(result == LIST_OUT_OF_MEMORY) {
			listDestroy(filter_grades);
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		LIST_FOREACH(Grade, grade, filter_grades) { //loop over all the grades
			if(amount > 0) { //don't print more then "amount"
				mtmPrintGradeInfo(getOutputChannel(course_manager),
							grade->course_id, grade->points_x2, grade->grade);
				--amount;
			}
		}
		listDestroy(filter_grades);
	}
	return true;
}

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
int compareBest(ListElement grade1, ListElement grade2, ListSortKey key) {
	if(grade1 == NULL || grade2 == NULL || key == NULL) {
		return 0;
	}
	else if(((Grade)grade1)->grade != ((Grade)grade2)->grade) {
		return (((Grade)grade2)->grade - ((Grade)grade1)->grade);
	}
	else if(((Grade)grade1)->semester != ((Grade)grade2)->semester){
		return (((Grade)grade1)->semester - ((Grade)grade2)->semester);
	}
	else {
		return (((Grade)grade1)->course_id - ((Grade)grade2)->course_id);
	}
}

/**
 * Prints the worst grades
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
bool reportWorst(CourseManager course_manager, Student student_in, List grades,
				 int amount) {
	//can't do anything if course_manager or output_channel or grades aren't set
	if(course_manager == NULL || getOutputChannel(course_manager) == NULL ||
	   grades == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	if(amount <= 0) {
		setError(course_manager, MTM_INVALID_PARAMETERS);
		return false;
	}
	//if the list is empty then there is nothing to print
	if(listGetSize(grades) > 0) {
		//Remove all non effective grades for the clean report
		List filter_grades = listFilter(grades, isEffectiveCleanGrade, grades);
		if(filter_grades == NULL) { //if there was an allocation failure
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		//sort filter_grades by courses/semesters
		ListResult result = listSort(filter_grades, compareWorst, filter_grades);
		if(result == LIST_OUT_OF_MEMORY) {
			listDestroy(filter_grades);
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		LIST_FOREACH(Grade, grade, filter_grades) { //loop over all the grades
			if(amount > 0) { //don't print more then "amount"
				mtmPrintGradeInfo(getOutputChannel(course_manager),
							grade->course_id, grade->points_x2, grade->grade);
				--amount;
			}
		}
		listDestroy(filter_grades);
	}
	return true;
}

/**
 * Compare between two grades then two semesters then two courses
 * This function is used as a filter for reportWorst
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
int compareWorst(ListElement grade1, ListElement grade2, ListSortKey key) {
	if(grade1 == NULL || grade2 == NULL || key == NULL) {
		return 0;
	}
	else if(((Grade)grade1)->grade != ((Grade)grade2)->grade) {
		return (((Grade)grade1)->grade - ((Grade)grade2)->grade);
	}
	else if(((Grade)grade1)->semester != ((Grade)grade2)->semester){
		return (((Grade)grade1)->semester - ((Grade)grade2)->semester);
	}
	else {
		return (((Grade)grade1)->course_id - ((Grade)grade2)->course_id);
	}
}

/**
 * Print best students for referencing for a course
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @patam3 grades the list of grades of the logged in student
 * @param4 course_id the course that we want to find a reference for
 * @param5 amount the amount of grades to print
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool reportReference(CourseManager course_manager, Student student_in,
					 Set friends, int course_id, int amount) {
	//can't do anything if course_manager or output_channel or friends isn't set
	if(course_manager == NULL || getOutputChannel(course_manager) == NULL ||
	   friends == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	if(amount <= 0) {
		setError(course_manager, MTM_INVALID_PARAMETERS);
		return false;
	}
	List friends_list = setToList(course_manager, friends);
	if(friends_list == NULL) {
		setError(course_manager, MTM_OUT_OF_MEMORY);
		return false;
	}
	//if the list is empty then there is nothing to print
	if(listGetSize(friends_list) > 0) {
		//Remove all non effective grades for the clean report
		List filter_grades = listFilter(friends_list, didStudentTakeCourse,
										friends_list);
		if(filter_grades == NULL) { //if there was an allocation failure
			listDestroy(friends_list);
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		//sort filter_grades by courses/semesters
		ListResult result = listSort(filter_grades, compareBestCourseGrades,
									 &course_id);
		if(result == LIST_OUT_OF_MEMORY) {
			listDestroy(filter_grades);
			listDestroy(friends_list);
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		LIST_FOREACH(Student, student, filter_grades) { //loop over all the grades
			if(amount > 0) { //don't print more then "amount"
				mtmPrintStudentName(getOutputChannel(course_manager),
					getStudentFirstName(student), getStudentLastName(student));
				--amount;
			}
		}
		listDestroy(filter_grades);
		listDestroy(friends_list);
	}
	return true;
}

/**
 * Compare best grade of a course between two students
 * This function is used as a sort function for reportReference
 * If the student doesn't have a grade in a course his grade will be -1
 *
 * @param1 student1 the student that will be compared
 * @param2 student2 the student that will be compared
 * @param3 course_id the course that will be used. this will be a pointer to int
 * @return
 * 		student1's best grade in the course minus student2's best grade in
 * 		the course
 */
int compareBestCourseGrades(ListElement student1, ListElement student2,
							ListSortKey course_id) {
	//copy list so the iterator wouldn't change while looping
	List grades_copy = listCopy(getStudentGrades((Student)student1));
	int student1_grade = -1, student2_grade = -1;
	LIST_FOREACH(Grade, grade, grades_copy) {
		if(grade->course_id == (*(int*)course_id)) { //if course was found
			if(grade->grade > student1_grade) { //if the grade is higher
				student1_grade = grade->grade;
			}
		}
	}
	listDestroy(grades_copy);
	//copy list so the iterator wouldn't change while looping
	grades_copy = listCopy(getStudentGrades((Student)student2));
	LIST_FOREACH(Grade, grade, grades_copy) {
		if(grade->course_id == (*(int*)course_id)) { //if course was found
			if(grade->grade > student2_grade) { //if the grade is higher
				student2_grade = grade->grade;
			}
		}
	}
	listDestroy(grades_copy);
	return (student2_grade - student1_grade);
}

/**
 * Checks if a student took a course
 *
 * @param1 student the student that is checked for taking the course
 * @param2 course_id the course that will be used. this will be a pointer to int
 * NOTE: the grade that is sent should be a pointer to a grade in the list of
 * grades
 * @return
 * true if the student took the course, false if not or if student or
 * course_id is NULL
 */
bool didStudentTakeCourse(ListElement student, ListFilterKey course_id) {
	if(student == NULL || ((int*)course_id) == NULL) {
		return false;
	}
	else {
		//copy list so the iterator wouldn't change while looping
		List grades_copy = listCopy(getStudentGrades((Student)student));
		LIST_FOREACH(Grade, grade, grades_copy) {
			if(grade->course_id == (*(int*)course_id)) { //if course was found
				listDestroy(grades_copy);
				return true;
			}
		}
		listDestroy(grades_copy);
		return false; //course was not found
	}
}

/**
 * Gets the course id of a certain grade
 *
 * @param1 grade the grade that contains the course id
 * @return
 * the course id on success, 0 on failure
 */
int getCourseID(Grade grade) {
	if(grade == NULL) {
		return 0;
	}
	return grade->course_id;
}

/**
 * Gets the grade of a certain grade object
 *
 * @param1 grade the grade object that contains the grade
 * @return
 * the grade on success, 0 on failure
 */
int getGrade(Grade grade) {
	if(grade == NULL) {
		return 0;
	}
	else {
		return grade->grade;
	}
}

/**
 * Destroys an instance of grade
 *
 * @param1 grade the grade we destroy
 */
void destroyGrade(ListElement grade) {
	free((Grade)grade);
}
