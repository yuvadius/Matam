#include <stdlib.h>
#include <assert.h>
#include "set.h"
#include "student.h"

struct course_manager_t {
	Set students;
	Student current_student;
	MtmErrorCode error;
};

static FacultyRequest getFacultyRequst(char* request) {
	
}