#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Course.h"
#include "DynamicArray.h"
#include <stdio.h>

//------------------------------------------------------------------------------------------
// create a new course with the details of the parameters id , name , credits;
// no pre courses are defined here.
// id and name should be copied into the new crated course by duplication.
// they must not be changed.
// Either id or name or course must not be NULL. (handled by assert).
// course is an output parameter which gets the created new course.
// return value : COURSE_OK ,
//                COURSE_MEMORY_ERROR  if there are memory problems
//                COURSE_ILLEGAL_PARAMETER if credits < 0
CourseResult createCourse(char *id, char *name, double credits, Course *course) {
	assert((id != NULL) && (name != NULL));
	if(credits < 0) {
		return COURSE_ILLEGAL_PARAMETER;
	}
	//free(*course);
	*course = malloc(sizeof(**course));
	if((*course) == NULL) {
        return COURSE_MEMORY_ERROR;
	}
	(*course)->id = (char *)malloc(strlen(id) + 1);
	(*course)->name = (char *)malloc(strlen(name) + 1);
	if((*course)->id == NULL || (*course)->name == NULL) {
		return COURSE_MEMORY_ERROR;
	}
	strcpy((*course)->id, id);
	strcpy((*course)->name, name);
	(*course)->credits = credits;
	(*course)->preCourses = createDynamicArray();
	if((*course)->preCourses == NULL) {
		return COURSE_MEMORY_ERROR;
	}
	return COURSE_OK;
}

//------------------------------------------------------------------------------------------
// checks if course1 and course2 have the same id. returns 1 or 0 accordingly.
// Either course1 or course2 must not be NULL. (handled by assert).

int coursesEqualId(Course course1, Course course2) {
	assert((course1 != NULL) && (course2 != NULL));
	return (strcmp(course1->id, course2->id) == 0);
}

//------------------------------------------------------------------------------------------
// checks if course1 is less than (not equal) course2. returns 1 if yes or 0 if no.
// the comparison is made according to the id of the courses, by means of a lexicographic order.
// Either course1 or course2 must not be NULL. (handled by assert).
int courseLessThan(Course course1, Course course2) {
	assert((course1 != NULL) && (course2 != NULL));
	if(strcmp(course1->id, course2->id) < 0) {
		return 1;
	}
	else {
		return 0;
	}
}

//------------------------------------------------------------------------------------------
// update course1 so that its name will be as new_name.
// the new name should be physically duplicated into course1.
// new_name must not be NULL. (handled by assert).
// return value : COURSE_OK ,
//                COURSE_MEMORY_ERROR  if there are memory problems. in that case,
//                the old name stays.
CourseResult courseUpdateName(Course course1, char *new_name) {
	assert(new_name != NULL);
	free(course1->name);
	course1->name = (char *)malloc(strlen(new_name) + 1);
	if(course1->name == NULL) {
		return COURSE_MEMORY_ERROR;
	}
	strcpy(course1->name, new_name);
	return COURSE_OK;
}

//------------------------------------------------------------------------------------------
// add course2 as a pre course of course1. course2 is not duplicated. just a simple assignment.
// all the pre courses of course1 should be keeped sorted, according to their id (lexicographic order,
// from small to big).
// Either course1 or course2 must not be NULL. (handled by assert).
// return value : COURSE_OK,
//                COURSE_THE_SAME_COURSE  if the id of course2 is the same as the id of course1
//                COURSE_ALREADY_EXISTS if a course with the id of course2 already registered as a pre course of course1
CourseResult addPreCourse(Course course1, Course course2) {
	assert((course1 != NULL) && (course2 != NULL));
	if(coursesEqualId(course1, course2)) {
		return COURSE_THE_SAME_COURSE;
	}
	int result_index = 0;
	if(indexOfElement(course1->preCourses, course2, 0, &result_index) == DA_OK) {
		if(result_index != -1) {
			return COURSE_ALREADY_EXISTS;
		}
	}
	for(int i = 0; i < course1->preCourses->len; ++i) {
		if(courseLessThan(course2, course1->preCourses->elements[i])) {
			addElementBefore(course1->preCourses, course2, i);
			return COURSE_OK;
		}
	}
	addElementEnd(course1->preCourses, course2);
	return COURSE_OK;
}

//------------------------------------------------------------------------------------------
// remove a course with the same id as of course2 from being a pre course of course1.
// no need to do any free here. just erase the pre course connection.
// all the rest pre courses of course1 should be keeped sorted, according to their id (lexicographic order,
// from small to big).
// Either course1 or course2 must not be NULL. (handled by assert).
// return value : COURSE_OK,
//                COURSE_NOT_EXIST if a course with the id of course2 is not defined as a pre course of course1
CourseResult removePreCourse(Course course1, Course course2) {
	assert((course1 != NULL) && (course2 != NULL));
	int result_index = 0;
	if(indexOfElement(course1->preCourses, course2, 0, &result_index) == DA_OK) {
		if(result_index == -1) {
			return COURSE_NOT_EXIST;
		}
		else {
			assert(removeElement(course1->preCourses, result_index) == DA_OK);
			return COURSE_OK;
		}
	}
	return COURSE_NOT_EXIST;//if precourses is empty then the course does not exist
}

//------------------------------------------------------------------------------------------
// display in a single line the details of course1 as follows (from left to right) :
// id , name , credits , ids of pre courses.
// credits is displayed in the format of %.1f
// the ids of pre courses are dispalyed sorted from left to right from small to big.
// between evert 2 adjacent word the is a single space. no spaces in the beginning of the line or
// at its end. no new line at the end of the displayed line.

void displayCourse(Course course1) {
	assert(course1 != NULL);
	printf("%s ", course1->id);
	printf("%s ", course1->name);
	printf("%.1f", course1->credits);
	for(int i = 0; i < course1->preCourses->len; ++i) {
		printf(" %s", course1->preCourses->elements[i]->id);
	}
}

//------------------------------------------------------------------------------------------
// deallocate all relevant memory of course1 and stop using it.
void destroyCourse(Course course1) {
	free(course1->id);
	free(course1->name);
	destroyDynamicArray(course1->preCourses);
    free(course1);
}
