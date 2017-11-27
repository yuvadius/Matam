#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "DynamicArray.h"
#include "Course.h"
#include "CourseSystem.h"

CourseSystem createSystem(char *name) {
	CourseSystem course_system = malloc(sizeof(*course_system));
	if(course_system != NULL) {
		course_system->name = (char *)malloc(strlen(name) + 1);
		if(course_system->name == NULL) {
			return NULL;
		}
		strcpy(course_system->name, name);
		course_system->courses = createDynamicArray();
		return course_system;
	}
	else { //allocation of course_system failed
		return NULL;
	}
}

int sysNumCourses(CourseSystem sys) {
	assert(sys != NULL);
	return sys->courses->len;
}

SysResult sysAddCourse(CourseSystem sys, Course c1) {
	assert((sys != NULL) && (c1 != NULL));
	int result_index = 0;
	if(indexOfElement(sys->courses, c1, 0, &result_index) == DA_OK) {
		if(result_index != -1) {
			return SYS_COURSE_ALREADY_EXIST;
		}
	}
	for(int i = 0; i < sys->courses->len; ++i) {
		//this works because the courses are sorted by id in ascending order
		if(courseLessThan(c1, sys->courses->elements[i])) {
			addElementBefore(sys->courses, c1, i);
			return SYS_OK;
		}
	}
	addElementEnd(sys->courses, c1);
	return SYS_OK;
}

SysResult sysRemoveCourse(CourseSystem sys, char *course_id) {
	assert((sys != NULL) && (course_id != NULL));
	int result = SYS_NOT_IN_SYSTEM;
	for(int i = 0; i < sys->courses->len; ++i) {
		sysRemovePreCourse(sys, sys->courses->elements[i]->id, course_id);
		if(strcmp(sys->courses->elements[i]->id, course_id) == 0) {
			removeElement(sys->courses, i);
			result = SYS_OK;
		}
	}
	return result;
}

SysResult sysIsPreCourse(CourseSystem sys, char *course_id1 , char *course_id2,
						int *ans) {
	assert((sys!=NULL)&&(course_id1!=NULL)&&(course_id2!=NULL)&&(ans!=NULL));
	bool isCourse1InSystem = false, isCourse2InSystem = false;
	int temp_ans = 0;
	for(int i = 0; i < sys->courses->len; ++i) {
		if(strcmp(sys->courses->elements[i]->id, course_id1) == 0) {
			isCourse1InSystem = true;
			DynamicArray preCourses = sys->courses->elements[i]->preCourses;
			for(int j = 0; j < preCourses->len; ++j) {
				if(strcmp(preCourses->elements[j]->id, course_id2) == 0) {
					temp_ans = 1;
					break;
				}
			}
		}
		else if(strcmp(sys->courses->elements[i]->id, course_id2) == 0) {
			isCourse2InSystem = true;
		}
	}
	if (isCourse1InSystem && isCourse2InSystem) {
        *ans = temp_ans;
		return SYS_OK;
	}
	else {
		return SYS_NOT_IN_SYSTEM;
	}
}

SysResult sysAddPreCourse(CourseSystem sys,char *course_id1 ,char *course_id2) {
	assert((sys != NULL) && (course_id1 != NULL) && (course_id2 != NULL));
	int isPrecourse = 0;
	int result = sysIsPreCourse(sys, course_id1, course_id2, &isPrecourse);
	if(result == SYS_NOT_IN_SYSTEM) {
		return result;
	}
	else if(isPrecourse == 1) {
		return SYS_ALREADY_PRE_COURSE;
	}
	else {
		Course course1 = NULL, course2 = NULL;
		for(int i = 0; i < sys->courses->len; ++i) {
			if(strcmp(sys->courses->elements[i]->id, course_id1) == 0) {
				course1 = sys->courses->elements[i];
			}
			else if(strcmp(sys->courses->elements[i]->id, course_id2) == 0) {
				course2 = sys->courses->elements[i];
			}
		}
		addPreCourse(course1, course2);
		return SYS_OK;
	}
}

SysResult sysRemovePreCourse(CourseSystem sys, char *course_id1,
							char *course_id2) {
	assert((sys != NULL) && (course_id1 != NULL) && (course_id2 != NULL));
	int isPrecourse = 0;
	int result = sysIsPreCourse(sys, course_id1, course_id2, &isPrecourse);
	if(result == SYS_NOT_IN_SYSTEM) {
		return result;
	}
	else if(isPrecourse == 0) {
		return SYS_NOT_PRE_COURSE;
	}
	else {
		Course course1 = NULL, course2 = NULL;
		for(int i = 0; i < sys->courses->len; ++i) {
			if(strcmp(sys->courses->elements[i]->id, course_id1) == 0) {
				course1 = sys->courses->elements[i];
			}
			else if(strcmp(sys->courses->elements[i]->id, course_id2) == 0) {
				course2 = sys->courses->elements[i];
			}
		}
		//course1 and course2 should not be NULL over here
		removePreCourse(course1, course2);
		return SYS_OK;
	}
}

SysResult sysUpdateCourseName(CourseSystem sys,char *course_id,char *new_name) {
	assert((sys != NULL) && (course_id != NULL) && (new_name != NULL));
	for(int i = 0; i < sys->courses->len; ++i) {
		if(strcmp(sys->courses->elements[i]->id, course_id) == 0) {
			free(sys->courses->elements[i]->name);
			sys->courses->elements[i]->name=malloc(sizeof(char)*(strlen(new_name)+1));
			if(sys->courses->elements[i]->name == NULL) {
				return SYS_MEMORY_PROBLEM;
			}
			strcpy(sys->courses->elements[i]->name, new_name);
			return SYS_OK;
		}
	}
	return SYS_NOT_IN_SYSTEM;
}

void displaySystem(CourseSystem sys) {
	assert(sys != NULL);
	printf("%s\n", sys->name);
	for(int i = 0; i < sys->courses->len; ++i) {
		displayCourse(sys->courses->elements[i]);
		printf("\n");
	}
}

void destroySystem(CourseSystem sys) {
	free(sys->name);
	destroyDynamicArray(sys->courses);
	free(sys);
}
