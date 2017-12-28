#include <stdlib.h>
#include <assert.h>
#include "set.h"
#include "student.h"

struct student_t {
	Set freinds;
	Set studentRequests;
	int id;
	char* first_name;
	char* last_name;
	List grades;
};