#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "course_manager.h"
#include "course_manager.c"


/**
 * List of tests, one for each function is usually a good thumb rule.
 */
static bool testCreateCourseManager() {
	ASSERT_TEST(createCourseManager(NULL) != NULL); // shouldn`t return NULL because
	// no allocation error is expected here. 
	return true;
}


static bool testAddStudent() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, 1010, "Mr", "Smith"));// should
	// be true
	ASSERT_TEST(!addStudent(course_manager, 1010, "Mr", "Smith"));// should
	// be FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_ALREADY_EXISTS); // the expected error
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(!addStudent(course_manager, -987, "Mr", "Smith"));// should
	// be FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_INVALID_PARAMETERS); // the expected error	
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	return true;
}



static bool testLoginStudent() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(!loginStudent(course_manager, 0)); //should return FALSE
	// because illegal ID was sent.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(!loginStudent(course_manager, -1113)); //should return FALSE
	// because illegal ID was sent.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(!loginStudent(course_manager, 999999)); // should return FALSE
	// because the ID doesn`t exist in the student SET.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(addStudent(course_manager, 1010, "Mr", "Smith"));// should
	// be true
	ASSERT_TEST(addStudent(course_manager, 12, "Mrs", "Smith"));// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,12)); // should be true
	ASSERT_TEST(!loginStudent(course_manager,12)); // should be FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_ALREADY_LOGGED_IN); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.

	return true;
}
static bool testRemoveStudent() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, 2002, "Yuv", "Smith"));// should
	// be true
	ASSERT_TEST(addStudent(course_manager, 121212, "Mrs", "Gliz"));// should
	// be true
	ASSERT_TEST(!removeStudent(course_manager, 14)); // should be FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(removeStudent(course_manager, 2002)); // should be true
	// we will check if the student was indeed removed by trying to login him:
	ASSERT_TEST(!loginStudent(course_manager,2002)); // should be FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_STUDENT_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	return true;
}

static bool testLogoutStudent() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, 2002, "Tiesto", "!!!"));// should
	// be true
	ASSERT_TEST(addStudent(course_manager, 121212, "Curbi", "???"));// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,2002)); // should be true
	ASSERT_TEST(!logoutStudent(NULL)); // should be FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_NOT_LOGGED_IN); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(logoutStudent(course_manager)); // should be true
	// we will check if the student indeed loged out by trying to login him.
	ASSERT_TEST(loginStudent(course_manager,2002)); // should be true
	return true;
}
/*
static bool testFacultyRequest() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(!facultyRequest(course_manager,"0210","REMOVE_COURSE")); //FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_NOT_LOGGED_IN); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(addStudent(course_manager, 121212, "Curbi", "???"));// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,121212)); // should be true
	ASSERT_TEST(!facultyRequest(course_manager,"-14","REMOVE_COURSE"));
																		//FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_COURSE_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(facultyRequest("1332", "CANCEL_COURSE")); // expected true.
	return true;
}
*/

static bool testIsValidCourseID() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(!isValidCourseID(-14)); // should be FALSE.
	ASSERT_TEST(!isValidCourseID(0)); // should be FALSE.
	ASSERT_TEST(isValidCourseID(1)); //should be true.
	return true;
}
/*
static bool testGetStudent() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	student1 = createStudent(123,"Don","Diablo");
	ASSERT_TEST(student1!=NULL); // expected true.
	ASSERT_TEST(getStudent(course_manager, 124)==NULL); //expected NULL
	ASSERT_TEST(getStudent(course_manager, 123)==student1); //student1 
	//expected to be returned.
	return true;
}
*/
static bool testSetError() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	setError( course_manager, MTM_INVALID_PARAMETERS);
	ASSERT_TEST(getCourseManagerError(course_manager)==MTM_INVALID_PARAMETERS);
	// this answer is expected.
	return true;
}

static bool testHandleInput() {
	bool ok; // bool var to check the success of funcs
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.

	// LOGIN, LOGOUT, ADD, REMOVE
	ok= handleInput(course_manager, "    student    add 15 oliver heldens");
	ASSERT_TEST(ok); // expected true
	ok= handleInput(course_manager, "  	 	 	    	 	   	 	 	 ");
	ASSERT_TEST(ok); // expected true, empty line
	ok= handleInput(course_manager, "  student    add 30 don diablo");
	ASSERT_TEST(ok); // expected true
	ok= handleInput(course_manager, "    student    add -15 oliver heldens");
	ASSERT_TEST(!ok); // illegal ID, expected false.
	ok= handleInput(course_manager, "    student    remove -15 		   		");
	ASSERT_TEST(!ok); // expected false
	ok= handleInput(course_manager, "student remove 15");
	ASSERT_TEST(ok); // expected true
	ok= handleInput(course_manager, "    student    login 30	 		");
	ASSERT_TEST(ok); // expected true
	ok= handleInput(course_manager, "  student     	 	 	      logout");
	ASSERT_TEST(ok); // expected true
	ok= handleInput(course_manager, "    student    login 30	 		");
	ASSERT_TEST(ok); // expected true
	ok= handleInput(course_manager, "    student    add 1 dj tiesto");
	ASSERT_TEST(ok); // expected true

	// FRIENDS // current situation: in system IDs 30,1. logged in 30
	ok= handleInput(course_manager, "    student friend_request 15");
	ASSERT_TEST(!ok); // expected false
	ok= handleInput(course_manager, "       student  friend_request 1");
	ASSERT_TEST(ok); // expected true
	// 30 sent a request to 1.
	ok= handleInput(course_manager, "  student     	 	 	      logout");
	ASSERT_TEST(ok); // expected true
	ok= handleInput(course_manager, "# student login 1   ");
	ASSERT_TEST(ok); // expected true, just a comment
	ok= handleInput(course_manager, "student login 1");
	ASSERT_TEST(ok); // expected true
	// 1 logged in. He sees the request 30 sent to him.
	ok= handleInput(course_manager, "student handle_request 1 accept");
	ASSERT_TEST(!ok); // expected false
	ok= handleInput(course_manager, "student handle_request 30 acceptt");
	ASSERT_TEST(!ok); // expected false
	ok= handleInput(course_manager, "    student handle_request 30 accept");
	ASSERT_TEST(!ok); // expected true
	// congrats, 30 and 1 have officially become friends
	ok= handleInput(course_manager, "");
	ASSERT_TEST(ok); // expected true, empty line
	ok= handleInput(course_manager, " # # student  add -15 oliver heldens");
	ASSERT_TEST(ok); // expected true, just a comment
	ok= handleInput(course_manager, "student 	unfriend 30");
	ASSERT_TEST(ok); // expected true, time to end the friendship

	//GRADES// current situation: 1 and 30 are in the system, 1 logged in,
	// no frienships

	return true;
}

static bool testDestroyCourseManager() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	destroyCourseManager(course_manager);
	return true;
}


int main() {
	RUN_TEST(testCreateCourseManager);
	RUN_TEST(testAddStudent);
	RUN_TEST(testLoginStudent);
	RUN_TEST(testRemoveStudent);
	RUN_TEST(testLogoutStudent);
	//RUN_TEST(testFacultyRequest);
	RUN_TEST(testIsValidCourseID);
	//RUN_TEST(testGetStudent);
	RUN_TEST(testSetError);
	RUN_TEST(testHandleInput);
	RUN_TEST(testDestroyCourseManager);

	return 0;
}


