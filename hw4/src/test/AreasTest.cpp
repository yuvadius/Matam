#include "testMacros.h"
#include "../Group.h"
#include "../Clan.h"
#include "../Area.h"
#include "../Plain.h"
#include "../Mountain.h"
#include "../River.h"
//#include "../World.h"
#include "../exceptions.h"

using namespace mtm;

std::map<std::string, Clan> makeClanMap() {
    std::map<std::string, Clan> clan_map;
    clan_map.insert(std::pair<std::string, Clan>("Beta", Clan("Beta")));
    clan_map.insert(std::pair<std::string, Clan>("Gamma", Clan("Gamma")));
    clan_map.at("Beta").addGroup(Group("Alpha1", 10, 10));
    clan_map.at("Beta").addGroup(Group("Alpha2","", 10, 10, 40, 0, 70));
    clan_map.at("Gamma").addGroup(Group("Lambda1", 10, 10));
    clan_map.at("Gamma").addGroup(Group("Lambda2", 5, 5));
    clan_map.at("Gamma").addGroup(Group("Lambda3", 100, 100));
    return clan_map;
}

bool testPlainConstructor() {
	ASSERT_EXCEPTION(Plain(""), AreaInvalidArguments);
	ASSERT_NO_EXCEPTION(Plain("yo"));
	return true;
}

bool testPlainGroupArrive() {
    Area* tel_aviv(new Plain("Tel-Aviv"));
    std::map<std::string, Clan> clan_map = makeClanMap();
    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Alpha1", "Beta", clan_map));
    // divide
    ASSERT_TRUE(tel_aviv->getGroupsNames().contains("Alpha1_2"));
    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Lambda1", "Gamma", clan_map));
    ASSERT_NO_EXCEPTION(tel_aviv->groupArrive("Lambda2", "Gamma", clan_map));
    //unite with lambda1

    ASSERT_FALSE(tel_aviv->getGroupsNames().contains("Lambda2"));

    return true;
}

bool testMountainConstructor() {
	ASSERT_EXCEPTION(Mountain(""), AreaInvalidArguments);
	ASSERT_NO_EXCEPTION(Mountain("yo"));
	return true;
}

bool testMountainGroupArrive(){
    Area* carmel(new Mountain("Carmel"));
    std::map<std::string, Clan> clan_map = makeClanMap();
    ASSERT_NO_EXCEPTION(carmel->groupArrive("Alpha1", "Beta", clan_map));
    ASSERT_NO_EXCEPTION(carmel->groupArrive("Lambda2", "Gamma", clan_map));
    ostringstream os;
    ASSERT_NO_EXCEPTION(os << *clan_map.at("Gamma").getGroup("Lambda2"));
    ASSERT_TRUE(VerifyOutput(os ,"Group's name: Lambda2\n"
        "Group's clan: Gamma\n"
        "Group's children: 3\n"
        "Group's adults: 3\n"
        "Group's tools: 10\n"
        "Group's food: 12\n"
        "Group's morale: 61\n"));
    return true;
}

bool testMountainGroupLeave(){
    Area* carmel(new Mountain("Carmel"));
    std::map<std::string, Clan> clan_map = makeClanMap();
    ASSERT_NO_EXCEPTION(carmel->groupArrive("Alpha1", "Beta", clan_map));
    ASSERT_NO_EXCEPTION(carmel->groupArrive("Lambda2", "Gamma", clan_map));
    ASSERT_NO_EXCEPTION(carmel->groupArrive("Lambda3", "Gamma", clan_map));
    ASSERT_NO_EXCEPTION(carmel->groupArrive("Alpha2", "Beta", clan_map));
    ostringstream os;
    ASSERT_NO_EXCEPTION(os << *clan_map.at("Beta").getGroup("Alpha2"));
	ASSERT_TRUE(VerifyOutput(os ,"Group's name: Alpha2\n"
		"Group's clan: Beta\n"
		"Group's children: 6\n"
		"Group's adults: 6\n"
		"Group's tools: 20\n"
		"Group's food: 0\n"
		"Group's morale: 61\n"));
    ASSERT_NO_EXCEPTION(carmel->groupLeave("Lambda3"));
	ASSERT_NO_EXCEPTION(os << *clan_map.at("Gamma").getGroup("Lambda2"));
	ASSERT_TRUE(VerifyOutput(os ,"Group's name: Lambda2\n"
		"Group's clan: Gamma\n"
		"Group's children: 3\n"
		"Group's adults: 3\n"
		"Group's tools: 10\n"
		"Group's food: 12\n"
		"Group's morale: 61\n"));
	ASSERT_NO_EXCEPTION(carmel->groupLeave("Lambda2"));
	ASSERT_NO_EXCEPTION(os << *clan_map.at("Beta").getGroup("Alpha2"));
	ASSERT_TRUE(VerifyOutput(os ,"Group's name: Alpha2\n"
		"Group's clan: Beta\n"
		"Group's children: 6\n"
		"Group's adults: 6\n"
		"Group's tools: 20\n"
		"Group's food: 0\n"
		"Group's morale: 61\n"));

    return true;
}

bool testRiverConstructor() {
	ASSERT_EXCEPTION(River(""), AreaInvalidArguments);
	ASSERT_NO_EXCEPTION(River("yo"));
	return true;
}

bool testRiverGroupArrive(){
    Area* jordan(new River("Jordan"));
    std::map<std::string, Clan> clan_map = makeClanMap();
    ASSERT_NO_EXCEPTION(jordan->groupArrive("Alpha1", "Beta", clan_map));
    ASSERT_NO_EXCEPTION(jordan->groupArrive("Alpha2", "Beta", clan_map));
    ostringstream os;
    ASSERT_NO_EXCEPTION(os << *clan_map.at("Beta").getGroup("Alpha1"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha1\n"
        "Group's clan: Beta\n"
        "Group's children: 10\n"
        "Group's adults: 10\n"
        "Group's tools: 53\n"
        "Group's food: 37\n"
        "Group's morale: 77\n"));
    return true;
}

int main(){
	RUN_TEST(testPlainConstructor);
	RUN_TEST(testPlainGroupArrive);
	RUN_TEST(testMountainConstructor);
	RUN_TEST(testMountainGroupArrive);
	RUN_TEST(testMountainGroupLeave);
	RUN_TEST(testRiverConstructor);
	RUN_TEST(testRiverGroupArrive);
    return 0;
}
