CC = gcc
ADTS = course_manager.o student.o grade.o
OBJS = main.o $(ADTS)
ADT_SRCS = course_manager.c student.c grade.c
SRCS = main.c $(ADT_SRCS)
HEADERS = course_manager.h student.h grade.h list.h set.h mtm_ex3.h
EXEC = mtm_cm
DEBUG = -DNDEBUG
CFLAGS = -std=c99 -Wall -Werror -pedantic-errors $(DEBUG)
LINK = -L. -lmtm
TESTS = course_manager_test student_test grade_test list_mtm_test
O_FILES = course_manager_test.o student_test.o grade_test.o list_mtm_test.o $(OBJS)
TEST_PATH=./tests/
LIST_MTM_PATH=./list_mtm/

$(EXEC) : $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $(SRCS) $(LINK) -o $@
	
tests : $(TESTS)

course_manager.o : course_manager.c $(HEADERS)
 
student.o : student.c $(HEADERS)
	
grade.o : student.c $(HEADERS)
	
main.o : main.c $(HEADERS)

course_manager_test : course_manager_test.o $(ADTS)
	$(CC) $(CFLAGS) $(DEBUG) $(TEST_PATH)course_manager_test.c $(ADT_SRCS) $(LINK) -o $@
	
student_test : student_test.o $(ADTS)
	$(CC) $(CFLAGS) $(DEBUG) $(TEST_PATH)student_test.c $(ADT_SRCS) $(LINK) -o $@
	
grade_test : grade_test.o $(ADTS)
	$(CC) $(CFLAGS) $(DEBUG) $(TEST_PATH)grade_test.c $(ADT_SRCS) $(LINK) -o $@

list_mtm_test : list_mtm_test.o
	$(CC) $(CFLAGS) $(DEBUG) $(TEST_PATH)list_mtm_test.c $(LIST_MTM_PATH)list_mtm.c $(LINK) -o $@

course_manager_test.o : $(TEST_PATH)course_manager_test.c $(TEST_PATH)test_utilities.h $(HEADERS)

student_test.o : $(TEST_PATH)student_test.c $(TEST_PATH)test_utilities.h $(HEADERS)

grade_test.o : $(TEST_PATH)grade_test.c $(TEST_PATH)test_utilities.h $(HEADERS)

list_mtm_test.o : $(TEST_PATH)list_mtm_test.c $(TEST_PATH)test_utilities.h $(LIST_MTM_PATH)list_mtm.h

clean:
	rm -f $(TESTS) $(O_FILES) $(EXEC)