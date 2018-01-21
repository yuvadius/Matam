/*#include "testMacros.h"
#include "../MtmSet.h"
#include "../exceptions.h"

using namespace mtm;

bool isEven(int n){
    return !(n%2);
}

bool isOdd(int n){
    return (n%2);
}

bool testConstructor() {
	MtmSet<string> set;
	ASSERT_TRUE(set.empty());
	return true;
}

bool testCopyConstructor() {
	MtmSet<string> set;
	ASSERT_NO_EXCEPTION(set.insert("hello"));
	ASSERT_NO_EXCEPTION(set.insert("world"));
	MtmSet<string> set2(set);
	ASSERT_TRUE(set == set2);
	MtmSet<int> set3;
	ASSERT_NO_EXCEPTION(set3.insert(1));
	ASSERT_NO_EXCEPTION(set3.insert(2));
	MtmSet<int> set4(set3);
	ASSERT_TRUE(set3 == set4);
	return true;
}

bool testInsert() {
	MtmSet<string> set;
	ASSERT_NO_EXCEPTION(set.insert("hello"));
	ASSERT_TRUE(set.size() == 1);
	ASSERT_TRUE(set.contains("hello"));
	ASSERT_NO_EXCEPTION(set.insert("hello"));
	ASSERT_TRUE(set.size() == 1);
	ASSERT_FALSE(set.contains("he"));
	ASSERT_NO_EXCEPTION(set.insert("world"));
	ASSERT_TRUE(set.contains("world"));
	MtmSet<string> set2(set);
	ASSERT_TRUE(set == set2);
	MtmSet<double> set3;
	ASSERT_NO_EXCEPTION(set3.insert(-12.23));
	ASSERT_TRUE(set3.contains(-12.23));
	ASSERT_TRUE(set3.size() == 1);
	ASSERT_NO_EXCEPTION(set3.insert(2));
	ASSERT_TRUE(set3.size() == 2);
	MtmSet<double> set4(set3);
	ASSERT_TRUE(set3 == set4);
	return true;
}

bool testEraseElem() {
	MtmSet<string> set;
	ASSERT_NO_EXCEPTION(set.insert("hello"));
	ASSERT_TRUE(set.contains("hello"));
	ASSERT_FALSE(set.contains("he"));
	ASSERT_NO_EXCEPTION(set.insert("world"));
	ASSERT_TRUE(set.contains("world"));
	MtmSet<string> set2(set);
	ASSERT_TRUE(set == set2);
	set.erase("world ");
	ASSERT_TRUE(set == set2);
	set.erase(" world");
	ASSERT_TRUE(set == set2);
	set.erase("");
	ASSERT_TRUE(set == set2);
	set.erase("world");
	ASSERT_TRUE(set != set2);
	ASSERT_TRUE(set.size() == 1);
	set.erase("hello");
	ASSERT_TRUE(set.empty());
	MtmSet<double> set3;
	ASSERT_NO_EXCEPTION(set3.insert(-12.23));
	set3.erase(0);
	ASSERT_TRUE(set3.size() == 1);
	set3.erase(-12.23);
	ASSERT_TRUE(set3.empty());
	return true;
}

bool testEraseIterator() {
	MtmSet<string> set;
	ASSERT_NO_EXCEPTION(set.insert("hello"));
	ASSERT_TRUE(set.contains("hello"));
	ASSERT_FALSE(set.contains("he"));
	ASSERT_NO_EXCEPTION(set.insert("world"));
	ASSERT_TRUE(set.contains("world"));
	MtmSet<string> set2(set);
	ASSERT_TRUE(set == set2);
	MtmSet<string>::iterator it = set.end();
	ASSERT_EXCEPTION(set.erase(it), NodeIsEndException);
	ASSERT_TRUE(set == set2);
	MtmSet<string>::iterator empty_iterator;
	ASSERT_EXCEPTION(set.erase(empty_iterator), NodeIsEndException);
	ASSERT_TRUE(set == set2);
	set.erase(set.find("world"));
	ASSERT_TRUE(set != set2);
	ASSERT_TRUE(set.size() == 1);
	set.erase(set.begin());
	ASSERT_TRUE(set.empty());
	MtmSet<double> set3;
	ASSERT_NO_EXCEPTION(set3.insert(-12.23));
	set3.erase(set3.find(-12.23));
	ASSERT_TRUE(set3.empty());
	return true;
}

bool testSize() {
	MtmSet<string> set;
	ASSERT_NO_EXCEPTION(set.insert("hello world"));
	ASSERT_TRUE(set.size() == 1);
	ASSERT_NO_EXCEPTION(set.insert("world"));
	ASSERT_TRUE(set.size() == 2);
	MtmSet<string> set2(set);
	ASSERT_TRUE(set == set2);
	set.erase("hello world");
	ASSERT_TRUE(set.size() == 1);
	set.erase("world");
	ASSERT_TRUE(set.size() == 0);
	ASSERT_TRUE(set.empty());
	MtmSet<double> set3;
	ASSERT_NO_EXCEPTION(set3.insert(-12.23));
	ASSERT_TRUE(set3.size() == 1);
	set3.erase(-12.23);
	ASSERT_TRUE(set3.size() == 0);
	return true;
}

bool testEmpty() {
	MtmSet<string> set;
	ASSERT_TRUE(set.empty());
	ASSERT_NO_EXCEPTION(set.insert("hello world"));
	ASSERT_FALSE(set.empty());
	ASSERT_NO_EXCEPTION(set.insert("world"));
	ASSERT_FALSE(set.empty());
	MtmSet<string> set2(set);
	ASSERT_TRUE(set == set2);
	set.erase("hello world");
	ASSERT_FALSE(set.empty());
	set.erase("world");
	ASSERT_TRUE(set.empty());
	MtmSet<double> set3;
	ASSERT_NO_EXCEPTION(set3.insert(-12.23));
	ASSERT_FALSE(set3.empty());
	set3.erase(-12.23);
	ASSERT_TRUE(set3.empty());
	return true;
}

bool testClear() {
	MtmSet<string> set;
	ASSERT_NO_EXCEPTION(set.insert("hello world"));
	set.clear();
	ASSERT_TRUE(set.empty());
	ASSERT_NO_EXCEPTION(set.insert("world"));
	ASSERT_FALSE(set.empty());
	set.clear();
	ASSERT_TRUE(set.empty());
	MtmSet<double> set3;
	ASSERT_NO_EXCEPTION(set3.insert(-12.23));
	ASSERT_FALSE(set3.empty());
	set3.clear();
	ASSERT_TRUE(set3.empty());
	return true;
}

bool testBegin() {
	MtmSet<string> set;
	ASSERT_NO_EXCEPTION(set.insert("agdfdg"));
	ASSERT_TRUE(*(set.begin()) == "agdfdg");
	ASSERT_NO_EXCEPTION(set.insert("worsagdld"));
	ASSERT_TRUE(*(set.begin()) == "worsagdld");
	set.clear();
	MtmSet<string>::iterator it = set.end();
	ASSERT_TRUE(set.begin() == it);
	MtmSet<double> set3;
	ASSERT_NO_EXCEPTION(set3.insert(-12.23));
	ASSERT_TRUE(*(set3.begin()) == -12.23);
	set3.clear();
	return true;
}

bool testBeginConst() {
	MtmSet<string> set;
	ASSERT_NO_EXCEPTION(set.insert("agdfdg"));
	ASSERT_NO_EXCEPTION(set.insert("worsagdld"));
	const MtmSet<string> set2(set);
	ASSERT_TRUE(*(set2.begin()) == "agdfdg");
	return true;
}

bool testEnd() {
	MtmSet<string> set;
	MtmSet<string>::iterator it;
	ASSERT_TRUE(it == set.end());
	MtmSet<string>::const_iterator it2;
	ASSERT_TRUE(it2 == set.end());
	return true;
}

bool testEndConst() {
	const MtmSet<string> set;
	MtmSet<string>::iterator it;
	ASSERT_TRUE(it == set.end());
	MtmSet<string>::const_iterator it2;
	ASSERT_TRUE(it2 == set.end());
	return true;
}

bool testFind() {
	MtmSet<string> set;
	MtmSet<string>::iterator empty_iterator = set.end();
	ASSERT_NO_EXCEPTION(set.insert("agdfdg"));
	ASSERT_TRUE(*(set.find("agdfdg")) == "agdfdg");
	ASSERT_NO_EXCEPTION(set.insert("worsagdld"));
	ASSERT_TRUE(*(set.find("worsagdld")) == "worsagdld");
	ASSERT_FALSE(set.find("doesn't exist") != empty_iterator);
	set.clear();
	return true;
}

bool testFindConst() {
	MtmSet<string> set;
	MtmSet<string>::iterator empty_iterator = set.end();
	ASSERT_NO_EXCEPTION(set.insert("agdfdg"));
	ASSERT_NO_EXCEPTION(set.insert("worsagdld"));
	const MtmSet<string> set2(set);
	ASSERT_TRUE(*(set2.find("agdfdg")) == "agdfdg");
	ASSERT_TRUE(*(set2.find("worsagdld")) == "worsagdld");
	ASSERT_FALSE(set2.find("doesn't exist") != empty_iterator);
	set.clear();
	return true;
}

bool testContains() {
	MtmSet<string> set;
	MtmSet<string>::iterator empty_iterator = set.end();
	ASSERT_NO_EXCEPTION(set.insert("agdfdg"));
	ASSERT_NO_EXCEPTION(set.insert("worsagdld"));
	ASSERT_TRUE(set.contains("agdfdg"));
	ASSERT_TRUE(set.contains("worsagdld"));
	ASSERT_FALSE(set.contains("doesn't exist"));
	return true;
}

bool testIsSuperSetOf() {
	MtmSet<string> set;
	MtmSet<string>::iterator empty_iterator = set.end();
	ASSERT_NO_EXCEPTION(set.insert("this homework is taking forever"));
	ASSERT_NO_EXCEPTION(set.insert("i need to study infi already"));
	MtmSet<string> set2(set);
	ASSERT_TRUE(set.isSuperSetOf(set2));
	ASSERT_TRUE(set2.isSuperSetOf(set));
	MtmSet<string> set3;
	ASSERT_NO_EXCEPTION(set3.insert("this homework is taking forever"));
	MtmSet<string> set4(set3);
	ASSERT_NO_EXCEPTION(set3.insert("i need to study infi already"));
	ASSERT_TRUE(set3.isSuperSetOf(set4));
	ASSERT_FALSE(set4.isSuperSetOf(set3));
	return true;
}

bool testSetEqual() {
	MtmSet<string> set;
	MtmSet<string>::iterator empty_iterator = set.end();
	ASSERT_NO_EXCEPTION(set.insert("this homework is taking forever"));
	ASSERT_NO_EXCEPTION(set.insert("i need to study infi already"));
	MtmSet<string> set2(set);
	ASSERT_TRUE(set == set2);
	MtmSet<string> set3;
	ASSERT_NO_EXCEPTION(set3.insert("this homework is taking forever"));
	MtmSet<string> set4(set3);
	ASSERT_NO_EXCEPTION(set3.insert("i need to study infi already"));
	ASSERT_FALSE(set3 == set4);
	return true;
}

bool testSetNotEqual() {
	MtmSet<string> set;
	MtmSet<string>::iterator empty_iterator = set.end();
	ASSERT_NO_EXCEPTION(set.insert("this homework is taking forever"));
	ASSERT_NO_EXCEPTION(set.insert("i need to study infi already"));
	MtmSet<string> set2(set);
	ASSERT_FALSE(set != set2);
	MtmSet<string> set3;
	ASSERT_NO_EXCEPTION(set3.insert("this homework is taking forever"));
	MtmSet<string> set4(set3);
	ASSERT_NO_EXCEPTION(set3.insert(""));
	ASSERT_TRUE(set3 != set4);
	return true;
}

bool testSetUnite() {
	MtmSet<int> set;
	ASSERT_NO_EXCEPTION(set.insert(1));
	ASSERT_NO_EXCEPTION(set.insert(2));
	MtmSet<int> set2(set);
	ASSERT_NO_EXCEPTION(set.erase(1));
	MtmSet<int>::iterator it = set.begin();
	ASSERT_TRUE(*(it++) == 2);
	ASSERT_NO_EXCEPTION(set.unite(set2));
	ASSERT_TRUE(set == set2);
	MtmSet<int> set3;
	ASSERT_NO_EXCEPTION(set3.unite(set2));
	ASSERT_TRUE(set == set3);
	return true;
}

bool testSetUniteConst() {
	MtmSet<int> set;
	ASSERT_NO_EXCEPTION(set.insert(1));
	ASSERT_NO_EXCEPTION(set.insert(2));
	const MtmSet<int> set2(set);
	ASSERT_NO_EXCEPTION(set.erase(1));
	MtmSet<int>::iterator it = set.begin();
	ASSERT_TRUE(*(it++) == 2);
	ASSERT_NO_EXCEPTION(set2.unite(set) == set);
	const MtmSet<int> set3;
	ASSERT_NO_EXCEPTION(set3.unite(set2) == set);
	return true;
}

bool testIntersect() {
	MtmSet<int> set;
	ASSERT_NO_EXCEPTION(set.insert(5));
	ASSERT_NO_EXCEPTION(set.insert(6));
	MtmSet<int> set2;
	ASSERT_NO_EXCEPTION(set2.insert(5));
	ASSERT_NO_EXCEPTION(set.intersect(set2));
	ASSERT_TRUE(set.size() == 1);
	ASSERT_TRUE(*(set.begin()) == 5);
	MtmSet<int> set3;
	ASSERT_NO_EXCEPTION(set.intersect(set3));
	ASSERT_TRUE(set.size() == 0);
	return true;
}

bool testIntersectConst() {
	MtmSet<int> set;
	ASSERT_NO_EXCEPTION(set.insert(5));
	ASSERT_NO_EXCEPTION(set.insert(6));
	MtmSet<int> set2;
	const MtmSet<int> const_set(set);
	ASSERT_NO_EXCEPTION(set2.insert(5));
	ASSERT_TRUE(*(const_set.intersect(set2).begin()) == 5);
	ASSERT_TRUE((const_set.intersect(set2).size()) == 1);
	MtmSet<int> set3;
	ASSERT_TRUE((const_set.intersect(set3).size()) == 0);
	return true;
}

bool testGetSubSet() {
	MtmSet<int> set;
	ASSERT_NO_EXCEPTION(set.insert(5));
	ASSERT_NO_EXCEPTION(set.insert(6));
	ASSERT_NO_EXCEPTION(set.getSubSet(isOdd));
	ASSERT_TRUE(set.size() == 1);
	ASSERT_TRUE(*(set.begin()) == 5);
	ASSERT_NO_EXCEPTION(set.getSubSet(isEven));
	ASSERT_TRUE(set.size() == 0);
	return true;
}

bool testGetSubSetConst() {
	MtmSet<int> set;
	ASSERT_NO_EXCEPTION(set.insert(5));
	const MtmSet<int> set3(set);
	ASSERT_NO_EXCEPTION(set.insert(6));
	const MtmSet<int> set2(set);
	ASSERT_TRUE(*(set2.getSubSet(isOdd).begin()) == 5);
	ASSERT_TRUE(set2.getSubSet(isOdd).size() == 1);
	ASSERT_TRUE(set3.getSubSet(isOdd).size() == 1);
	ASSERT_TRUE(set3.getSubSet(isEven).size() == 0);
	return true;
}

int main(){
	RUN_TEST(testConstructor);
	RUN_TEST(testCopyConstructor);
	RUN_TEST(testInsert);
	RUN_TEST(testEraseElem);
	RUN_TEST(testEraseIterator);
	RUN_TEST(testSize);
	RUN_TEST(testEmpty);
	RUN_TEST(testClear);
	RUN_TEST(testBegin);
	RUN_TEST(testBeginConst);
	RUN_TEST(testEnd);
	RUN_TEST(testEndConst);
	RUN_TEST(testFind);
	RUN_TEST(testFindConst);
	RUN_TEST(testContains);
	RUN_TEST(testIsSuperSetOf);
	RUN_TEST(testSetEqual);
	RUN_TEST(testSetNotEqual);
	RUN_TEST(testSetUnite);
	RUN_TEST(testSetUniteConst);
	RUN_TEST(testIntersect);
	RUN_TEST(testIntersectConst);
	RUN_TEST(testGetSubSet);
	RUN_TEST(testGetSubSetConst);
    return 0;
}*/
