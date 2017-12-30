#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "student.h"

static bool testCreateGrade() {
	Grade grade = createGrade("1","234114","4.0","62");
	ASSERT_TEST(grade!=NULL); //shouldnt be NULL.
	return true;
}

static bool testAddGrade() {
	CourseManager course_manager = createCourseManager();
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(!addGrade(course_manager, "-1","2","4.5","12"));//FALSE expected
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_NOT_LOGGED_IN); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(addStudent(course_manager,"110", "Alpha", "Beta"));//true expect
																			//ed
	ASSERT_TEST(loginStudent(course_manager,"110"));//should be true.
	ASSERT_TEST(!addGrade(course_manager, "-1","2","4.5","12"));//FALSE expected
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_INVALID_PARAMETERS); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(!addGrade(course_manager, "1","2","4.4","12"));//FALSE expected
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_INVALID_PARAMETERS); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(addGrade(course_manager, "1","2","4.5","12"));// true expected.
	ASSERT_TEST(addGrade(course_manager, "1","2","4.5","12"));// true expected:
	// added the same grade twice. it should NOT be a problem.
	ASSERT_TEST(listGetcourse_manager->current_student->grades)

														

	return true;
}




int main() {
	RUN_TEST(testCreateGrade);
	RUN_TEST(testAddGrade);




	return 0;
}