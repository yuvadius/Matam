#include "testMacros.h"
#include "../World.h"
#include "../exceptions.h"

using namespace mtm;

bool testWorldConstructor() {
	ASSERT_NO_EXCEPTION(World());
	return true;
}

bool testWorldAddClan() {
	World world;
	ASSERT_EXCEPTION(world.addClan(""), WorldInvalidArgument);
	ASSERT_NO_EXCEPTION(world.addClan("Beta"));
	ASSERT_EXCEPTION(world.addClan("Beta"), WorldClanNameIsTaken);
	ASSERT_NO_EXCEPTION(world.addClan("Gamma"));
	ASSERT_NO_EXCEPTION(world.addClan("Lambda"));
	ASSERT_NO_EXCEPTION(world.addArea("Carmel", MOUNTAIN));
	ASSERT_NO_EXCEPTION(world.addGroup("Alpha","Gamma", 10, 10, "Carmel"));
	ostringstream os;
	ASSERT_NO_EXCEPTION(world.printGroup(os, "Alpha"));
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
		"Group's clan: Gamma\n"
		"Group's children: 10\n"
		"Group's adults: 10\n"
		"Group's tools: 40\n"
		"Group's food: 50\n"
		"Group's morale: 77\n"
		"Group's current area: Carmel\n"));
	ASSERT_NO_EXCEPTION(world.printClan(os, "Gamma"));
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: Gamma\n"
		"Clan's groups:\n"
		"Alpha\n"));
	ASSERT_NO_EXCEPTION(world.printClan(os, "Lambda"));
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: Lambda\n"
		"Clan's groups:\n"));

	return true;
}

bool testWorldAddArea() {
	World world;
	ASSERT_EXCEPTION(world.addClan(""), WorldInvalidArgument);
	ASSERT_NO_EXCEPTION(world.addClan("Beta"));
	ASSERT_EXCEPTION(world.addClan("Beta"), WorldClanNameIsTaken);
	ASSERT_NO_EXCEPTION(world.addClan("Gamma"));
	ASSERT_NO_EXCEPTION(world.addClan("Lambda"));
	ASSERT_EXCEPTION(world.addArea("", MOUNTAIN), WorldInvalidArgument);
	ASSERT_NO_EXCEPTION(world.addArea("Carmel", MOUNTAIN));
	ASSERT_EXCEPTION(world.addArea("Carmel", RIVER), WorldAreaNameIsTaken);
	ASSERT_NO_EXCEPTION(world.addGroup("Alpha","Gamma", 10, 10, "Carmel"));
	ostringstream os;
	ASSERT_NO_EXCEPTION(world.printGroup(os, "Alpha"));
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
		"Group's clan: Gamma\n"
		"Group's children: 10\n"
		"Group's adults: 10\n"
		"Group's tools: 40\n"
		"Group's food: 50\n"
		"Group's morale: 77\n"
		"Group's current area: Carmel\n"));
	ASSERT_NO_EXCEPTION(world.printClan(os, "Gamma"));
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: Gamma\n"
		"Clan's groups:\n"
		"Alpha\n"));
	ASSERT_NO_EXCEPTION(world.printClan(os, "Lambda"));
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: Lambda\n"
		"Clan's groups:\n"));

	return true;
}

bool testWorldAddGroup() {
	World world;
	ASSERT_EXCEPTION(world.addClan(""), WorldInvalidArgument);
	ASSERT_NO_EXCEPTION(world.addClan("B"));
	ASSERT_EXCEPTION(world.addClan("B"), WorldClanNameIsTaken);
	ASSERT_NO_EXCEPTION(world.addClan("G"));
	ASSERT_NO_EXCEPTION(world.addClan("L"));
	ASSERT_EXCEPTION(world.addArea("", MOUNTAIN), WorldInvalidArgument);
	ASSERT_NO_EXCEPTION(world.addArea("C", MOUNTAIN));
	ASSERT_NO_EXCEPTION(world.addGroup("A","G", 10, 10, "C"));
	ASSERT_EXCEPTION(world.addArea("C", PLAIN), WorldAreaNameIsTaken);
	ASSERT_EXCEPTION(world.addGroup("","G", 5, 6, "C"), WorldInvalidArgument);
	ASSERT_EXCEPTION(world.addGroup("A","G", -5, 6, "C"), WorldInvalidArgument);
	ASSERT_EXCEPTION(world.addGroup("A","G", 5, -6, "C"), WorldInvalidArgument);
	ASSERT_EXCEPTION(world.addGroup("A","G", 0, 0, "C"), WorldInvalidArgument);
	ASSERT_EXCEPTION(world.addGroup("A","G", -5, 6, "C"), WorldInvalidArgument);
	ASSERT_EXCEPTION(world.addGroup("A","G", 1, 6,"C"), WorldGroupNameIsTaken);
	ASSERT_EXCEPTION(world.addGroup("D","Z", 1, 6,"C"), WorldClanNotFound);
	ASSERT_EXCEPTION(world.addGroup("D","G", 1, 6,"Y"), WorldAreaNotFound);
	ostringstream os;
	ASSERT_NO_EXCEPTION(world.printGroup(os, "A"));
	ASSERT_TRUE(VerifyOutput(os, "Group's name: A\n"
		"Group's clan: G\n"
		"Group's children: 10\n"
		"Group's adults: 10\n"
		"Group's tools: 40\n"
		"Group's food: 50\n"
		"Group's morale: 77\n"
		"Group's current area: C\n"));
	ASSERT_NO_EXCEPTION(world.printClan(os, "G"));
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: G\n"
		"Clan's groups:\n"
		"A\n"));
	ASSERT_NO_EXCEPTION(world.printClan(os, "L"));
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: L\n"
		"Clan's groups:\n"));

	return true;
}

bool testWorldMakeReachable() {
	World world;
	ASSERT_NO_EXCEPTION(world.addClan("B"));
	ASSERT_NO_EXCEPTION(world.addClan("G"));
	ASSERT_NO_EXCEPTION(world.addClan("L"));
	ASSERT_NO_EXCEPTION(world.addArea("C", MOUNTAIN));
	ASSERT_NO_EXCEPTION(world.addArea("T", PLAIN));
	ASSERT_NO_EXCEPTION(world.addGroup("A","G", 5, 4, "C"));
	ostringstream os;
	ASSERT_NO_EXCEPTION(world.printGroup(os, "A"));
	ASSERT_TRUE(VerifyOutput(os, "Group's name: A\n"
		"Group's clan: G\n"
		"Group's children: 5\n"
		"Group's adults: 4\n"
		"Group's tools: 16\n"
		"Group's food: 22\n"
		"Group's morale: 77\n"
		"Group's current area: C\n"));
	ASSERT_EXCEPTION(world.makeReachable("Cds", "Twde"), WorldAreaNotFound);
	ASSERT_NO_EXCEPTION(world.makeReachable("C", "T"));
	ASSERT_NO_EXCEPTION(world.moveGroup("A", "T"));
	ASSERT_NO_EXCEPTION(world.printGroup(os, "A"));
	ASSERT_TRUE(VerifyOutput(os, "Group's name: A\n"
		"Group's clan: G\n"
		"Group's children: 5\n"
		"Group's adults: 4\n"
		"Group's tools: 16\n"
		"Group's food: 22\n"
		"Group's morale: 77\n"
		"Group's current area: T\n"));
	ASSERT_NO_EXCEPTION(world.printClan(os, "G"));
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: G\n"
		"Clan's groups:\n"
		"A\n"));
	ASSERT_NO_EXCEPTION(world.printClan(os, "B"));
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: B\n"
		"Clan's groups:\n"));

	return true;
}

bool testWorldMoveGroup() {
	World world;
	ASSERT_NO_EXCEPTION(world.addClan("B"));
	ASSERT_NO_EXCEPTION(world.addClan("G"));
	ASSERT_NO_EXCEPTION(world.addClan("L"));
	ASSERT_NO_EXCEPTION(world.addArea("C", RIVER));
	ASSERT_NO_EXCEPTION(world.addArea("T", MOUNTAIN));
	ASSERT_NO_EXCEPTION(world.addArea("W", PLAIN));
	ASSERT_NO_EXCEPTION(world.addGroup("A","G", 5, 4, "C"));
	ostringstream os;
	ASSERT_NO_EXCEPTION(world.printGroup(os, "A"));
	ASSERT_TRUE(VerifyOutput(os, "Group's name: A\n"
		"Group's clan: G\n"
		"Group's children: 5\n"
		"Group's adults: 4\n"
		"Group's tools: 16\n"
		"Group's food: 22\n"
		"Group's morale: 77\n"
		"Group's current area: C\n"));
	ASSERT_EXCEPTION(world.makeReachable("Cds", "Twde"), WorldAreaNotFound);
	ASSERT_NO_EXCEPTION(world.makeReachable("C", "T"));
	ASSERT_NO_EXCEPTION(world.moveGroup("A", "T"));
	ASSERT_NO_EXCEPTION(world.printGroup(os, "A"));
	ASSERT_TRUE(VerifyOutput(os, "Group's name: A\n"
		"Group's clan: G\n"
		"Group's children: 5\n"
		"Group's adults: 4\n"
		"Group's tools: 16\n"
		"Group's food: 22\n"
		"Group's morale: 77\n"
		"Group's current area: T\n"));
	ASSERT_NO_EXCEPTION(world.makeReachable("T", "C"));
	ASSERT_NO_EXCEPTION(world.moveGroup("A", "C"));
	ASSERT_NO_EXCEPTION(world.printGroup(os, "A"));
	ASSERT_TRUE(VerifyOutput(os, "Group's name: A\n"
		"Group's clan: G\n"
		"Group's children: 5\n"
		"Group's adults: 4\n"
		"Group's tools: 16\n"
		"Group's food: 22\n"
		"Group's morale: 77\n"
		"Group's current area: C\n"));
	ASSERT_EXCEPTION(world.moveGroup("", "C"), WorldGroupNotFound);
	ASSERT_EXCEPTION(world.moveGroup("A", ""), WorldAreaNotFound);
	ASSERT_EXCEPTION(world.moveGroup("A", "C"), WorldGroupAlreadyInArea);
	ASSERT_EXCEPTION(world.moveGroup("A", "W"), WorldAreaNotReachable);
	return true;
}

bool testWorldMakeFriends() {
	World world;
	ASSERT_NO_EXCEPTION(world.addClan("B"));
	ASSERT_EXCEPTION(world.makeFriends("B", "G"), WorldClanNotFound);
	ASSERT_NO_EXCEPTION(world.addClan("G"));
	ASSERT_NO_EXCEPTION(world.makeFriends("B", "G"));
	return true;
}

bool testWorldUniteClans() {
	World world;
	ASSERT_NO_EXCEPTION(world.addClan("B"));
	ASSERT_NO_EXCEPTION(world.addClan("G"));
	ASSERT_NO_EXCEPTION(world.addClan("O"));
	ASSERT_NO_EXCEPTION(world.addArea("C", RIVER));
	ASSERT_NO_EXCEPTION(world.addArea("T", MOUNTAIN));
	ASSERT_NO_EXCEPTION(world.addArea("W", PLAIN));
	ASSERT_NO_EXCEPTION(world.addGroup("A","B", 5, 4, "C"));
	ASSERT_NO_EXCEPTION(world.addGroup("Z","G", 15, 14, "W"));
	ostringstream os;
	ASSERT_NO_EXCEPTION(world.printClan(os, "B"));
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: B\n"
		"Clan's groups:\n"
		"A\n"));
	ASSERT_NO_EXCEPTION(world.printClan(os, "G"));
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: G\n"
		"Clan's groups:\n"
		"Z\n"));
	ASSERT_EXCEPTION(world.uniteClans("B", "G", ""), WorldInvalidArgument);
	ASSERT_EXCEPTION(world.uniteClans("B", "G", "O"), WorldClanNameIsTaken);
	ASSERT_EXCEPTION(world.uniteClans("B", "E", "O"), WorldClanNameIsTaken);
	ASSERT_NO_EXCEPTION(world.uniteClans("B", "G", "G"));
	ASSERT_NO_EXCEPTION(world.printClan(os, "G"));
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: G\n"
		"Clan's groups:\n"
		"Z\n"
		"A\n"));
	return true;
}

bool testWorldPrintGroup(){
    World w;
    ASSERT_NO_EXCEPTION(w.addClan("Beta"));
    ASSERT_NO_EXCEPTION(w.addArea("Tel-Aviv", PLAIN));
    ASSERT_NO_EXCEPTION(w.addArea("Carmel", MOUNTAIN));
    ASSERT_NO_EXCEPTION(w.makeReachable("Carmel", "Tel-Aviv"));
    ASSERT_NO_EXCEPTION(w.addGroup("Alpha","Beta", 10, 10, "Carmel"));
    ASSERT_NO_EXCEPTION(w.moveGroup("Alpha", "Tel-Aviv")); //divides
    ostringstream os;
    ASSERT_NO_EXCEPTION(w.printGroup(os, "Alpha"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
        "Group's clan: Beta\n"
        "Group's children: 5\n"
        "Group's adults: 5\n"
        "Group's tools: 20\n"
        "Group's food: 25\n"
        "Group's morale: 77\n"
        "Group's current area: Tel-Aviv\n"));
    return true;
}

bool testWorldPrintClan(){
    World w;
    ASSERT_NO_EXCEPTION(w.addClan("Beta"));
    ASSERT_NO_EXCEPTION(w.addArea("Tel-Aviv", PLAIN));
    ASSERT_NO_EXCEPTION(w.addArea("Carmel", MOUNTAIN));
    ASSERT_NO_EXCEPTION(w.makeReachable("Carmel", "Tel-Aviv"));
    ASSERT_NO_EXCEPTION(w.addGroup("Alpha","Beta", 10, 10, "Carmel"));
    ASSERT_NO_EXCEPTION(w.moveGroup("Alpha", "Tel-Aviv")); //divides
    ostringstream os;
    ASSERT_NO_EXCEPTION(w.printClan(os, "Beta"));
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: Beta\n"
    	"Clan's groups:\n"
        "Alpha\n"));
    return true;
}

int main(){
	RUN_TEST(testWorldConstructor);
	RUN_TEST(testWorldAddClan);
	RUN_TEST(testWorldAddArea);
	RUN_TEST(testWorldAddGroup);
	RUN_TEST(testWorldMakeReachable);
	RUN_TEST(testWorldMoveGroup);
	RUN_TEST(testWorldMakeFriends);
	RUN_TEST(testWorldUniteClans);
	RUN_TEST(testWorldPrintGroup);
	RUN_TEST(testWorldPrintClan);
    return 0;
}
