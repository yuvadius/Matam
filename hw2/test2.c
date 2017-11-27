#include "Course.h"
#include "CourseSystem.h"
#include <stdio.h>

#define ASSERT(test_number, test_condition)  \
   if (!(test_condition)) {printf("\nTEST %s FAILED", test_number); } \
   else printf("\nTEST %s OK", test_number);

int main() {
    //Variables and data
    CourseSystem sys = NULL;
    CourseResult rc;
    Course c1, c2, c3, c4, c5, c6, c7;
    createCourse("234122", "Introduction to systems programming", 4, &c1);
    createCourse("234141", "Combinatorics", 4, &c2);
    createCourse("234114", "Introduction to Computer Science", 3, &c3);
    createCourse("104134", "Modern algebra", 2.5, &c4);

    //check if course was created successfully
    rc = createCourse("104032", "Infi 2m", 3, &c5);
    ASSERT("1", rc == COURSE_OK);

    //check if negative credits can be inserted
    rc = createCourse("856432", "bad course", -5, &c6);
    ASSERT("2", rc == COURSE_ILLEGAL_PARAMETER);

    createCourse("104032", "Infi 2", 5, &c7);
    sys=createSystem("Technion_2017_2018");

    printf("\n");
    displaySystem(sys);//should be: Technion_2017_2018

    int is_equal = coursesEqualId(c5, c7);

    //check if the courses are equal
    ASSERT("3", is_equal == 1);

    is_equal = coursesEqualId(c1, c7);
    //check not equal course IDs
    ASSERT("4", is_equal == 0);

    int is_less_than = courseLessThan(c1, c2);

    //test 5 : check 1 is less than 2
    ASSERT("5", is_less_than == 1);

    is_less_than = courseLessThan(c5, c7);

    //test 6 : check ID is the same
    ASSERT("6", is_less_than == 0);

    is_less_than = courseLessThan(c2, c1);

    //test 7 : check 2 is less than 1
    ASSERT("7", is_less_than == 0);

    rc = courseUpdateName(c7, "Infi new");

    //test 8 : check good name changing
    ASSERT("8", rc == COURSE_OK);

    /*Display 2
     expected output :
     104032 Infi new 5.0*/
    printf("\n");
    displayCourse(c7);

    rc = addPreCourse(c1, c1);

    //test 9 : check adding course as a pre to itself
    ASSERT("9", rc == COURSE_THE_SAME_COURSE);

    //test 10 : check empty system
    int system_len = sysNumCourses(sys);
    ASSERT("10", system_len == 0);

    SysResult sys_result;

    //System data
    sysAddCourse(sys, c2);
    sysAddCourse(sys, c3);
    sysAddCourse(sys, c4);
    sysAddCourse(sys, c5);

    //test 11 : check add existing course ID
    sys_result = sysAddCourse(sys, c7);
    ASSERT("11", sys_result == SYS_COURSE_ALREADY_EXIST);

    //test 12 : check adding valid course
    sys_result = sysAddCourse(sys, c1);
    ASSERT("12", sys_result == SYS_OK);

    /*Display 3
    expected output :
    Technion_2017_2018
    104032 Infi 2m 3.0
    104134 Modern algebra 2.5
    234114 Introduction to Computer Science 3.0
    234122 Introduction to systems programming 4.0
    234141 Combinatorics 4.0*/
    printf("\n");
    displaySystem(sys);

    //test 13 : check num of courses with 5 courses
    system_len = sysNumCourses(sys);
    ASSERT("13", system_len == 5);

    //test 14 : check add pre course to not exist course
    sys_result = sysAddPreCourse(sys, "111222", "234114");
    ASSERT("14", sys_result == SYS_NOT_IN_SYSTEM);

    //test 15 : check not exist pre course
    sys_result = sysAddPreCourse(sys, "234114", "111222");
    ASSERT("15", sys_result == SYS_NOT_IN_SYSTEM);

    //add data
    sysAddPreCourse(sys, "234122", "234141");
    sysAddPreCourse(sys, "234122", "234114");
    sysAddPreCourse(sys, "234122", "104134");
    sysAddPreCourse(sys, "234122", "104032");
    sysAddPreCourse(sys, "234114", "234141");
    sysAddPreCourse(sys, "234114", "104134");
    sysAddPreCourse(sys, "234114", "104032");

    //test 16 : check pre course added ok
    sys_result = sysAddPreCourse(sys, "104134", "104032");
    ASSERT("16", sys_result == SYS_OK);

    //test 17 : check trying to add an existing pre-course
    sys_result = sysAddPreCourse(sys, "234122", "104134");
    ASSERT("17", sys_result == SYS_ALREADY_PRE_COURSE);

    /*Display 4
    expected output :
     Technion_2017_2018
    104032 Infi 2m 3.0
    104134 Modern algebra 2.5 104032
    234114 Introduction to Computer Science 3.0 104032 104134 234141
    234122 Introduction to systems programming 4.0 104032 104134 234114 234141
    234141 Combinatorics 4.0*/
    printf("\n");
    displaySystem(sys);

    //test 18 : check valid pre course removal
    sys_result = sysRemovePreCourse(sys, "234122", "104134");
    ASSERT("18", sys_result == SYS_OK);

    //test 19 : check invalid pre course removal (not a pre course)
    sys_result = sysRemovePreCourse(sys, "234122", "104134");
    ASSERT("19", sys_result == SYS_NOT_PRE_COURSE);

    /*Display 5
    expected output :
    234122 Introduction to systems programming 4.0 104032 234114 234141*/
    printf("\n");
    displayCourse(c1);

    //test 20 : check trying to remove a pre course from not existing course
    sys_result = sysRemovePreCourse(sys, "232323", "104134");
    ASSERT("20", sys_result == SYS_NOT_IN_SYSTEM);

    //test 21 : update name for not existing course
    sys_result = sysUpdateCourseName(sys, "131313", "heyheyhey");
    ASSERT("21", sys_result == SYS_NOT_IN_SYSTEM);

    //test 22 : update name for existing course
    sys_result = sysUpdateCourseName(sys, "234122", "Systems programming");
    ASSERT("22", sys_result == SYS_OK);

    /*Display 6
    expected output :
    Technion_2017_2018
    104032 Infi 2m 3.0
    104134 Modern algebra 2.5 104032
    234114 Introduction to Computer Science 3.0 104032 104134 234141
    234122 Systems programming 4.0 104032 234114 234141
    234141 Combinatorics 4.0*/
    printf("\n");
    displaySystem(sys);

    int ans;

    //test 23 : trying to ask if not existing course is a pre course
    sys_result = sysIsPreCourse(sys, "234122", "454545", &ans);
    ASSERT("23", sys_result == SYS_NOT_IN_SYSTEM);

    //test 24,25 : valid isPreCourse request (pre course)
    sys_result = sysIsPreCourse(sys, "234122", "234114", &ans);
    ASSERT("24", sys_result == SYS_OK);
    ASSERT("25", ans == 1);

    //test 26,27 : valid isPreCourse request (not pre course)
    sys_result = sysIsPreCourse(sys, "234122", "104134", &ans);
    ASSERT("26", sys_result == SYS_OK);
    ASSERT("27", ans == 0);

    //test 28 : testing remove course which is also pre course for others
    sysRemoveCourse(sys, "104032");
    ASSERT("28", sys_result == SYS_OK);

    /*Display 7
    expected output :
    Technion_2017_2018
    104134 Modern algebra 2.5
    234114 Introduction to Computer Science 3.0 104134 234141
    234122 Systems programming 4.0 234114 234141
    234141 Combinatorics 4.0*/
    printf("\n");
    displaySystem(sys);

    //destroy data
    destroyCourse(c7);
    destroySystem(sys);

    return 0;
}
