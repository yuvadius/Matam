#include "Course.h"
#include "CourseSystem.h"
#include <stdio.h>

#define ASSERT(test_number, test_condition)  \
   if (!(test_condition)) {printf("\nTEST %s FAILED", test_number); } \
   else printf("\nTEST %s OK", test_number);

int main() {
    //variables
    CourseSystem sys = NULL;
    CourseResult rc;
    Course c1, c2, c3, c4, c5, c6, c7; // "c" stands for "course"
    createCourse("234122", "Matam", 4, &c1);
    createCourse("234141", "Combi", 4, &c2);
    createCourse("234114", "Mavo", 3, &c3);
    createCourse("236315", "Algebric Methods", 3, &c4);

    //check - was the course successfully created?
    rc = createCourse("104032", "Infi 2m", 3, &c5);
    ASSERT("2.01", rc == COURSE_OK);

    //check if negative credits can be inserted
    rc = createCourse("856432", "bad course", -5, &c6);
    ASSERT("2.02", rc == COURSE_ILLEGAL_PARAMETER);

    createCourse("104032", "Infi 2", 5, &c7);
    sys=createSystem("Technion_2017_2018");

    printf("\n");
    displaySystem(sys);//should be: Technion_2017_2018

    int is_equal = coursesEqualId(c5, c7);

    //checking if the courses are equal
    ASSERT("2.03", is_equal == 1);

    is_equal = coursesEqualId(c1, c7);
    //check NOT equal courses IDs
    ASSERT("2.04", is_equal == 0);

    int is_less_than = courseLessThan(c1, c2);

    //test 2.05 : check 1 is less than 2
    ASSERT("2.05", is_less_than == 1);

    is_less_than = courseLessThan(c5, c7);

    //test 2.06 : check ID is the same
    ASSERT("2.06", is_less_than == 0);

    is_less_than = courseLessThan(c2, c1);

    //test 2.07 : check 2 is less than 1
    ASSERT("2.07", is_less_than == 0);

    rc = courseUpdateName(c7, "Infi new");

    //test 2.08 : check good name changing
    ASSERT("2.08", rc == COURSE_OK);

    /*Display 2
     expected output :
     104032 Infi new 5.0*/
    printf("\n");
    displayCourse(c7);

    rc = addPreCourse(c1, c1);

    //test 2.09 : check adding course as a pre to itself
    ASSERT("2.09", rc == COURSE_THE_SAME_COURSE);

    //test 2.1 : check empty system
    int system_len = sysNumCourses(sys);
    ASSERT("2.1", system_len == 0);

    SysResult sys_result;

    //System data
    sysAddCourse(sys, c2);
    sysAddCourse(sys, c3);
    sysAddCourse(sys, c4);
    sysAddCourse(sys, c5);

    //test 2.11 : check add existing course ID
    sys_result = sysAddCourse(sys, c7);
    ASSERT("2.11", sys_result == SYS_COURSE_ALREADY_EXIST);

    //test 2.12 : check adding valid course
    sys_result = sysAddCourse(sys, c1);
    ASSERT("2.12", sys_result == SYS_OK);

    /*Display 3
    expected output :
    Technion_2017_2018
    104032 Infi 2m 3.0
    234114 Mavo 3.0
    234122 Matam 4.0
    234141 Combi 4.0
    236315 Algebric Methods 3.0*/
    printf("\n");
    displaySystem(sys);

    //test 2.13 : check num of courses with 5 courses
    system_len = sysNumCourses(sys);
    ASSERT("2.13", system_len == 5);

    //test 2.14 : check add pre course to NOT existing course
    sys_result = sysAddPreCourse(sys, "111222", "234114");
    ASSERT("2.14", sys_result == SYS_NOT_IN_SYSTEM);

    //test 2.15 : check NOT existing pre course
    sys_result = sysAddPreCourse(sys, "234114", "111222");
    ASSERT("2.15", sys_result == SYS_NOT_IN_SYSTEM);

    //add data
    sysAddPreCourse(sys, "234122", "234141");
    sysAddPreCourse(sys, "234122", "234114");
    sysAddPreCourse(sys, "234122", "236315");
    sysAddPreCourse(sys, "234122", "104032");
    sysAddPreCourse(sys, "234114", "234141");
    sysAddPreCourse(sys, "234114", "236315");
    sysAddPreCourse(sys, "234114", "104032");

    //test 2.16 : check pre course added ok
    sys_result = sysAddPreCourse(sys, "236315", "104032");
    ASSERT("2.16", sys_result == SYS_OK);

    //test 2.17 : check trying to add an existing pre-course
    sys_result = sysAddPreCourse(sys, "234122", "236315");
    ASSERT("2.17", sys_result == SYS_ALREADY_PRE_COURSE);

    /*Display 4
    expected output :
     Technion_2017_2018
    104032 Infi 2m 3.0
    234114 Mavo 3.0 104032 234141 236315
    234122 Matam 4.0 104032 234114 234141 236315
    234141 Combi 4.0
    236315 Algebric Methods 3.0 104032 */
    printf("\n");
    displaySystem(sys);

    //test 2.18 : check valid pre course removal
    sys_result = sysRemovePreCourse(sys, "234122", "236315");
    ASSERT("2.18", sys_result == SYS_OK);

    //test 2.19 : check invalid pre course removal (NOT a pre course)
    sys_result = sysRemovePreCourse(sys, "234122", "236315");
    ASSERT("2.19", sys_result == SYS_NOT_PRE_COURSE);

    /*Display 5
    expected output :
    234122 Matam 4.0 104032 234114 234141*/
    printf("\n");
    displayCourse(c1);

    //test 2.2 : trying to remove a pre course from a course that
    //doesn`t exist
    sys_result = sysRemovePreCourse(sys, "232323", "236315");
    ASSERT("2.2", sys_result == SYS_NOT_IN_SYSTEM);

    //test 2.21 : update name for a NOT existing course
    sys_result = sysUpdateCourseName(sys, "131313", "heyheyhey");
    ASSERT("2.21", sys_result == SYS_NOT_IN_SYSTEM);

    //test 2.22 : update name for an existing course
    sys_result = sysUpdateCourseName(sys, "234122", "Systems programming");
    ASSERT("2.22", sys_result == SYS_OK);

    /*Display 6
    expected output :
    Technion_2017_2018
    104032 Infi 2m 3.0
    234114 Mavo 3.0 104032 234141 236315
    234122 Systems programming 4.0 104032 234114 234141
    234141 Combi 4.0
    236315 Algebric Methods 3.0 104032 */
    printf("\n");
    displaySystem(sys);

    int ans;

    //test 2.23 : trying to ask if NOT existing course is a pre course
    sys_result = sysIsPreCourse(sys, "234122", "454545", &ans);
    ASSERT("2.23", sys_result == SYS_NOT_IN_SYSTEM);

    //test 2.24,2.25 : valid isPreCourse request (pre course)
    sys_result = sysIsPreCourse(sys, "234122", "234114", &ans);
    ASSERT("2.24", sys_result == SYS_OK);
    ASSERT("2.25", ans == 1);

    //test 2.26,2.27 : valid isPreCourse request (NOT pre course)
    sys_result = sysIsPreCourse(sys, "234122", "236315", &ans);
    ASSERT("2.26", sys_result == SYS_OK);
    ASSERT("2.27", ans == 0);



    //destroy data
    destroySystem(sys);

    return 0;
}
