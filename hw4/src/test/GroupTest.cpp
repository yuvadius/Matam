/*#include "testMacros.h"
#include "../Group.h"
#include "../exceptions.h"

using namespace mtm;

bool testConstructor1() {
	ASSERT_EXCEPTION(Group("", "", 1, 2, 3, 4, 5), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("Alpha", "", -1, 2, 3, 4, 5), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("Alpha", "", 1, -2, 3, 4, 5), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("Beta", "", 1, 2, -3, 4, 5), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("Lambda", "", 1, 2, 3, -4, 5), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("Gamma", "", 1, 2, 3, 4, -5), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("Gamma", "", 1, 2, 3, 4, 435), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("Gamma", "", 0, 0, 3, 4, 5), GroupInvalidArgs);
	Group alpha("Alpha", "Beta", 10, 10, 40, 50, 80);
	Group gamma("Gamma", "Lambda" ,5 , 5, 20, 10 , 90);
	ASSERT_TRUE(alpha.getName() == "Alpha");
	ASSERT_TRUE(alpha.getSize() == 20);
	ASSERT_TRUE(alpha.getClan() == "Beta");
	ASSERT_TRUE(alpha > gamma);
	ASSERT_FALSE(alpha == gamma);

	std::ostringstream os;

	os << alpha << gamma;
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
		"Group's clan: Beta\n"
		"Group's children: 10\n"
		"Group's adults: 10\n"
		"Group's tools: 40\n"
		"Group's food: 50\n"
		"Group's morale: 80\n"
		"Group's name: Gamma\n"
		"Group's clan: Lambda\n"
		"Group's children: 5\n"
		"Group's adults: 5\n"
		"Group's tools: 20\n"
		"Group's food: 10\n"
		"Group's morale: 90\n"));

	return true;
}

bool testConstructor2() {
	ASSERT_EXCEPTION(Group("", 1, 2), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("Alpha", -1, 2), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("Beta", 1, -2), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("Gamma", -1, -2), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("Lambda", 0, 0), GroupInvalidArgs);
	Group alpha("Alpha", 10, 10);
	Group gamma("Gamma", 5 , 0);
	ASSERT_TRUE(alpha.getName() == "Alpha");
	ASSERT_TRUE(alpha.getSize() == 20);
	ASSERT_TRUE(alpha.getClan() == "");
	ASSERT_TRUE(alpha > gamma);
	ASSERT_FALSE(alpha == gamma);

	std::ostringstream os;

	os << alpha << gamma;
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
		"Group's clan: \n"
		"Group's children: 10\n"
		"Group's adults: 10\n"
		"Group's tools: 40\n"
		"Group's food: 50\n"
		"Group's morale: 70\n"
		"Group's name: Gamma\n"
		"Group's clan: \n"
		"Group's children: 5\n"
		"Group's adults: 0\n"
		"Group's tools: 0\n"
		"Group's food: 10\n"
		"Group's morale: 70\n"));

	return true;
}

bool testGetName() {
	Group alpha("Alpha", "Beta", 10, 10, 10, 10, 70);
	Group gamma("Gamma", "Beta", 5 , 0, 10, 10, 70);
	ASSERT_TRUE(alpha.getName() == "Alpha");
	ASSERT_TRUE(gamma.getName() == "Gamma");
	ASSERT_TRUE(alpha.getClan() == "Beta");
	ASSERT_TRUE(gamma.getClan() == "Beta");
	ASSERT_TRUE(alpha > gamma);
	ASSERT_FALSE(alpha == gamma);
	ASSERT_TRUE(alpha.unite(gamma, 40));
	ASSERT_TRUE(alpha.getName() == "Alpha");
	ASSERT_TRUE(gamma.getName() == "");
	return true;
}

bool testGetSize() {
	Group alpha("Alpha", "Beta", 10, 10, 10, 10, 70);
	Group gamma("Gamma", "Beta", 5 , 0, 10, 10, 70);
	ASSERT_TRUE(alpha.getSize() == 20);
	ASSERT_TRUE(gamma.getSize() == 5);
	ASSERT_TRUE(alpha.getClan() == "Beta");
	ASSERT_TRUE(alpha > gamma);
	ASSERT_FALSE(alpha == gamma);
	ASSERT_TRUE(alpha.unite(gamma, 25));
	ASSERT_TRUE(alpha.getSize() == 25);
	ASSERT_TRUE(gamma.getSize() == 0);
	Group alpha2 = alpha.divide("Alpha 2");
	ASSERT_TRUE(alpha.getSize() == 13);
	ASSERT_TRUE(alpha2.getSize() == 12);
	return true;
}

bool testGetClan() {
	Group alpha("Alpha", 10, 10);
	ASSERT_TRUE(alpha.getClan() == "");
	Group gamma("Gamma", 5 , 0);
	ASSERT_TRUE(gamma.getClan() == "");
	Group alpha2("Alpha2", "", 0, 1, 2, 10, 10);
	ASSERT_TRUE(alpha2.getClan() == "");
	Group alpha3("Alpha3", "Beta", 0, 2, 2, 10, 70);
	ASSERT_TRUE(alpha3.getClan() == "Beta");
	Group alpha4("Alpha4", "Beta", 0, 1, 2, 10, 70);
	ASSERT_TRUE(alpha4.getClan() == "Beta");
	ASSERT_TRUE(alpha3.unite(alpha4, 5));
	ASSERT_TRUE(alpha3.getSize() == 3);
	ASSERT_TRUE(alpha4.getSize() == 0);
	ASSERT_TRUE(alpha3.getClan() == "Beta");
	ASSERT_TRUE(alpha4.getClan() == "");
	Group alpha5 = alpha3.divide("Alpha 5");
	ASSERT_TRUE(alpha3.getClan() == "Beta");
	ASSERT_TRUE(alpha5.getClan() == "Beta");
	return true;
}

bool testChangeClan() {
	Group alpha("Alpha", "Beta", 0, 2, 2, 10, 100);
	alpha.changeClan("Beta");
	ASSERT_TRUE(alpha.getClan() == "Beta");
	alpha.changeClan("Clan");
	ASSERT_TRUE(alpha.getClan() == "Clan");

	std::ostringstream os;
	os << alpha;
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
		"Group's clan: Clan\n"
		"Group's children: 0\n"
		"Group's adults: 2\n"
		"Group's tools: 2\n"
		"Group's food: 10\n"
		"Group's morale: 90\n"));

	alpha.changeClan("Clan");
	ASSERT_TRUE(alpha.getClan() == "Clan");
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
			"Group's clan: Clan\n"
			"Group's children: 0\n"
			"Group's adults: 2\n"
			"Group's tools: 2\n"
			"Group's food: 10\n"
			"Group's morale: 90\n"));

	alpha.changeClan("");
	ASSERT_TRUE(alpha.getClan() == "");
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
			"Group's clan: \n"
			"Group's children: 0\n"
			"Group's adults: 2\n"
			"Group's tools: 2\n"
			"Group's food: 10\n"
			"Group's morale: 81\n"));

	alpha.changeClan("Final");
	ASSERT_TRUE(alpha.getClan() == "");
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
			"Group's clan: Final\n"
			"Group's children: 0\n"
			"Group's adults: 2\n"
			"Group's tools: 2\n"
			"Group's food: 10\n"
			"Group's morale: 89\n"));
	return true;
}

bool testLess() {
	Group alpha("Alpha", "Beta", 10, 10, 10, 10, 70);
	Group gamma("Gamma", "Beta", 5 , 0, 10, 10, 70);
	ASSERT_FALSE(alpha < gamma);
	ASSERT_TRUE(gamma < alpha);
	Group alpha2("Alpha", "Beta", 10, 10, 10, 10, 70);
	ASSERT_FALSE(alpha < alpha2);
	Group alpha3("Alpha", "Beta", 20, 0, 10, 10, 70);
	ASSERT_FALSE(alpha < alpha3);
	Group alpha4("Alpha", "Beta", 0, 20, 10, 10, 70);
	ASSERT_TRUE(alpha < alpha4);
	Group alpha5("Alpha", "Beta", 9, 10, 10, 10, 100);
	ASSERT_TRUE(alpha < alpha5);
	Group alpha6("ZZZ", "Beta", 10, 10, 10, 10, 70);
	ASSERT_TRUE(alpha < alpha6);
	Group alpha7("Alpha", "Beta", 10, 10, 11, 9, 70);
	ASSERT_TRUE(alpha < alpha7);
	Group alpha8("Alpha", "Beta", 10, 10, 9, 11, 70);
	ASSERT_FALSE(alpha < alpha8);
	Group alpha9("Alpha", "Beta", 100, 100, 9000, 11000, 0);
	ASSERT_FALSE(alpha < alpha9);
	Group alpha10("Alpha", "Beta", 1, 0, 9000000, 100100000, 100);
	ASSERT_TRUE(alpha < alpha10);
	return true;
}

bool testMore() {
	Group alpha("Alpha", "Beta", 10, 10, 10, 10, 70);
	Group gamma("Gamma", "Beta", 5 , 0, 10, 10, 70);
	ASSERT_TRUE(alpha > gamma);

	ASSERT_FALSE(gamma > alpha);

	Group alpha2("Alpha", "Beta", 10, 10, 10, 10, 70);

	ASSERT_FALSE(alpha > alpha2);

	Group alpha3("Alpha", "Beta", 20, 0, 10, 10, 70);

	ASSERT_TRUE(alpha > alpha3);

	Group alpha4("Alpha", "Beta", 0, 20, 10, 10, 70);

	ASSERT_FALSE(alpha > alpha4);

	Group alpha5("Alpha", "Beta", 9, 10, 10, 10, 100);

	ASSERT_FALSE(alpha > alpha5);

	Group alpha6("ZZZ", "Beta", 10, 10, 10, 10, 70);

	ASSERT_FALSE(alpha > alpha6);

	Group alpha7("Alpha", "Beta", 10, 10, 11, 9, 70);

	ASSERT_FALSE(alpha > alpha7);

	Group alpha8("Alpha", "Beta", 10, 10, 9, 11, 70);

	ASSERT_TRUE(alpha > alpha8);

	Group alpha9("Alpha", "Beta", 100, 100, 9000, 11000, 0);

	ASSERT_TRUE(alpha > alpha9);

	Group alpha10("Alpha", "Beta", 1, 0, 9000000, 100100000, 100);

	ASSERT_FALSE(alpha > alpha10);
	return true;
}

bool testLessEqual() {
	Group alpha("Alpha", "Beta", 10, 10, 10, 10, 70);
	Group gamma("Gamma", "Beta", 5 , 0, 10, 10, 70);
	ASSERT_FALSE(alpha <= gamma);
	ASSERT_TRUE(gamma <= alpha);
	Group alpha2("Alpha", "Beta", 10, 10, 10, 10, 70);
	ASSERT_TRUE(alpha <= alpha2);
	Group alpha3("Alpha", "Beta", 20, 0, 10, 10, 70);
	ASSERT_FALSE(alpha <= alpha3);
	Group alpha4("Alpha", "Beta", 0, 20, 10, 10, 70);
	ASSERT_TRUE(alpha <= alpha4);
	Group alpha5("Alpha", "Beta", 9, 10, 10, 10, 100);
	ASSERT_TRUE(alpha <= alpha5);
	Group alpha6("ZZZ", "Beta", 10, 10, 10, 10, 70);
	ASSERT_TRUE(alpha <= alpha6);
	Group alpha7("Alpha", "Beta", 10, 10, 11, 9, 70);
	ASSERT_TRUE(alpha <= alpha7);
	Group alpha8("Alpha", "Beta", 10, 10, 9, 11, 70);
	ASSERT_FALSE(alpha <= alpha8);
	Group alpha9("Alpha", "Beta", 100, 100, 9000, 11000, 0);
	ASSERT_FALSE(alpha <= alpha9);
	Group alpha10("Alpha", "Beta", 1, 0, 9000000, 100100000, 100);
	ASSERT_TRUE(alpha <= alpha10);
	return true;
}

bool testMoreEqual() {
	Group alpha("Alpha", "Beta", 10, 10, 10, 10, 70);
	Group gamma("Gamma", "Beta", 5 , 0, 10, 10, 70);
	ASSERT_TRUE(alpha >= gamma);

	ASSERT_FALSE(gamma >= alpha);

	Group alpha2("Alpha", "Beta", 10, 10, 10, 10, 70);

	ASSERT_TRUE(alpha >= alpha2);

	Group alpha3("Alpha", "Beta", 20, 0, 10, 10, 70);

	ASSERT_TRUE(alpha >= alpha3);

	Group alpha4("Alpha", "Beta", 0, 20, 10, 10, 70);

	ASSERT_FALSE(alpha >= alpha4);

	Group alpha5("Alpha", "Beta", 9, 10, 10, 10, 100);

	ASSERT_FALSE(alpha >= alpha5);

	Group alpha6("ZZZ", "Beta", 10, 10, 10, 10, 70);

	ASSERT_FALSE(alpha >= alpha6);

	Group alpha7("Alpha", "Beta", 10, 10, 11, 9, 70);

	ASSERT_FALSE(alpha >= alpha7);

	Group alpha8("Alpha", "Beta", 10, 10, 9, 11, 70);

	ASSERT_TRUE(alpha >= alpha8);

	Group alpha9("Alpha", "Beta", 100, 100, 9000, 11000, 0);

	ASSERT_TRUE(alpha >= alpha9);

	Group alpha10("Alpha", "Beta", 1, 0, 9000000, 100100000, 100);

	ASSERT_FALSE(alpha >= alpha10);
	return true;
}

bool testEqual() {
	Group alpha("Alpha", "Beta", 10, 10, 10, 10, 70);
	Group gamma("Gamma", "Beta", 5 , 0, 10, 10, 70);
	ASSERT_FALSE(alpha == gamma);
	ASSERT_FALSE(gamma == alpha);
	Group alpha2("Alpha", "Beta", 10, 10, 10, 10, 70);
	ASSERT_TRUE(alpha == alpha2);
	Group alpha3("Alpha", "Beta", 20, 0, 10, 10, 70);
	ASSERT_FALSE(alpha == alpha3);
	Group alpha4("Alpha", "Beta", 0, 20, 10, 10, 70);
	ASSERT_FALSE(alpha == alpha4);
	Group alpha5("Alpha", "Beta", 9, 10, 10, 10, 100);
	ASSERT_FALSE(alpha == alpha5);
	Group alpha6("ZZZ", "Beta", 10, 10, 10, 10, 70);
	ASSERT_FALSE(alpha == alpha6);
	Group alpha7("Alpha", "Beta", 10, 10, 11, 9, 70);
	ASSERT_FALSE(alpha == alpha7);
	Group alpha8("Alpha", "Beta", 10, 10, 9, 11, 70);
	ASSERT_FALSE(alpha == alpha8);
	Group alpha9("Alpha", "Beta", 100, 100, 9000, 11000, 0);
	ASSERT_FALSE(alpha == alpha9);
	Group alpha10("Alpha", "Beta", 1, 0, 9000000, 100100000, 100);
	ASSERT_FALSE(alpha == alpha10);
	Group alpha11("Alpha", "Beta", 10, 10, 9, 20, 70);
	ASSERT_TRUE(alpha == alpha11);
	Group alpha12("Alpha", "Beta", 10, 10, 11, 0, 70);
	ASSERT_TRUE(alpha == alpha12);
	Group alpha13("Alpha", "Beta", 0, 13, 10, 10, 70);
	ASSERT_TRUE(alpha == alpha13);
	return true;
}

bool testNotEqual() {
	Group alpha("Alpha", "Beta", 10, 10, 10, 10, 70);
	Group gamma("Gamma", "Beta", 5 , 0, 10, 10, 70);
	ASSERT_TRUE(alpha != gamma);
	ASSERT_TRUE(gamma != alpha);
	Group alpha2("Alpha", "Beta", 10, 10, 10, 10, 70);
	ASSERT_FALSE(alpha != alpha2);
	Group alpha3("Alpha", "Beta", 20, 0, 10, 10, 70);
	ASSERT_TRUE(alpha != alpha3);
	Group alpha4("Alpha", "Beta", 0, 20, 10, 10, 70);
	ASSERT_TRUE(alpha != alpha4);
	Group alpha5("Alpha", "Beta", 9, 10, 10, 10, 100);
	ASSERT_TRUE(alpha != alpha5);
	Group alpha6("ZZZ", "Beta", 10, 10, 10, 10, 70);
	ASSERT_TRUE(alpha != alpha6);
	Group alpha7("Alpha", "Beta", 10, 10, 11, 9, 70);
	ASSERT_TRUE(alpha != alpha7);
	Group alpha8("Alpha", "Beta", 10, 10, 9, 11, 70);
	ASSERT_TRUE(alpha != alpha8);
	Group alpha9("Alpha", "Beta", 100, 100, 9000, 11000, 0);
	ASSERT_TRUE(alpha != alpha9);
	Group alpha10("Alpha", "Beta", 1, 0, 9000000, 100100000, 100);
	ASSERT_TRUE(alpha != alpha10);
	Group alpha11("Alpha", "Beta", 10, 10, 9, 20, 70);
	ASSERT_FALSE(alpha != alpha11);
	Group alpha12("Alpha", "Beta", 10, 10, 11, 0, 70);
	ASSERT_FALSE(alpha != alpha12);
	Group alpha13("Alpha", "Beta", 0, 13, 10, 10, 70);
	ASSERT_FALSE(alpha != alpha13);
	return true;
}

bool testUnite() {
	ASSERT_EXCEPTION(Group("", 1, 1), GroupInvalidArgs);
	Group alpha("Alpha", "Beta", 10, 10, 40, 50, 80);
	Group gamma("Gamma", "Lambda" ,5 , 5, 20, 10 , 90);
	ASSERT_TRUE(alpha.getName() == "Alpha");
	ASSERT_TRUE(alpha.getSize() == 20);
	ASSERT_TRUE(alpha.getClan() == "Beta");
	ASSERT_NO_EXCEPTION(alpha.changeClan("Lambda"));
	ASSERT_TRUE(alpha.getClan() == "Lambda");
	ASSERT_TRUE(alpha > gamma);
	ASSERT_FALSE(alpha == gamma);

	ASSERT_TRUE(alpha.trade(gamma));
	ASSERT_FALSE(alpha.unite(gamma, 29));
	ASSERT_TRUE(alpha.unite(gamma, 30));

	std::ostringstream os;
	os << alpha << gamma;
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
		"Group's clan: Lambda\n"
		"Group's children: 15\n"
		"Group's adults: 15\n"
		"Group's tools: 60\n"
		"Group's food: 60\n"
		"Group's morale: 78\n"
		"Group's name: \n" // notice the space!!!!!
		"Group's clan: \n" //notice the space!!!!!
		"Group's children: 0\n"
		"Group's adults: 0\n"
		"Group's tools: 0\n"
		"Group's food: 0\n"
		"Group's morale: 0\n"));

	ASSERT_FALSE(alpha.unite(gamma, 100));

	Group gamma2("Gamma", "Lambda" ,5 , 5, 20, 10 , 69);
	ASSERT_FALSE(alpha.unite(gamma2, 100));

	Group gamma3("Gamma", "Lambda" ,55 , 5, 20, 10 , 69);
	ASSERT_FALSE(alpha.unite(gamma3, 100));

	Group gamma4("Alpha", "Lambda", 15, 15, 60, 60, 78);
	ASSERT_FALSE(alpha.unite(gamma4, 100));

	Group gamma5("Gamma", "Beta2", 10, 10, 10, 10, 70);
	ASSERT_FALSE(alpha.unite(gamma5, 100));

	return true;
}

bool testDivide() {
    ASSERT_EXCEPTION(Group("", 1, 1), GroupInvalidArgs);
    Group alpha("Alpha", "Beta", 10, 10, 40, 50, 80);
    Group gamma("Gamma", "Lambda" ,5 , 5, 20, 10 , 90);
    ASSERT_TRUE(alpha.getName() == "Alpha");
    ASSERT_TRUE(alpha.getSize() == 20);
    ASSERT_TRUE(alpha.getClan() == "Beta");
    ASSERT_NO_EXCEPTION(alpha.changeClan("Lambda"));
    ASSERT_TRUE(alpha.getClan() == "Lambda");
    ASSERT_TRUE(alpha > gamma);
    ASSERT_FALSE(alpha == gamma);

    ASSERT_TRUE(alpha.trade(gamma));

    std::ostringstream os;
    ASSERT_TRUE(alpha.unite(gamma, 40));
    Group alpha2 = alpha.divide("Alpha 2");
    ASSERT_TRUE(alpha.fight(alpha2) == WON);

    os << alpha << alpha2;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
        "Group's clan: Lambda\n"
        "Group's children: 8\n"
        "Group's adults: 6\n"
        "Group's tools: 23\n"
        "Group's food: 37\n"
        "Group's morale: 94\n"
        "Group's name: Alpha 2\n"
        "Group's clan: Lambda\n"
        "Group's children: 4\n"
        "Group's adults: 4\n"
        "Group's tools: 15\n"
        "Group's food: 15\n"
        "Group's morale: 62\n"));

    ASSERT_EXCEPTION(alpha.divide(""), GroupInvalidArgs);
    Group alpha3("Alpha", "Beta", 1, 1, 40, 50, 80);
    ASSERT_EXCEPTION(alpha3.divide("Empty"), GroupCantDivide);
    Group alpha4("Alpha", "Beta", 0, 1, 40, 50, 80);
	ASSERT_EXCEPTION(alpha4.divide("Empty"), GroupCantDivide);
	Group alpha5("Alpha", "Beta", 1, 0, 40, 50, 80);
	ASSERT_EXCEPTION(alpha5.divide("Empty"), GroupCantDivide);
    return true;
}

bool testFight() {
	ASSERT_EXCEPTION(Group("", 1, 1), GroupInvalidArgs);
	Group alpha("Alpha", "Beta", 10, 10, 40, 50, 80);
	Group gamma("Gamma", "Lambda" ,5 , 5, 20, 10 , 90);
	ASSERT_TRUE(alpha.getName() == "Alpha");
	ASSERT_TRUE(alpha.getSize() == 20);
	ASSERT_TRUE(alpha.getClan() == "Beta");
	ASSERT_NO_EXCEPTION(alpha.changeClan("Lambda"));
	ASSERT_TRUE(alpha.getClan() == "Lambda");
	ASSERT_TRUE(alpha > gamma);
	ASSERT_FALSE(alpha == gamma);

	ASSERT_TRUE(alpha.trade(gamma));

	std::ostringstream os;

	ASSERT_TRUE(alpha.unite(gamma, 40));

	Group alpha2 = alpha.divide("Alpha 2");

	ASSERT_EXCEPTION(alpha.fight(alpha), GroupCantFightWithItself);

	ASSERT_TRUE(alpha.fight(alpha2) == WON);

	os << alpha << alpha2;
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
		"Group's clan: Lambda\n"
		"Group's children: 8\n"
		"Group's adults: 6\n"
		"Group's tools: 23\n"
		"Group's food: 37\n"
		"Group's morale: 94\n"
		"Group's name: Alpha 2\n"
		"Group's clan: Lambda\n"
		"Group's children: 4\n"
		"Group's adults: 4\n"
		"Group's tools: 15\n"
		"Group's food: 15\n"
		"Group's morale: 62\n"));

	ASSERT_TRUE(alpha2.fight(alpha) == LOST);
	ASSERT_TRUE(alpha2.fight(alpha) == LOST);
	ASSERT_TRUE(alpha2.fight(alpha) == LOST);
	ASSERT_EXCEPTION(alpha.fight(alpha2), GroupCantFightEmptyGroup);

	return true;
}

bool testTrade() {
	ASSERT_EXCEPTION(Group("", 1, 1), GroupInvalidArgs);
	Group alpha("Alpha", "Beta", 10, 10, 40, 50, 80);
	Group gamma("Gamma", "Lambda" ,5 , 5, 20, 10 , 90);
	ASSERT_TRUE(alpha.getName() == "Alpha");
	ASSERT_TRUE(alpha.getSize() == 20);
	ASSERT_TRUE(alpha.getClan() == "Beta");
	ASSERT_NO_EXCEPTION(alpha.changeClan("Lambda"));
	ASSERT_TRUE(alpha.getClan() == "Lambda");
	ASSERT_TRUE(alpha > gamma);
	ASSERT_FALSE(alpha == gamma);

	ASSERT_TRUE(alpha.trade(gamma));

	ASSERT_EXCEPTION(alpha.trade(alpha), GroupCantTradeWithItself);

	ASSERT_EXCEPTION(gamma.trade(gamma), GroupCantTradeWithItself);

	std::ostringstream os;

	os << alpha << gamma;
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
		"Group's clan: Lambda\n"
		"Group's children: 10\n"
		"Group's adults: 10\n"
		"Group's tools: 45\n"
		"Group's food: 45\n"
		"Group's morale: 72\n"
		"Group's name: Gamma\n"
		"Group's clan: Lambda\n"
		"Group's children: 5\n"
		"Group's adults: 5\n"
		"Group's tools: 15\n"
		"Group's food: 15\n"
		"Group's morale: 90\n"));

	ASSERT_FALSE(alpha.trade(gamma));

	Group alpha2("Alpha", "Beta", 10, 10, 40, 50, 80);
	Group gamma2("Gamma", "Beta" ,5 , 5, 20, 100 , 90);
	ASSERT_FALSE(alpha2.trade(gamma2));

	Group alpha3("Alpha", "Beta", 10, 10, 400, 50, 80);
	Group gamma3("Gamma", "Beta" ,5 , 5, 200, 100 , 90);
	ASSERT_FALSE(alpha3.trade(gamma3));

	Group alpha4("Alpha", "Beta", 10, 10, 100, 0, 80);
	Group gamma4("Gamma", "Beta" ,5 , 5, 0, 10 , 90);
	ASSERT_TRUE(alpha4.trade(gamma4));
	os << alpha4 << gamma4;
	ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
		"Group's clan: Beta\n"
		"Group's children: 10\n"
		"Group's adults: 10\n"
		"Group's tools: 90\n"
		"Group's food: 10\n"
		"Group's morale: 80\n"
		"Group's name: Gamma\n"
		"Group's clan: Beta\n"
		"Group's children: 5\n"
		"Group's adults: 5\n"
		"Group's tools: 10\n"
		"Group's food: 0\n"
		"Group's morale: 90\n"));

	return true;
}

bool testOutput(){
    ASSERT_EXCEPTION(Group("", 1, 1), GroupInvalidArgs);
    Group alpha("Alpha", "Beta", 10, 10, 40, 50, 80);
    Group gamma("Gamma", "Lambda" ,5 , 5, 20, 10 , 90);
    ASSERT_TRUE(alpha.getName() == "Alpha");
    ASSERT_TRUE(alpha.getSize() == 20);
    ASSERT_TRUE(alpha.getClan() == "Beta");
    ASSERT_NO_EXCEPTION(alpha.changeClan("Lambda"));
    ASSERT_TRUE(alpha.getClan() == "Lambda");
    ASSERT_TRUE(alpha > gamma);
    ASSERT_FALSE(alpha == gamma);

    ASSERT_TRUE(alpha.trade(gamma));

    std::ostringstream os;

    os << alpha << gamma;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
        "Group's clan: Lambda\n"
        "Group's children: 10\n"
        "Group's adults: 10\n"
        "Group's tools: 45\n"
        "Group's food: 45\n"
        "Group's morale: 72\n"
        "Group's name: Gamma\n"
        "Group's clan: Lambda\n"
        "Group's children: 5\n"
        "Group's adults: 5\n"
        "Group's tools: 15\n"
        "Group's food: 15\n"
        "Group's morale: 90\n"));


    ASSERT_TRUE(alpha.unite(gamma, 40));


    os << alpha << gamma;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
        "Group's clan: Lambda\n"
        "Group's children: 15\n"
        "Group's adults: 15\n"
        "Group's tools: 60\n"
        "Group's food: 60\n"
        "Group's morale: 78\n"
        "Group's name: \n" // notice the space!!!!!
        "Group's clan: \n" //notice the space!!!!!
        "Group's children: 0\n"
        "Group's adults: 0\n"
        "Group's tools: 0\n"
        "Group's food: 0\n"
        "Group's morale: 0\n"));

    Group alpha2 = alpha.divide("Alpha 2");
    ASSERT_TRUE(alpha.fight(alpha2) == WON);

    os << alpha << alpha2;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: Alpha\n"
        "Group's clan: Lambda\n"
        "Group's children: 8\n"
        "Group's adults: 6\n"
        "Group's tools: 23\n"
        "Group's food: 37\n"
        "Group's morale: 94\n"
        "Group's name: Alpha 2\n"
        "Group's clan: Lambda\n"
        "Group's children: 4\n"
        "Group's adults: 4\n"
        "Group's tools: 15\n"
        "Group's food: 15\n"
        "Group's morale: 62\n"));

    return true;
}

int main(){
	RUN_TEST(testConstructor1);
	RUN_TEST(testConstructor2);
	RUN_TEST(testGetName);
	RUN_TEST(testGetSize);
	RUN_TEST(testGetClan);
	RUN_TEST(testLess);
	RUN_TEST(testMore);
	RUN_TEST(testLessEqual);
	RUN_TEST(testMoreEqual);
	RUN_TEST(testEqual);
	RUN_TEST(testNotEqual);
	RUN_TEST(testUnite);
	RUN_TEST(testDivide);
	RUN_TEST(testFight);
	RUN_TEST(testTrade);
	RUN_TEST(testOutput);
    return 0;
}*/
