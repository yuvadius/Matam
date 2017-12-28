#include <stdlib.h>
#include <assert.h>
#include "set.h"
#include "student.h"

struct grade_t {
	int grade;
	char* course_id;
	char* points;
	int semester;
};