#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "list_mtm.h"
#include "list_mtm.c"


 ListElement copyString(ListElement str) {
   assert(str);
   char* copy = malloc(strlen(str) + 1);
   return copy ? strcpy(copy, str) : NULL;
 }
 listElement
 void destroyString(ListElement str) {
 	free(str); // frees the string element.
 }
 int compareStrings(ListElement str1, ListElement str2, listSortKey key) {
 	key = NULL; // no need in the key for compartign strings.
 	return strcmp(*str1, *str2);
 }
 



/**
 * List of tests, one for each function is usually a good thumb rule.
 */
static bool testListCreate() {
	List list= listCreate(copyString, destroyString);
	ASSERT_TEST(list != NULL);
// if the list has been successfully created, then TRUE.
	return true;
}

static bool testListInsertFirst() {
	List list = listCreate(copyString, destroyString);
	ASSERT_TEST(list != NULL); // if the list was successfully created.
	ListElement element = char* "HELLO WORLD"
	ASSERT_TEST(testListInsertFirst(list, "HELLO WORLD") == LIST_SUCCESS);
	//if the element was successfully inserted to list->head
	ASSERT_TEST(compareStrings("HELLO WORLD" , list->head->data)==0);
	return true;
}

static bool testListCopy() {
	List list1 = listCreate(copyString, destroyString);
	ASSERT_TEST(list1 != NULL);
	List list2 = listCreate(copyString, destroyString);
	ASSERT_TEST(list2 != NULL);

	return true;
}

static bool testListGetSize() {
	return true;
}

static bool testListGetFirst() {
	return true;
}

static bool testListGetNext() {
	return true;
}

static bool testListInsertFirst() {
	return true;
}

static bool testListInsertLast() {
	return true;
}

static bool testListInsertBeforeCurrent() {
	return true;
}

static bool testListInsertAfterCurrent() {
	return true;
}

static bool testListRemoveCurrent() {
	return true;
}

static bool testListSort() {
	return true;
}

static bool testListClear() {
	return true;
}

static bool testListDestroy() {
	return true;
}

int main() {
	RUN_TEST(testListCreate);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListCopy);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListGetNext);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertBeforeCurrent);
	RUN_TEST(testListInsertAfterCurrent);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListSort);
	RUN_TEST(testListClear);
	RUN_TEST(testListDestroy);
	return 0;
}
