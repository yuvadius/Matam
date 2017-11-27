#include <stdio.h>

#include "Course.h"
#include "CourseSystem.h"

#define ASSERT(test_number, test_condition)  \
   if (!(test_condition)) {printf("\nTEST %s FAILED", test_number); } \
   else printf("\nTEST %s OK", test_number);


int main(int argc, char **argv)
{

    CourseSystem sys=NULL;
    CourseResult rc = COURSE_OK;

    sys=createSystem("Technion_2017_2018");
    ASSERT("1.1" , sys!=NULL)

    Course c1;
    rc = createCourse("234114", "Introduction to Computer Science", -4, &c1);
    ASSERT("1.2" , rc==COURSE_ILLEGAL_PARAMETER)
    rc = createCourse("234114", "Introduction to Computer Science", 4, &c1);
    ASSERT("1.3" , rc==COURSE_OK)
    destroyCourse(c1);
    destroySystem(sys);
    return 0;
}
