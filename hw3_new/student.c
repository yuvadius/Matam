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
	student->id = student_id;
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
