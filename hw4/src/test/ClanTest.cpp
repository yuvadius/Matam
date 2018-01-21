/*#include "testMacros.h"
#include "../Group.h"
#include "../Clan.h"
#include "../exceptions.h"

using namespace mtm;

bool testConstructor() {
	ASSERT_EXCEPTION(Clan(""), ClanEmptyName);
    Clan gamma("Gamma");
    ASSERT_TRUE(gamma.getSize() == 0);
    return true;
}

bool testCopyConstructor() {
    Clan gamma("Gamma");
    ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma1", 10, 10)));
	ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma2", 5, 5)));
	Clan beta(gamma);
	ASSERT_TRUE(beta.getSize() == 30);
    return true;
}

bool testAddGroup() {
    Clan gamma("Gamma");
    ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma1", 7, 9)));
    Clan beta(gamma);
	ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma2", 5, 50)));
	ASSERT_TRUE(gamma.doesContain("Gamma1"));
	ASSERT_TRUE(gamma.doesContain("Gamma2"));
	ASSERT_FALSE(gamma.doesContain("Gamma3"));
	ASSERT_TRUE(beta.doesContain("Gamma1"));
	ASSERT_FALSE(beta.doesContain("Gamma2"));
	ASSERT_TRUE(beta.getSize() == 16);
	ASSERT_TRUE(gamma.getSize() == 71);
    return true;
}

bool testGetGroup() {
    Clan gamma("Gamma");
    ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma1", 7, 9)));
    Clan beta(gamma);
	ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma2", 5, 50)));
	ASSERT_TRUE(beta.getGroup("Gamma1")->getClan() == "Gamma");
	ASSERT_TRUE(beta.getGroup("Gamma1")->getSize() == 16);
	ASSERT_TRUE(beta.getGroup("Gamma1")->getName() == "Gamma1");
	ASSERT_EXCEPTION(beta.getGroup("Gamma2"), ClanGroupNotFound);
	ASSERT_TRUE(gamma.getGroup("Gamma2")->getClan() == "Gamma");
    return true;
}

bool testDoesContain() {
    Clan gamma("Gamma");
    ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma1", 7, 9)));
    Clan beta(gamma);
	ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma2", 5, 50)));
	ASSERT_TRUE(beta.doesContain("Gamma1"));
	ASSERT_FALSE(beta.doesContain("Gamma2"));
	ASSERT_TRUE(gamma.doesContain("Gamma2"));
    return true;
}

bool testGetSize() {
    Clan gamma("Gamma");
    ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma1", 7, 9)));
    Clan beta(gamma);
    ASSERT_TRUE(beta.getGroup("Gamma1")->getSize() == 16);
    ASSERT_TRUE(beta.getSize() == 16);
	ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma2", 5, 50)));
	ASSERT_TRUE(gamma.getGroup("Gamma2")->getSize() == 55);
	ASSERT_TRUE(gamma.getSize() == 71);
    return true;
}

bool testUnite() {
    Clan gamma("Gamma");
    ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma1", 7, 9)));
    Clan beta(gamma);
	ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma2", 5, 50)));
	ASSERT_EXCEPTION(beta.unite(beta, "Lambda"), ClanCantUnite);
	ASSERT_EXCEPTION(beta.unite(gamma, "Lambda"), ClanCantUnite);
	ASSERT_EXCEPTION(beta.unite(beta, ""), ClanEmptyName);
	Clan lambda("Lambda");
	ASSERT_NO_EXCEPTION(lambda.addGroup(Group("Lambda1", 5, 2)));
	ASSERT_NO_EXCEPTION(lambda.addGroup(Group("Lambda2", 15, 12)));
	ASSERT_NO_EXCEPTION(gamma.unite(lambda, "New Clan"));
	ASSERT_TRUE(lambda.getSize() == 0);
	ostringstream os;
	ASSERT_NO_EXCEPTION(os << gamma << beta << lambda);
	ASSERT_TRUE(VerifyOutput(os, "Clan's name: New Clan\n"
		"Clan's groups:\n"
		"Gamma2\n"
		"Lambda2\n"
		"Gamma1\n"
		"Lambda1\n"
		"Clan's name: Gamma\n"
		"Clan's groups:\n"
		"Gamma1\n"
		"Clan's name: Lambda\n"
		"Clan's groups:\n"));
    return true;
}

bool testMakeFriend() {
    Clan gamma("Gamma");
    ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma1", 7, 9)));
	ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma2", 5, 50)));
	Clan beta("Beta");
	ASSERT_NO_EXCEPTION(beta.addGroup(Group("Beta1", 735, 97)));
	ASSERT_NO_EXCEPTION(beta.addGroup(Group("Beta2", 54, 503)));
	ASSERT_TRUE(gamma.isFriend(gamma));
	ASSERT_FALSE(gamma.isFriend(beta));
	ASSERT_NO_EXCEPTION(gamma.makeFriend(beta));
	ASSERT_TRUE(gamma.isFriend(beta));
	ASSERT_TRUE(beta.isFriend(gamma));
    return true;
}

bool testIsFriend() {
    Clan gamma("Gamma");
    ASSERT_TRUE(gamma.isFriend(gamma));
    ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma1", 7, 9)));
	ASSERT_NO_EXCEPTION(gamma.addGroup(Group("Gamma2", 5, 50)));
	Clan beta("Beta");
	ASSERT_TRUE(beta.isFriend(beta));
	ASSERT_NO_EXCEPTION(beta.addGroup(Group("Beta1", 735, 97)));
	ASSERT_NO_EXCEPTION(beta.addGroup(Group("Beta2", 54, 503)));
	ASSERT_FALSE(gamma.isFriend(beta));
	ASSERT_NO_EXCEPTION(gamma.makeFriend(beta));
	ASSERT_TRUE(gamma.isFriend(beta));
	ASSERT_TRUE(beta.isFriend(gamma));
    return true;
}

bool testOutput(){
    Clan beta("Beta");
    ASSERT_NO_EXCEPTION(beta.addGroup(Group("Alpha1", 10, 10)));
    ASSERT_NO_EXCEPTION(beta.addGroup(Group("Alpha2", 5, 5)));
    ASSERT_NO_EXCEPTION(beta.addGroup(Group("Alpha3", 15, 15)));
    ASSERT_NO_EXCEPTION(beta.getGroup("Alpha1")->getClan() == "Beta");
    ASSERT_TRUE(beta.doesContain("Alpha2"));
    ASSERT_TRUE(beta.getSize() == 60);
    Clan beta2("Beta2");
    ASSERT_NO_EXCEPTION(beta2.addGroup(Group("Alpha4", 20, 20)));
    ASSERT_NO_EXCEPTION(beta.unite(beta2, "Beta3"));
    ostringstream os;
    ASSERT_TRUE(beta2.getSize() == 0);
    Clan gamma1("Gamma 1");
    Clan gamma2("Gamma 2");
    ASSERT_NO_EXCEPTION(gamma1.makeFriend(gamma2));
    ASSERT_TRUE(gamma1.isFriend(gamma2));
    ASSERT_NO_EXCEPTION(os << beta << gamma1 << gamma2);
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: Beta3\n"
            "Clan's groups:\n"
            "Alpha4\n"
            "Alpha3\n"
            "Alpha1\n"
            "Alpha2\n"
    		"Clan's name: Gamma 1\n"
    		"Clan's groups:\n"
    		"Clan's name: Gamma 2\n"
    		"Clan's groups:\n"));
    return true;
}

int main(){
	RUN_TEST(testConstructor);
	RUN_TEST(testCopyConstructor);
	RUN_TEST(testAddGroup);
	RUN_TEST(testGetGroup);
	RUN_TEST(testDoesContain);
	RUN_TEST(testGetSize);
	RUN_TEST(testUnite);
	RUN_TEST(testMakeFriend);
	RUN_TEST(testIsFriend);
	RUN_TEST(testOutput);
    return 0;
}*/
