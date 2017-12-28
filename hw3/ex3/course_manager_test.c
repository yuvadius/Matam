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

static bool testAddStudent() {
	CourseManager course_manager = courseManagerCreate();
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, "1010", "Mr", "Smith");// should
	// be true
	ASSERT_TEST(!addStudent(course_manager, "1010", "Mr", "Smith");// should
	// be FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_ALREADY_EXISTS); // the expected error
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(!addStudent(course_manager, "-987", "Mr", "Smith");// should
	// be FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_INVALID_PARAMETERS); // the expected error	
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	return true;
}

static bool testLoginStudent() {
	CourseManager course_manager = courseManagerCreate();
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(!loginStudent(course_manager, NULL)); //should return FALSE
	// because illegal ID was sent.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(!loginStudent(course_manager, "-1113")); //should return FALSE
	// because illegal ID was sent.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(!loginStudent(course_manager, "999999")); // should return FALSE
	// because the ID doesn`t exist in the student SET.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(addStudent(course_manager, "1010", "Mr", "Smith");// should
	// be true
	ASSERT_TEST(addStudent(course_manager, "12", "Mrs", "Smith");// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,"12")); // should be true
	ASSERT_TEST(!loginStudent(course_manager,"12")); // should be FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_ALREADY_LOGGED_IN); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.

	return true;
}

static bool testRemoveStudent() {
	CourseManager course_manager = courseManagerCreate();
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, "2002", "Yuv", "Smith");// should
	// be true
	ASSERT_TEST(addStudent(course_manager, "121212", "Mrs", "Gliz");// should
	// be true
	ASSERT_TEST(!removeStudent(course_manager, "14")); // should be FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(removeStudent(course_manager, "2002")); // should be true
	// we will check if the student was indeed removed by trying to login him:
	ASSERT_TEST(!loginStudent(course_manager,"2002")); // should be FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	return true;
}

static bool testLogoutStudent() {
	CourseManager course_manager = courseManagerCreate();
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, "2002", "Tiesto", "!!!");// should
	// be true
	ASSERT_TEST(addStudent(course_manager, "121212", "Curbi", "???");// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,"2002")); // should be true
	ASSERT_TEST(logoutStudent(NULL)) // should be FALSE
	ASSERT_TEST(logoutStudent(course_manager)) // should be true
	// we will check if the student indeed loged out by trying to login him.
	ASSERT_TEST(loginStudent(course_manager,"2002")); // should be true
	return true;
}

static bool testFacultyRequest() {
	CourseManager course_manager = courseManagerCreate();
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(!facultyRequest(course_manager,"0210",REMOVE_COURSE)); //FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_NOT_LOGGED_IN); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(addStudent(course_manager, "121212", "Curbi", "???");// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,"121212")); // should be true
		ASSERT_TEST(!facultyRequest(course_manager,"121212",REMOVE_COURSE)); 
		// FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_COURSE_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.




	//???????DO NOT FORGET TO ADD TESTS FOR THE THIRD ERROR MTM_INVALID_PARAMETERS?????????????
	// +CORRECT TESTS FOR TRUE




	return true;
}









int main() {
	RUN_TEST(testCourseManagerCreate);
	RUN_TEST(testHandleInput);
	RUN_TEST(testAddStudent);
	RUN_TEST(testLoginStudent);
	RUN_TEST(testRemoveStudent);
	RUN_TEST(testLogoutStudent);
	RUN_TEST(testFacultyRequest);

	return 0;
}
