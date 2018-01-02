#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "../student.h"
#include "../grade.h"
#include "../list.h"

static bool testCreateGrade() {
	Grade grade = createGrade(1,300,8,46);
	ASSERT_TEST(grade!=NULL); // no allocation error expected here.
	return true;
}

static bool testIsValidSemester() {
	Grade grade = createGrade(1,300,8,46);
	ASSERT_TEST(grade!=NULL); // no allocation error expected here.
	// there is no actual way to check the semester. We are not allowed to  
	//create new funcs(getSemester) just for ADT tests. Therefore the only way   
	//to check the validity of the semester input is the output of createGrade
	//func.
	return true;
}

static bool testIsValidPointsX2() {
	Grade grade = createGrade(1,300,8,46);
	ASSERT_TEST(grade!=NULL); // no allocation error expected here.
	// there is no actual way to check the pointsX2. We are not allowed to  
	//create new funcs(getPointsX2) just for ADT tests. Therefore the only way   
	//to check the validity of the pointsX2 input is the output of createGrade
	//func.
	return true;
}

static bool testGetGrade() {
	Grade grade = createGrade(1,300,8,46);
	ASSERT_TEST(grade!=NULL); // no allocation error expected here.
	int int_grade = getGrade(grade);
	ASSERT_TEST(int_grade==46); // expected grade - 46.
	return true;
}

static bool testIsValidGrade() {
	Grade grade = createGrade(1,300,8,13);
	ASSERT_TEST(grade!=NULL);// no allocation error expected here.
	int int_grade = getGrade(grade);
	ASSERT_TEST(int_grade!= -1);//createGrade uses isValidGrade. Therefore we
	//should never have an illegal grade. If an illegal grade was sent to
	//createGrade, Grade grade=NULL.
	return true;
}


static bool testAddGrade() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, 1010, "aa", "bb"));// should
	// be true
	Student student = getStudent(course_manager, 1010);
	List grades_list = getStudentGrades(student);
	ASSERT_TEST(grades_list!=NULL); // shoudln`t be NULL.
	ASSERT_TEST(!addGrade(course_manager, NULL,grades_list, 1, 300, 8, 48)); 
	// FALSE expected.
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_NOT_LOGGED_IN); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(loginStudent(course_manager,1010));//should be true.
	// "1" logged in.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 300, 8, 48));
	// true expected.
	Grade grade_first = listGetFirst(grades_list); // the first list element.
	ASSERT_TEST(grades_list!=NULL); // shouldnt be NULL.
	Grade grade_added_to_list = createGrade(1,300,8,48); // the grade we added.
	ASSERT_TEST(grade_added_to_list!=NULL);// no allocation error expected here.
	ASSERT_TEST(compareBest(grade_added_to_list,grade_first,grades_list)==0);
	return true;
}

static bool testCopyGrade() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, 1010, "aa", "bb"));// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,1010));//should be true.
	// "1010" logged in.
	Student student = getStudent(course_manager, 1010);
	List grades_list = getStudentGrades(student);
	ASSERT_TEST(grades_list!=NULL); // shoudln`t be NULL.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 300, 8, 48));
	Grade grade = listGetFirst(grades_list); // the grade we added to the list.
	ASSERT_TEST(grade!=NULL);// no allocation error expected here.
	Grade copied_grade = copyGrade(grade);
	// now we would like to compare the grades.
	ASSERT_TEST(compareBest(grade,copied_grade, grades_list)==0);
	return true;
}

static bool testRemoveGrade() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, 1010, "aa", "bb"));// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,1010));//should be true.
	// "1010" logged in.
	Student student = getStudent(course_manager, 1010); // the logged in student
	List grades_list = getStudentGrades(student);
	ASSERT_TEST(grades_list!=NULL); // shoudln`t be NULL.
	ASSERT_TEST(!removeGrade(course_manager, NULL, grades_list, 1,1)); //FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_NOT_LOGGED_IN); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(!removeGrade(course_manager,student, grades_list, 1,1)); //FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_COURSE_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 300, 8, 48));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 300, 8, 54));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 300, 8, 87));
	// true expected.
	// we`ve added three different grades in the same semester for the same
	// course.
	ASSERT_TEST(removeGrade(course_manager,student, grades_list,1,300)); 
	ASSERT_TEST(removeGrade(course_manager,student, grades_list,1,300)); 
	ASSERT_TEST(removeGrade(course_manager,student, grades_list,1,300)); 
	// now we expect the grades list to have no elements. we expect the
	// the output of the following function to be NULL:
	//NOTE: grades_list!=NULL.
	ASSERT_TEST(listGetFirst(grades_list)== NULL);
	return true;
}

static bool testUpdateGrade() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, 1010, "aa", "bb"));// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,1010));//should be true.
	// "1010" logged in.
	Student student = getStudent(course_manager, 1010); // the logged in student
	List grades_list = getStudentGrades(student);
	ASSERT_TEST(!updateGrade(course_manager, NULL, grades_list, 1,1)); //FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_NOT_LOGGED_IN); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(!updateGrade(course_manager, student, grades_list, 1,1));//FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_COURSE_DOES_NOT_EXIST); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 300, 8, 48));
	// true expected.
	ASSERT_TEST(!updateGrade(course_manager,student,grades_list,300,-1));//FALSE
	ASSERT_TEST(getCourseManagerError(course_manager) == 
				MTM_INVALID_PARAMETERS); // this error is expected.
	// if getCourseManagerError func failed, it will return NULL such that
	// ASSERT_TEST fails.
	int int_new_grade = 98;
	ASSERT_TEST(updateGrade(course_manager,student,grades_list,300
				,int_new_grade));
	// true expected. the grade should update to 98.
	Grade updated_grade = listGetFirst(grades_list);
	ASSERT_TEST(updated_grade != NULL); //if listGetFirst succeed, it should be
	// NULL.
	int int_updated_grade = getGrade(updated_grade);
	ASSERT_TEST(int_updated_grade==int_new_grade);// expected true
	return true;
}

static bool testGetTotalPointsX2() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, 1010, "aa", "bb"));// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,1010));//should be true.
	// "1010" logged in.
	Student student = getStudent(course_manager, 1010); // the logged in student
	List grades_list = getStudentGrades(student);
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 300, 3, 93));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 400, 8, 48));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 500, 10, 18));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 600, 1, 53));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 2, 600, 30, 53));
	// true expected.
	int total_points_X2= getTotalPointsX2(grades_list, -1); // expected "error"
	// value : -1.
	ASSERT_TEST(total_points_X2==-1);
	total_points_X2= getTotalPointsX2(grades_list, 1);// expected: 3+8+10+1=22
	ASSERT_TEST(total_points_X2==22); // for semester 1 only.
	total_points_X2= getTotalPointsX2(grades_list, 2);// expected: 30
	ASSERT_TEST(total_points_X2==30); // for semester 2 only.
	total_points_X2= getTotalPointsX2(grades_list, 0);// 
	// expected:30+3+8+10+1=52
	ASSERT_TEST(total_points_X2==52); // for BOTH semesters.
	return true;
}

static bool testGetFailedPointsX2() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, 1010, "aa", "bb"));// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,1010));//should be true.
	// "1010" logged in.
	Student student = getStudent(course_manager, 1010); // the logged in student
	List grades_list = getStudentGrades(student);
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 300, 3, 0));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 2, 400, 8, 100));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 500, 10, 0));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 600, 1, 50));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 2, 600, 30, 0));
	// true expected.
	int total_failed_points = getFailedPointsX2(grades_list, 1);
	// expected: 1
	ASSERT_TEST(total_failed_points==14);
	total_failed_points = getFailedPointsX2(grades_list, 2);
	// expected: 30
	ASSERT_TEST(total_failed_points==30);
	total_failed_points = getFailedPointsX2(grades_list, 0);
	// expected: 31
	ASSERT_TEST(total_failed_points==44);
	return true;
}

static bool testIsPassingGrade() {

	ASSERT_TEST(!isPassingGrade(46)); //46<55 expected to fail.
	ASSERT_TEST(isPassingGrade(100)); //100<55 expected to pass
	return true;
}

static bool testGetEffectivePointsX2() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, 1010, "aa", "bb"));// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,1010));//should be true.
	// "1010" logged in.
	Student student = getStudent(course_manager, 1010); // the logged in student
	List grades_list = getStudentGrades(student);
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 300, 3, 0));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 200, 8, 100));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 2, 100, 3, 0));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 200, 8, 50));
	// true expected.
	int int_effective_points = getEffectivePointsX2(grades_list,1); //effective
	//point in semester 1
	ASSERT_TEST(int_effective_points==11);
	int_effective_points = getEffectivePointsX2(grades_list,0);
	//effective points in ALL semesters
	ASSERT_TEST(int_effective_points==14);
	return true;
}

static bool testGetEffectiveGradeSumX2() {
	CourseManager course_manager = createCourseManager(NULL);
	ASSERT_TEST(course_manager != NULL); // shouldn`t be NULL.
	ASSERT_TEST(addStudent(course_manager, 1010, "aa", "bb"));// should
	// be true
	ASSERT_TEST(loginStudent(course_manager,1010));//should be true.
	// "1010" logged in.
	Student student = getStudent(course_manager, 1010); // the logged in student
	List grades_list = getStudentGrades(student);
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 300, 3, 10));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 200, 8, 100));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 2, 100, 3, 0));
	// true expected.
	ASSERT_TEST(addGrade(course_manager,student,grades_list, 1, 200, 8, 50));
	// true expected.
	int int_effective_grades = getEffectiveGradeSumX2(grades_list,1);//effective
	//grades in semester 1
	ASSERT_TEST(int_effective_grades==430);
	int_effective_grades = getEffectiveGradeSumX2(grades_list,0);
	//effective grades in ALL semesters
	ASSERT_TEST(int_effective_grades==430);
	return true;
}


int main() {
	RUN_TEST(testCreateGrade);
	RUN_TEST(testIsValidSemester);
	RUN_TEST(testIsValidPointsX2);
	RUN_TEST(testGetGrade);
	RUN_TEST(testIsValidGrade);
	RUN_TEST(testAddGrade); // NOTE: compareBest func that we use here isn`t 
	// going to be tested as it is an internal, helper-function used as a
	// function pointer for sorting purposes(in set.h). The same note for 
	// compareWorst func.
	RUN_TEST(testCopyGrade);
	RUN_TEST(testRemoveGrade);
	RUN_TEST(testUpdateGrade);
	RUN_TEST(testGetTotalPointsX2);
	RUN_TEST(testGetFailedPointsX2);
	RUN_TEST(testIsPassingGrade);
	RUN_TEST(testGetEffectivePointsX2);
	RUN_TEST(testGetEffectiveGradeSumX2);




	return 0;
}
