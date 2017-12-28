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









int main() {
	RUN_TEST(testCourseManagerCreate);
	RUN_TEST()

	return 0;
}
