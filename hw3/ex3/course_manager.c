#include <stdlib.h>
#include <assert.h>
#include "set.h"
#include "student.h"

/** The List is implemented as a List of Elements.
* With iterator as an index to the current Element or NULL
*/
struct course_manager_t {
	Set students;
	Student current_student;
	MtmErrorCode error;
};

static FacultyRequest GetFacultyRequst(char* request) {
	
}