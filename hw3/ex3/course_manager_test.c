#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "course_manager.h"


/**
 * List of tests, one for each function is usually a good thumb rule.
 */
static bool testCourseManagerCreate() {
	ASSERT_TEST(courseManagerCreate() != NULL); // shouldn`t return NULL because
	// no allocation error is expected here. 
	return true;
}

static bool testHandleInput() {

	return true;
}

static bool testLoginStudent() {
	CourseManager course_manager = courseManagerCreate();
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(!loginStudent(course_manager, NULL)); //should return FALSE
	// because illegal ID was sent.
	ASSERT_TEST(getCourseManagerError(course_manager)==


	return true;
}









int main() {
	RUN_TEST(testCourseManagerCreate);
	RUN_TEST(testHandleInput);
	RUN_TEST(testLoginStudent);

	return 0;
}
