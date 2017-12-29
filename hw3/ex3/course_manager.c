#include <stdlib.h>
#include <assert.h>
#include "set.h"
#include "student.h"

struct course_manager_t {
	Set students;
	Student current_student;
	MtmErrorCode error;
};

/**
 * Converts a faculty request string to a constant
 *
 * @param1 request the faculty request string
 * @return
 * The FacultyRequest representing the faculty request string
 */
static FacultyRequest getFacultyRequst(char* request) {
	if(strcmp(request, "remove_course") == 0) {
		return REMOVE_COURSE;
	}
	else if(strcmp(request, "register_course") == 0) {
		return REGISTER_COURSE;
	}
	else if(strcmp(request, "cancel_course") == 0) {
		return CANCEL_COURSE;
	}
	else {
		return NO_REQUEST;
	}
}

/**
 * Creates an empty CourseManager
 *
 * @return
 * The CourseManager created. if there was an allocation error return NULL
 */
CourseManager createCourseManager() {
	//create an instance of a course manager
	CourseManager course_manager = malloc(sizeof(*course_manager));
	if(course_manager == NULL) { //if there was a memory failure return NULL
		return NULL;
	}
	course_manager->students = setCreate(copyStudent, destroyStudent, 
										 compareStudents);
	//if memory allocation failed
	if(course_manager->students == NULL) {
		free(course_manager);
		return NULL;
	}
	course_manager->current_student = NULL;
	course_manager->error = NULL;
	return course_manager;
}

/**
 * Processes an input line command
 *
 * @param1 course_manager the CourseManager that the student will be logged into
 * @param2 input_line a line received from the input stream
 * @return
 * false if there was an error. The error will be written to
 * course_manager->error.
 * true if there was no error
 */
bool handleInput(CourseManager course_manager, char* input_line) {
	return true;
}

/**
 * Logs in a student to the system
 *
 * @param1 course_manager the CourseManager that the student will be logged into
 * @param2 student_id the students ID
 * @return 
 * false if there was an error. The error will be written to
 * course_manager->error.
 * Possible Non Critical Errors: MTM_ALREADY_LOGGED_IN, 
 * MTM_STUDENT_DOES_NOT_EXIST
 * true if there was no error
 */
bool loginStudent(CourseManager course_manager, char* student_id) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if the parameters aren't valid return MTM_INVALID_PARAMETERS
	if(student_id == NULL) {
		course_manager->error = MTM_INVALID_PARAMETERS;
		return false;
	}
	//if a student is logged in return MTM_ALREADY_LOGGED_IN
	if(course_manager->current_student != NULL) {
		course_manager->error = MTM_ALREADY_LOGGED_IN;
		return false;
	}
	//find the student in the set
	SET_FOREACH(Student, student, course_manager->students) {
		//if the student was found
		if(strcmp(student->id, student_id) == 0) {
			course_manager->current_student = student;
			return true;
		}
	}
	//if the student wasn't found
	course_manager->error = MTM_STUDENT_DOES_NOT_EXIST;
	return false;
}

/**
 * Logs out a student from the system
 *
 * @param1 course_manager CourseManager that the student will be logged out of
 * @return 
 * false if there was an error. The error will be written to
 * course_manager->error.
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN
 * true if there was no error
 */
bool logoutStudent(CourseManager course_manager) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if a student is not logged in return MTM_NOT_LOGGED_IN
	if(course_manager->current_student == NULL) {
		course_manager->error = MTM_NOT_LOGGED_IN;
		return false;
	}
	course_manager->current_student = NULL; //logout the student
	return true;
}

/**
 * Remove the student from the system
 *
 * @param1 course_manager CourseManager that the student will be removed from
 * @param2 student_id the students ID
 * @return 
 * false if there was an error. The error will be written to
 * course_manager->error.
 * Possible Non Critical Errors: MTM_STUDENT_DOES_NOT_EXIST
 * true if there was no error
 */
bool removeStudent(CourseManager course_manager, char* student_id) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if the parameters aren't valid return MTM_INVALID_PARAMETERS
	if(student_id == NULL) {
		course_manager->error = MTM_INVALID_PARAMETERS;
		return false;
	}
	logoutStudent(course_manager); //log out the student if he is logged in
	//find the student in the set
	SET_FOREACH(Student, student, course_manager->students) {
		//if the student was found
		if(strcmp(student->id, student_id) == 0) {
			destroyStudent(student);
			return true;
		}
	}
	//if the student wasn't found
	course_manager->error = MTM_STUDENT_DOES_NOT_EXIST;
	return false;
}

/**
 * add the student to the system
 *
 * @param1 course_manager the CourseManager that the student will be added to
 * @param2 student_id the students ID
 * @param3 first_name the first name of the student
 * @param4 last_name the last name of the student
 * @return 
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_STUDENT_ALREADY_EXISTS,
 * MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool addStudent(CourseManager course_manager, char* student_id, 
				char* first_name, char* last_name) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if the parameters aren't valid return MTM_INVALID_PARAMETERS
	if(student_id == NULL || first_name == NULL || last_name == NULL) {
		course_manager->error = MTM_INVALID_PARAMETERS;
		return false;
	}
	//create a student
	Student student = createStudent(student_id, first_name, last_name);
	//if there was a memory allocation failure
	if(student == NULL) {
		course_manager->error = MTM_OUT_OF_MEMORY;
		return false;
	}
	//add a copy of student to the set
	SetResult result = setAdd(course_manager->students, student);
	destroyStudent(student);
	if(result == SET_OUT_OF_MEMORY) {
		course_manager->error = MTM_OUT_OF_MEMORY;
		return false
	}
	else if(result == SET_ITEM_ALREADY_EXISTS) { //if the student was found
		course_manager->error = MTM_STUDENT_ALREADY_EXISTS;
		return false;
	}
	else {
		return true;
	}
}

/**
 * Ignore the student faculty request
 *
 * @param1 course_manager CourseManager that the student will be removed from
 * @param2 course_id the course ID
 * @param3 requst the request of the student which can be "remove_course" or
 * "register_course" or "cancel_course"
 * @return 
 * false if there was an error. The error will be written to
 * course_manager->error
 * Possible Non Critical Errors: MTM_NOT_LOGGED_IN,
 * MTM_COURSE_DOES_NOT_EXIST, MTM_INVALID_PARAMETERS
 * true if there was no error
 */
bool facultyRequest(CourseManager course_manager, char* course_id,
					char* request) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	//if the parameters aren't valid return MTM_INVALID_PARAMETERS
	if(course_id == NULL || request == NULL) {
		course_manager->error = MTM_INVALID_PARAMETERS;
		return false;
	}
	//if no student is logged in to the system
	if(course_manager->current_student == NULL) {
		course_manager->error = MTM_NOT_LOGGED_IN;
		return false;
	}
	//if the request is to remove a course that does not exist
	if(getFacultyRequst(request) == REGISTER_COURSE &&
	   getGrade(course_manager, course_id) == -1) {
		course_manager->error = MTM_COURSE_DOES_NOT_EXIST;
		return false;
	}
	return true;
}

/**
 * return the last "error" produced by one of the funcs.
 *
 * @param1 course_manager CourseManager that we take the error from.
 * @return 
 * returns course_manager->error.
 * NULL if something went wrong.

 */
MtmErrorCode getCourseManagerError(CourseManager course_manager) {
	//can't do anything if course_manager isn't set
	if(course_manager == NULL) {
		return false;
	}
	return course_manager->errorl
}

/**
 * Destroy an instance of CourseManager
 *
 * @param1 course_manager the CourseManager we destroy
 */
void destroyCourseManager(CourseManager course_manager) {
	//nothing to do if course_manager isn't set
	if(course_manager == NULL) {
		return;
	}
	setDestroy(course_manager->students); //destroy all students in the system
	free(course_manager);
}