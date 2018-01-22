SRCS = Area.cpp Clan.cpp Group.cpp Mountain.cpp Plain.cpp River.cpp World.cpp
COMMAND = g++ -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG
TESTS = AreasTest ClanTest GroupTest MtmSetTest WorldTest
O_FILES = AreasTest.o ClanTest.o GroupTest.o MtmSetTest.o WorldTest.o
TEST_PATH=./test/
	
tests : $(TESTS)

AreasTest :
	$(COMMAND) ./test/AreasTest.cpp $(SRCS) -o AreasTest
	
ClanTest :
	$(COMMAND) ./test/ClanTest.cpp $(SRCS) -o ClanTest
	
GroupTest :
	$(COMMAND) ./test/GroupTest.cpp $(SRCS) -o GroupTest

MtmSetTest :
	$(COMMAND) ./test/MtmSetTest.cpp $(SRCS) -o MtmSetTest

WorldTest :
	$(COMMAND) ./test/WorldTest.cpp $(SRCS) -o WorldTest

clean:
	rm -f $(TESTS) $(O_FILES)