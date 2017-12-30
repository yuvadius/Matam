#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "student.h"


static bool testCreateStudent() {
	Student student= createStudent("110", "Sharon", "Mishayev"); // no memory
	//allocation failiure expected.
	ASSERT_TEST(student!=NULL); //student should NOT be NULL
	return true;
}

static bool testFriendRequest() {
	CourseManager course_manager = createCourseManager();
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager,"110", "Alpha", "Beta"))// true expect
																			//ed
	ASSERT_TEST(!friendRequest(course_manager, "222"));// FALSE expected.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_NOT_LOGGED_IN); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(loginStudent(course_manager,"110")); // should be true
	ASSERT_TEST(!friendRequest(course_manager, "222"));// FALSE expected.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(!friendRequest(course_manager, "110"));// FALSE expected.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_ALREADY_FRIEND); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(addStudent(course_manager, "222","HH","YY"));// true expected.
	ASSERT_TEST(friendRequest(course_manager, "222"));// true expected.
	ASSERT_TEST(friendRequest(course_manager, "222"));// FALSE expected.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_ALREADY_REQUESTED); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	return true;
}

static bool testHandleFriendRequest() {
	CourseManager course_manager = createCourseManager();
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, "1","HH","YY"));// true expected.
	ASSERT_TEST(!handleFriendRequest(course_manager, "666","accept"));//FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_NOT_LOGGED_IN); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(loginStudent(course_manager,"1"));//should be true.
	ASSERT_TEST(!handleFriendRequest(course_manager, "666","accept"));//FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(addStudent(course_manager, "666", "A", "A"));// true expected.
	ASSERT_TEST(!handleFriendRequest(course_manager, "666","accept"));//FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_NOT_REQUESTED); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(!handleFriendRequest(course_manager, "666","accepttt"));//FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_INVALID_PARAMETERS); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(friendRequest(course_manager, "666"));// true expected.
	// NOTE: "1" sent a request to "666". Therefore, in order to react to the
	// request - "666" has to be logged in.
	ASSERT_TEST(handleFriendRequest(course_manager, "666","accept")); // FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_NOT_REQUESTED); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(logoutStudent(course_manager)); // true expected. "1" was logged
	// out.
	ASSERT_TEST(loginStudent(course_manager,"666")); // true expected. "666" was
	// logged in to the system.
	ASSERT_TEST(handleFriendRequest(course_manager, "1","accept")); // true 
	//expected. Now, "1" and "666" are friends.
	ASSERT_TEST(friendRequest(course_manager, "1"));// FALSE expected.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_ALREADY_FRIEND); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.





	return true:
}









int main() {
	RUN_TEST(testCreateStudent);
	RUN_TEST(testFriendRequest);
	RUN_TEST(testHandleFriendRequest);

	return 0;
}