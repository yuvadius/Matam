TEST_PATH=./tests/
CC = gcc
ADTS = user.o series.o
OBJS = $(TEST_PATH)mtmFlixTestEx.o mtmflix.o util.o $(ADTS)
ADT_SRCS = user.c series.c
SRCS = $(TEST_PATH)mtmFlixTestEx.c mtmflix.c util.c $(ADT_SRCS)
HEADERS = user.h series.h mtmflix.h util.h mtm_ex3.h list.h set.h map.h $(TEST_PATH)test_utilities.h
EXEC = mtmflix
DEBUG = -DNDEBUG
CFLAGS = -std=c99 -Wall -Werror -pedantic-errors $(DEBUG)
LINK = -L. -lmtm
O_FILES = $(OBJS)

$(EXEC) : $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $(SRCS) mtm_ex3.o $(LINK) -o $@

mtmFlixTestEx.o : $(TEST_PATH)mtmFlixTestEx.c $(HEADERS)
 
mtmflix.o : mtmflix.c $(HEADERS)
	
util.o : util.c $(HEADERS)
	
user.o : user.c $(HEADERS)

series.o : series.c $(HEADERS)

clean:
	rm -f $(O_FILES) $(EXEC)