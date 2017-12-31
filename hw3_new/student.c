#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "set.h"
#include "list.h"
#include "mtm_ex3.h"
#include "course_manager.h"
#include "student.h"

struct student_t {
	Set friends;
	Set requests;
	int id;
	char* first_name;
	char* last_name;
	List grades; //will be sorted from newest to oldest
};

/**
 * Creates a student
 *
 * @param1 student_id the students ID
 * @param2 first_name the first name of the student
 * @param3 last_name the last name of the student
 * @return
 * The student created. if there was an allocation error return NULL
 * or if the id isn't valid return NULL
 */
Student createStudent(int student_id, char* first_name, char* last_name) {
	//create an instance of a student
	Student student = malloc(sizeof(*student));
	if(student == NULL) { //if there was a memory failure return NULL
		return NULL;
	}
	student->friends = setCreate(copyStudent, destroyStudent, compareStudents);
	if(student->friends == NULL) { //if memory allocation failed
		destroyStudent(student);
		return NULL;
	}
	student->requests = setCreate(copyStudent, destroyStudent, compareStudents);
	if(student->requests == NULL) { //if memory allocation failed
		destroyStudent(student);
		return NULL;
	}
	if(isValidStudentID(student_id)) {
		student->id = student_id;
	}
	else { //invalid student id
		return NULL;
	}
	student->first_name = malloc(strlen(first_name) + 1);
	if(student->first_name == NULL) { //if memory allocation failed
		destroyStudent(student);
		return NULL;
	}
	strcpy(student->first_name, first_name);
	student->last_name = malloc(strlen(last_name) + 1);
	if(student->last_name == NULL) { //if memory allocation failed
		destroyStudent(student);
		return NULL;
	}
	strcpy(student->last_name, last_name);
	/*student->grades = listCreate(copyGrade, destroyGrade);
	if(student->grades == NULL) { //if memory allocation failed
		destroyStudent(student);
		return NULL;
	}*/
	return student;
}

/**
 * Copys a student
 *
 * @param1 student the student we want to copy
 * @return
 * NULL if there was an allocation failure or student = NULL
 * otherwise the student
 */
SetElement copyStudent(SetElement student) {
	//can't do anything if student isn't set
	if(student == NULL) {
		return NULL;
	}
	return createStudent(((Student)student)->id, ((Student)student)->first_name,
						 ((Student)student)->last_name);
}

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
int compareStudents(SetElement student1, SetElement student2) {
	if(student1 == NULL || student2 == NULL) {
		return 0;
	}
	else {
		return (((Student)student1)->id - ((Student)student2)->id);
	}
}

/**
 * Get a student with a certain id from the system
 *
 * @param1 student the student to retrieve from the system
 * @param2 student_id the id of the student
 * @return
 * the student if he exists, NULL otherwise
 */
Student getStudent(CourseManager course_manager, int student_id) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return NULL;
	}
	//find the student in the set
	SET_FOREACH(Student, student, getStudents(course_manager)) {
		//if the student was found
		if(student->id == student_id) {
			return student;
		}
	}
	//student wasn't found
	return NULL;
}

/**
 * Checks if a student id is valid
 *
 * @param1 student_id the id of the student that is checked
 * @return
 * true if the id is valid, false otherwise
 */
bool isValidStudentID(int student_id) {
	if(student_id >= STUDENT_ID_MIN && student_id <= STUDENT_ID_MAX) {
		return true;
	}
	else {
		return false;
	}
}

/**
 * Get the students id
 *
 * @param1 student the student which contains the id
 * @return
 * the id if valid, 0 otherwise
 */
int getStudentID(Student student) {
	//can't do anything if course_manager isn't set
	if(student == NULL) {
		return 0;
	}
	else {
		return student->id;
	}
}

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
bool friendRequest(CourseManager course_manager, int id) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if((course_manager->current_student == NULL) {
		course_manager->error = MTM_NOT_LOGGED_IN;
		return false;
	}
	//find the student in the set
	Student student = getStudent(course_manager, id);
	//if the student wasn't found
	if(student == NULL) {
		course_manager->error = MTM_STUDENT_DOES_NOT_EXIST;
		return false;
	}
	//if the students are already friends
	else if(setIsIn(course_manager->current_student->friends, student)) {
		course_manager->error = MTM_ALREADY_FRIEND;
		return false;
	}
	//if a request was already sent
	else if(setIsIn(course_manager->current_student->requests, student)) {
		course_manager->error = MTM_ALREADY_REQUESTED;
		return false;
	}
	else {
		//add the friend request to the set of requests
		SetResult result = setAdd(course_manager->current_student->requests,
								  student);
		if(result == SET_OUT_OF_MEMORY) {
			course_manager->error = MTM_OUT_OF_MEMORY;
			return false;
		}
		else {
			return true;
		}
	}
}

/**
 * Destroys an instance of student
 *
 * @param1 student the student we destroy
 */
void destroyStudent(SetElement student) {
	//nothing to do if student isn't set
	if(student == NULL) {
		return;
	}
	free(((Student)student)->first_name);
	free(((Student)student)->last_name);
	listDestroy(((Student)student)->grades);
	setDestroy(((Student)student)->requests);
	setDestroy(((Student)student)->friends);
	free(student);
}
