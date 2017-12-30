#include <stdlib.h>
#include <assert.h>
#include "set.h"
#include "student.h"

#define STUDENT_ID_MIN 1
#define STUDENT_ID_MAX 999999999

struct student_t {
	Set friends;
	Set requests;
	char* id;
	char* first_name;
	char* last_name;
	List grades; //will be sorted from newest to oldest
};

static Action getAction(char* action) {
	if(strcmp(action, "accept") == 0) {
		return ACCEPT;
	}
	else if(strcmp(action, "reject") == 0) {
		return REJECT;
	}
	else {
		return NO_ACTION;
	}
}

/**
 * Creates a student
 *
 * @param1 student_id the students ID
 * @param2 first_name the first name of the student
 * @param3 last_name the last name of the student
 * @return
 * The student created. if there was an allocation error return NULL
 */
Student createStudent(char* student_id, char* first_name, char* last_name) {
	//create an instance of a course student
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
	student->id = student_id;
	student->first_name = first_name;
	student->last_name = last_name;
	student->grades = listCreate(copyStudent, destroyStudent);
	if(student->grades == NULL) { //if memory allocation failed
		destroyStudent(student);
		return NULL;
	}
	return student;
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
bool friendRequest(CourseManager course_manager, char* id) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(course_manager->current_student == NULL) {
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
		return true;
	}
}

/**
 * Terminate a friendship between two students
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 id the student, which is being unfriended, ID
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_NOT_FRIEND,
 * MTM_STUDENT_DOES_NOT_EXIST
 * true if there was no error
 */
bool unFriend(CourseManager course_manager, char* id) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student isn't logged in return MTM_NOT_LOGGED_IN
	if(course_manager->current_student == NULL) {
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
	//if the students are not friends
	else if(!setIsIn(course_manager->current_student->friends, student)) {
		course_manager->error = MTM_NOT_FRIEND;
		return false;
	}
	//terminate the friendship between both students
	else {
		return removeFriendship(course_manager, student);
	}
}

/**
 * Handle the request of the student with the "id"
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 id the student, which sent the friend request, ID
 * @param3 action if is "accept" then the two become friends, if "reject" then
 * decline the friend request
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_ALREADY_FRIEND,
 * MTM_STUDENT_DOES_NOT_EXIST, MTM_NOT_REQUESTED, MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool handleFriendRequest(CourseManager course_manager, char* id, char* action) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if the action is not valid
	if(Action(action) == NO_ACTION) {
		course_manager->error = MTM_INVALID_PARAMETERS;
		return false;
	}
	//if a student isn't logged in return MTM_NOT_LOGGED_IN
	if(course_manager->current_student == NULL) {
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
	//if the students are friends
	else if(setIsIn(course_manager->current_student->friends, student)) {
		course_manager->error = MTM_ALREADY_FRIEND;
		return false;
	}
	//if a request wasn't sent
	else if(!setIsIn(student->requests, course_manager->current_student)) {
		course_manager->error = MTM_NOT_REQUESTED;
		return false;
	}
	//Handle the request of the student with the "id"
	else {
		if(Action(action) == ACCEPT) {
			//remove friendship requests from both sides
			setRemove(student->requests, course_manager->current_student);
			setRemove(course_manager->current_student->requests, student);
			//add friendship between the two students
			return addFriendship(course_manager, student);
		}
		else if(Action(action) == REJECT) {
			//remove the friend request of the student
			setRemove(student->requests, course_manager->current_student);
			return true;
		}
		else { //shouldn't reach this place
			course_manager->error = MTM_INVALID_PARAMETERS;
			return true;
		}
	}
}

/**
 * Copys a student
 *
 * @param1 student the student we want to copy
 * @return
 * NULL if there was an allocation failure or student = NULL
 * otherwise the student
 */
Student copyStudent(Student student) {
	//can't do anything if student isn't set
	if(student == NULL) {
		return NULL;
	}
	return createStudent(student->id, student->first_name, student->last_name);
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
int compareStudents(Student student1, Student student2) {
	if(student1 == NULL || student2 == NULL) {
		return 0;
	}
	else if(student1->id == NULL || student2->id == NULL) {
		return 0;
	}
	else {
		return strcmp(student1->id, student2->id);
	}
}

/**
 * Get the logged in student's valid grade for clean report
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 course_id the course ID of the grade
 * @return
 * the valid grade of the course
 * if there is no grade then return -1
 */
int getGrade(CourseManager course_manager, char* course_id) {
	//can't do anything if course_manager isn't set or the course id isn't valid
	if(course_manager == NULL || !isValidCourseID(course_id)) {
		return -1;
	}
	//if no student is logged in to the system
	if(course_manager->current_student == NULL) {
		return -1;
	}
	//find a grade for the course
	LIST_FOREACH(Grade, grade, course_manager) {
		//if grade for course found
		if(strcmp(course_id, grade->course_id) == 0) {
			return grade->grade;
		}
	}
	//no grade was found
	return -1;
}

/**
 * Checks if a student id is valid
 *
 * @param1 student_id the id of the student that is checked
 * @return
 * true if the id is valid, false otherwise
 */
bool isValidStudentID(char* student_id) {
	//NULL isn't a valid student id
	if(student_id == NULL) {
		return false;
	}
	while(student_id) {
		//if the student id contain a character that isn't a digit
		if(student_id < '0' || student_id > '1') {
			return false;
		}
		++student_id;
	}
	int number = atoi(student_id); //string to number
	if(number < STUDENT_ID_MIN || number > STUDENT_ID_MAX) {
		return false;
	}
	else {
		return true;
	}
}

/**
 * Create a friendship between two students, the logged in student and the
 * student sent in the parameters
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student the student that will become friends with the logged in
 * student
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_INVALID_PARAMETERS, MTM_NOT_LOGGED_IN,
 * MTM_ALREADY_FRIEND
 * true if there was no error
 */
bool addFriendship(CourseManager course_manager, Student student) {
	//can't do anything if course_manager or student are not set
	if(course_manager == NULL) {
		return false;
	}
	if(student == NULL) {
		course_manager->error = MTM_INVALID_PARAMETERS;
		return false;
	}
	if(course_manager->current_student == NULL) { //if student is logged in
		course_manager->error = MTM_NOT_LOGGED_IN;
		return false;
	}
	//add "student" as friend to logged in student
	SetResult result = addSet(course_manager->current_student->friends,student);
	if(result == SET_OUT_OF_MEMORY) {
		course_manager->error = MTM_OUT_OF_MEMORY;
		return false;
	}
	else if(result == SET_ITEM_ALREADY_EXISTS) { //if already friends
		course_manager->error = MTM_ALREADY_FRIEND;
		return false;
	}
	//add logged in student as friend to "student"
	result = addSet(student->friends, course_manager->current_student);
	if(result == SET_OUT_OF_MEMORY) {
		course_manager->error = MTM_OUT_OF_MEMORY;
		return false;
	}
	else if(result == SET_ITEM_ALREADY_EXISTS) { //if already friends
		course_manager->error = MTM_ALREADY_FRIEND;
		return false;
	}
	else {
		return true;
	}
}

/**
 * Terminate a friendship between two students, the logged in student and the
 * student sent in the parameters
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student the student that will cease to be friends with the logged in
 * student
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_INVALID_PARAMETERS, MTM_NOT_LOGGED_IN,
 * MTM_NOT_FRIEND
 * true if there was no error
 */
bool removeFriendship(CourseManager course_manager, Student student) {
	//can't do anything if course_manager or student are not set
	if(course_manager == NULL) {
		return false;
	}
	if(student == NULL) {
		course_manager->error = MTM_INVALID_PARAMETERS;
		return false;
	}
	if(course_manager->current_student == NULL) { //if student is logged in
		course_manager->error = MTM_NOT_LOGGED_IN;
		return false;
	}
	//remove "student" as friend from logged in student
	SetResult result = setRemove(course_manager->current_student->friends, 
								 student);
	if(result == SET_OUT_OF_MEMORY) {
		course_manager->error = MTM_OUT_OF_MEMORY;
		return false;
	}
	else if(result == SET_ITEM_DOES_NOT_EXIST) {
		course_manager->error = MTM_NOT_FRIEND;
		return false;
	}
	//remove logged in student as friend from "student"
	result = setRemove(student->friends, course_manager->current_student);
	if(result == SET_OUT_OF_MEMORY) {
		course_manager->error = MTM_OUT_OF_MEMORY;
		return false;
	}
	else if(result == SET_ITEM_DOES_NOT_EXIST) {
		course_manager->error = MTM_NOT_FRIEND;
		return false;
	}
	else {
		return true;
	}
}

/**
 * Destroys an instance of student
 *
 * @param1 student the student we destroy
 */
void destroyStudent(Student student) {
	//nothing to do if student isn't set
	if(student == NULL) {
		return;
	}
	listDestroy(student->grades):
	setDestroy(student->requests);
	setDestroy(student->friends);
	free(student);
}