CC = gcc
OBJS = main.o course_manager.o student.o grade.o
ADTS = course_manager.o student.o grade.o
HEADERS = course_manager.h student.h grade.h list.h set.h mtm_ex3.h
EXEC = mtm_cm
DEBUG = -DNDEBUG
CFLAGS = -std=c99 -Wall -Werror -pedantic-errors $(DEBUG)
LINK = -L. -lmtm
TESTS = course_manager_test student_test grade_test
O_FILES = course_manager_test.o student_test.o grade_test.o $(OBJS)
TEST_PATH=./tests/

$(EXEC) : $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $(OBJS) $(LINK) -o $@
	
tests : $(TESTS)

course_manager.o : course_manager.c $(HEADERS)
 
student.o : student.c $(HEADERS)
	
grade.o : student.c $(HEADERS)
	
main.o : main.c $(HEADERS)

course_manager_test : course_manager_test.o $(ADTS)
	$(CC) $(CFLAGS) course_manager_test.o $(ADTS) $(DEBUG) $(LINK) -o $@
	
student_test : student_test.o $(ADTS)
	$(CC) $(CFLAGS) student_test.o $(ADTS) $(DEBUG) $(LINK) -o $@
	
grade_test : grade_test.o $(ADTS)
	$(CC) $(CFLAGS) grade_test.o $(ADTS) $(DEBUG) $(LINK) -o $@

course_manager_test.o : $(TEST_PATH)course_manager_test.c $(TEST_PATH)test_utilities.h $(HEADERS)

student_test.o : $(TEST_PATH)student_test.c $(TEST_PATH)test_utilities.h $(HEADERS)

grade_test.o : $(TEST_PATH)grade_test.c $(TEST_PATH)grade_test.h $(HEADERS)

clean:
	rm -f $(TESTS) $(O_FILES) $(EXEC)