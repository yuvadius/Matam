#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "set.h"
#include "list.h"
#include "mtm_ex3.h"
#include "course_manager.h"
#include "student.h"
#include "grade.h"

struct student_t {
	Set friends;
	Set sent_requests;
	int id;
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
	student->sent_requests = setCreate(copyStudent, destroyStudent, compareStudents);
	if(student->sent_requests == NULL) { //if memory allocation failed
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
	student->grades = listCreate(copyGrade, destroyGrade);
	if(student->grades == NULL) { //if memory allocation failed
		destroyStudent(student);
		return NULL;
	}
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
 * Get a student(NOT A COPY) with a certain id from the system
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
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @param3 id the student, which is receiving the friend request, ID
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_ALREADY_FRIEND,
 * MTM_STUDENT_DOES_NOT_EXIST, MTM_ALREADY_REQUESTED
 * true if there was no error
 */
bool friendRequest(CourseManager course_manager, Student student_in, int id) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student is logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	//find the student in the set
	Student student = getStudent(course_manager, id);
	//if the student wasn't found
	if(student == NULL) {
		setError(course_manager, MTM_STUDENT_DOES_NOT_EXIST);
		return false;
	}
	//if the students are already friends or the logged in student sent a
	// request to himself.
	else if(setIsIn(student_in->friends, student) || compareStudents(student_in,
													 student)==0) {
		setError(course_manager, MTM_ALREADY_FRIEND);
		return false;
	}
	//if a request was already sent
	else if(setIsIn(student_in->sent_requests, student)) {
		setError(course_manager, MTM_ALREADY_REQUESTED);
		return false;
	}
	else {
		//add the friend request to the set of requests
		SetResult result = setAdd(student_in->sent_requests, student);
		if(result == SET_OUT_OF_MEMORY) {
			setError(course_manager, MTM_OUT_OF_MEMORY);
			return false;
		}
		else {
			return true;
		}
	}
}

/**
 * Handle the request of the student with the "id"
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
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
bool handleFriendRequest(CourseManager course_manager, Student student_in,
						 int id, char* action) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student isn't logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	//find the student in the set
	Student student = getStudent(course_manager, id);
	//if the student wasn't found
	if(student == NULL) {
		setError(course_manager, MTM_STUDENT_DOES_NOT_EXIST);
		return false;
	}
	//if the students are friends
	else if(setIsIn(student_in->friends, student)) {
		setError(course_manager, MTM_ALREADY_FRIEND);
		return false;
	}
	//if a request wasn't sent
	else if(setIsIn(student->sent_requests, student_in) == false) {
		setError(course_manager, MTM_NOT_REQUESTED);
		return false;
	}
	//Handle the request of the student with the "id"
	//if the action is not valid
	else if(getAction(action) == NO_ACTION) {
		setError(course_manager, MTM_INVALID_PARAMETERS);
		return false;
	}
	else if(getAction(action) == ACCEPT) {
		//remove friendship requests from both sides
		setRemove(student->sent_requests, student_in);
		setRemove(student_in->sent_requests, student);
		//add friendship between the two students
		return addFriendship(course_manager, student_in, student);
	}
	else if(getAction(action) == REJECT) {
		//remove the friend request of the student
		setRemove(student->sent_requests, student_in);
		return true;
	}
	else { //shouldn't reach this place
		return false;
	}
}

/**
 * Create a friendship between two students, the logged in student and the
 * student sent in the parameters
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @param3 student the student that will become friends with the logged in
 * student
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_INVALID_PARAMETERS, MTM_NOT_LOGGED_IN,
 * MTM_ALREADY_FRIEND
 * true if there was no error
 */
bool addFriendship(CourseManager course_manager, Student student_in,
				   Student student) {
	//can't do anything if course_manager or student are not set
	if(course_manager == NULL) {
		return false;
	}
	if(student_in == NULL) { //if student isn't logged in
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	if(student == NULL) { //student must be set
		setError(course_manager, MTM_INVALID_PARAMETERS);
		return false;
	}
	//add "student" as friend to logged in student
	SetResult result = setAdd(student_in->friends, student);
	if(result == SET_OUT_OF_MEMORY) {
		setError(course_manager, MTM_OUT_OF_MEMORY);
		return false;
	}
	else if(result == SET_ITEM_ALREADY_EXISTS) { //if already friends
		setError(course_manager, MTM_ALREADY_FRIEND);
		return false;
	}
	//add logged in student as friend to "student"
	result = setAdd(student->friends, student_in);
	if(result == SET_OUT_OF_MEMORY) {
		setError(course_manager, MTM_OUT_OF_MEMORY);
		return false;
	}
	else if(result == SET_ITEM_ALREADY_EXISTS) { //if already friends
		setError(course_manager, MTM_ALREADY_FRIEND);
		return false;
	}
	else {
		return true;
	}
}

/**
 * Terminate a friendship between two students
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @param3 id the student, which is being unfriended, ID
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN, MTM_NOT_FRIEND,
 * MTM_STUDENT_DOES_NOT_EXIST
 * true if there was no error
 */
bool unFriend(CourseManager course_manager, Student student_in, int id) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student isn't logged in return MTM_NOT_LOGGED_IN
	if(student_in == NULL) {
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	//find the student in the set
	Student student = getStudent(course_manager, id);
	//if the student wasn't found
	if(student == NULL) {
		setError(course_manager, MTM_STUDENT_DOES_NOT_EXIST);
		return false;
	}
	//if the students are not friends
	else if(setIsIn(student_in->friends, student) == false) {
		setError(course_manager, MTM_NOT_FRIEND);
		return false;
	}
	//terminate the friendship between both students
	else {
		return removeFriendship(course_manager, student_in, student);
	}
}

/**
 * Terminate a friendship between two students, the logged in student and the
 * student sent in the parameters
 *
 * @param1 course_manager the CourseManager that the logged in student is in
 * @param2 student_in the current student that is logged in(will be NULL if no
 * student is logged in)
 * @param3 student the student that will cease to be friends with the logged in
 * student
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors:
 * true if there was no error
 */
bool removeFriendship(CourseManager course_manager, Student student_in,
					  Student student) {
	//can't do anything if course_manager or student are not set
	if(course_manager == NULL) {
		return false;
	}
	if(student_in == NULL) { //if student is logged in
		setError(course_manager, MTM_NOT_LOGGED_IN);
		return false;
	}
	//remove "student" as friend from logged in student
	SetResult result = setRemove(student_in->friends, student);
	if(result == SET_ITEM_DOES_NOT_EXIST) {
		setError(course_manager, MTM_NOT_FRIEND);
		return false;
	}
	//remove logged in student as friend from "student"
	result = setRemove(student->friends, student_in);
	if(result == SET_ITEM_DOES_NOT_EXIST) {
		setError(course_manager, MTM_NOT_FRIEND);
		return false;
	}
	return true;
}

/**
 * Get a student's grades
 *
 * @param1 student the student that has the grades
 * @return
 * The list of grades. if there was an error return NULL
 */
List getStudentGrades(Student student) {
	//can't do anything if the student isn't set
	if(student == NULL) {
		return NULL;
	}
	else {
		return student->grades;
	}
}

/**
 * Get the student's first name
 *
 * @param1 student the student that has the first name
 * @return
 * The first name. if there was an error return NULL
 */
char* getStudentFirstName(Student student) {
	//can't do anything if the student isn't set
	if(student == NULL) {
		return NULL;
	}
	else {
		return student->first_name;
	}
}

/**
 * Get the student's last name
 *
 * @param1 student the student that has the last name
 * @return
 * The last name. if there was an error return NULL
 */
char* getStudentLastName(Student student) {
	//can't do anything if the student isn't set
	if(student == NULL) {
		return NULL;
	}
	else {
		return student->last_name;
	}
}

/**
 * Get the student's newest grade for a course
 *
 * @param1 student the student that will be checked
 * @param2 course_id the course ID of the grade
 * @return
 * the newest grade of the course
 * if there is no grade then return -1
 */
int getNewestGrade(Student student, int course_id) {
	//can't do anything if the student isn't set
	if(student == NULL) {
		return -1;
	}
	//find a grade for the course
	LIST_FOREACH(Grade, grade, student->grades) {
		//if grade for course found
		if(course_id == getCourseID(grade)) {
			return getGrade(grade);
		}
	}
	//no grade was found
	return -1;
}

/**
 * Get the student's set of friends
 *
 * @param1 student the student that contains the set of friends
 * @return
 * NULL on error, the set of friends on success
 */
Set getStudentFriends(Student student) {
	//can't do anything if the student isn't set
	if(student == NULL) {
		return NULL;
	}
	else {
		return student->friends;
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
	setDestroy(((Student)student)->sent_requests);
	setDestroy(((Student)student)->friends);
	free((Student)student);
}
