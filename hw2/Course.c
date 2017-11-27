#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Course.h"
#include "DynamicArray.h"
#include <stdio.h>

CourseResult createCourse(char *id, char *name, double credits, Course *course){
	assert((id != NULL) && (name != NULL));
	if(credits < 0) {//credits can't be negative
		return COURSE_ILLEGAL_PARAMETER;
	}
	Course temp_course = malloc(sizeof(*temp_course));
	temp_course->id = malloc(sizeof(char) * (strlen(id) + 1));
	temp_course->name = malloc(sizeof(char) * (strlen(name) + 1));
	if(temp_course->id == NULL || temp_course->name == NULL) {
		return COURSE_MEMORY_ERROR;
	}
	strcpy(temp_course->id, id);
	strcpy(temp_course->name, name);
	temp_course->credits = credits;
	temp_course->preCourses = createDynamicArray();
	if(temp_course->preCourses == NULL) {
		return COURSE_MEMORY_ERROR;
	}
	*course = temp_course;
	return COURSE_OK;
}

int coursesEqualId(Course course1, Course course2) {
	assert((course1 != NULL) && (course2 != NULL));
	return (strcmp(course1->id, course2->id) == 0);
}

int courseLessThan(Course course1, Course course2) {
	assert((course1 != NULL) && (course2 != NULL));
	if(strcmp(course1->id, course2->id) < 0) {
		return 1;
	}
	else {
		return 0;
	}
}

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

CourseResult addPreCourse(Course course1, Course course2) {
	assert((course1 != NULL) && (course2 != NULL));
	if(coursesEqualId(course1, course2)) {
		return COURSE_THE_SAME_COURSE;
	}
	int result_index = 0;
	if(indexOfElement(course1->preCourses, course2, 0, &result_index) == DA_OK){
		if(result_index != -1) {
			return COURSE_ALREADY_EXISTS;
		}
	}
	for(int i = 0; i < course1->preCourses->len; ++i) {
		//this works because the precourses are sorted by id in ascending order
		if(courseLessThan(course2, course1->preCourses->elements[i])) {
			addElementBefore(course1->preCourses, course2, i);
			return COURSE_OK;
		}
	}
	addElementEnd(course1->preCourses, course2);
	return COURSE_OK;
}

CourseResult removePreCourse(Course course1, Course course2) {
	assert((course1 != NULL) && (course2 != NULL));
	int result_index = 0;
	if(indexOfElement(course1->preCourses, course2, 0, &result_index) == DA_OK){
		if(result_index == -1) {
			return COURSE_NOT_EXIST;
		}
		else {
			removeElement(course1->preCourses, result_index);
			return COURSE_OK;
		}
	}
	return COURSE_NOT_EXIST;//if precourses is empty, the course does not exist
}

void displayCourse(Course course1) {
	assert(course1 != NULL);
	printf("%s ", course1->id);
	printf("%s ", course1->name);
	printf("%.1f", course1->credits);
	for(int i = 0; i < course1->preCourses->len; ++i) {
		printf(" %s", course1->preCourses->elements[i]->id);
	}
}

void destroyCourse(Course course1) {
	free(course1->id);
	free(course1->name);
	destroyDynamicArray(course1->preCourses);
    free(course1);
}
