#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "../list_mtm/list_mtm.h"


ListElement copyString(ListElement str) {
	assert(str);
	char* copy = malloc(strlen(str) + 1);
	return copy ? strcpy(copy, str) : NULL;
}
void destroyString(ListElement str) {
	free(str); // frees the string element.
}
int compareStrings(ListElement str1, ListElement str2, ListSortKey key) {
	key = NULL; // no need in the key for compartign strings.
	return strcmp(str1, str2);
}
bool isLongerThan(ListElement string, ListFilterKey key) {
	return strlen(string) > *(int*)key;
}

/**
 * List of tests, one for each function is usually a good thumb rule.
 */
static bool testListCreate() {
	List list = listCreate(copyString, destroyString);
	ASSERT_TEST(list != NULL);
	// if the list has been successfully created, then TRUE.
	ASSERT_TEST(listCreate(copyString,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,destroyString) == NULL);
	return true;
}

static bool testListInsertFirst() {
	List list= listCreate(copyString, destroyString);
	ASSERT_TEST(listInsertFirst(list, "HELLO WORLD") == LIST_SUCCESS);
	//if the element was successfully inserted to list->head
	char* first = listGetFirst(list);
	ASSERT_TEST(first!=NULL); // shouldnt be NULL
	ASSERT_TEST(compareStrings("HELLO WORLD" , first, NULL)==0);
	// checking if the first element indeed is as expected.
	return true;
}

static bool testListInsertLast() {
	List list= listCreate(copyString, destroyString);
	ASSERT_TEST(listInsertFirst(list, "HELLO WORLD") == LIST_SUCCESS);
	ASSERT_TEST(listGetFirst(list)!=NULL); // the iterator is the head node.
	ASSERT_TEST(listInsertLast(list, "last element") == LIST_SUCCESS);
	// checking if the function returned success.
	listGetNext(list); // iterator moved be 1 to the last Node
	char* last = listGetCurrent(list);
	ASSERT_TEST(last!=NULL); // shouldnt be NULL
	ASSERT_TEST(compareStrings("last element",last, NULL)==0);
	// checking if the new element indeed is the last one(second element);
	ASSERT_TEST(listInsertLast(list, NULL) == LIST_NULL_ARGUMENT);
	// the function should fail because a NULL element was sent.
	return true;
}

static bool testListInsertAfterCurrent() {
	List list= listCreate(copyString, destroyString);
	ASSERT_TEST(listInsertAfterCurrent(NULL,NULL) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listInsertAfterCurrent(NULL,"hello") == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listInsertAfterCurrent(list,NULL) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listInsertAfterCurrent(list,"hello") == LIST_INVALID_CURRENT);
	// should fail because iterator=NULL.
	ASSERT_TEST(listInsertFirst(list, "HELLO WORLD") == LIST_SUCCESS);
	listGetFirst(list); // the iterator is the head of the list.
	ASSERT_TEST(listInsertAfterCurrent(list,"hello") == LIST_SUCCESS);
	// should pass.
	listGetNext(list); // the iterator is the second element
	ASSERT_TEST(listInsertLast(list, "last element") == LIST_SUCCESS);
	// this is the last element. iterator should be the second element
	ASSERT_TEST(listInsertAfterCurrent(list,"middle") == LIST_SUCCESS);
	// this element should be "hello" element. therefore "middle" is between
	// "hello" and "last element".
	listGetNext(list);; // the iterator should be "middle"
	char* current = listGetCurrent(list);
	ASSERT_TEST(current!=NULL); // shouldnt be NULL

	ASSERT_TEST(compareStrings(current, "middle", NULL)==0);
	// checking if the iterator indeed is "middle".
	return true;
}


static bool testListCopy() {
	ASSERT_TEST(listCopy(NULL)==NULL);
	char* arr[4] = {"hello","hi","hey","world"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<4; i++){
		ASSERT_TEST(listInsertFirst(list,arr[i])==LIST_SUCCESS);
	}
	List list2 = listCopy(list);
	ASSERT_TEST(list2!=NULL); // if the function indeed returned the new list
	listGetFirst(list); // the iterator is the head of the list
	listGetFirst(list2); // the iterator is the head of the list
	for (int i=0; i<3; i++){
		ASSERT_TEST(compareStrings(listGetNext(list),
				listGetNext(list2),NULL)==0); // o shone..
	}
	// checks if all the elements are identical.
	return true;
}

static bool testListGetSize() {
	char* arr[4] = {"hello","hi","hey","world"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<4; i++){
		ASSERT_TEST(listInsertFirst(list,arr[i])==LIST_SUCCESS);
	}
	ASSERT_TEST(listGetSize(NULL)==-1); // should return -1.
	ASSERT_TEST(listGetSize(list)==4); // sould return 4.
	return true;
}

static bool testListGetFirst() {
	List list = listCreate(copyString,destroyString);
	ASSERT_TEST(listInsertFirst(list, "hello")==LIST_SUCCESS);
	char* first = listGetFirst(list);
	ASSERT_TEST(first!=NULL);//should be NULL
	ASSERT_TEST(compareStrings(first,"hello",NULL)==0);
	return true;
}

static bool testListGetNext() {
	char* arr[4] = {"hello","world","2017","2018"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<4; i++){
		ASSERT_TEST(listInsertFirst(list,arr[i])==LIST_SUCCESS);
	}
	ASSERT_TEST(listGetNext(list)==NULL); //should be NULL because iterator=NULL
	ASSERT_TEST(listGetFirst(list)!=NULL); // iterator is the first element
	ASSERT_TEST(listGetNext(list)!=NULL); //iterator is second element
	ASSERT_TEST(compareStrings("world", listGetNext(list), NULL)==0);
	// listGetNext should return the next iterator, i.e. "2017".
	return true;
}

static bool testListInsertBeforeCurrent() {
	List list = listCreate(copyString,destroyString);
	char* element = "hello";
	ASSERT_TEST(listInsertFirst(list, element)==LIST_SUCCESS);//success expected
	ASSERT_TEST(listInsertBeforeCurrent(NULL, element)== LIST_NULL_ARGUMENT);
													// this error is expected
	listGetFirst(list); // iterator is the first node.
	ASSERT_TEST(listInsertBeforeCurrent(list, "world")== LIST_SUCCESS);
	ASSERT_TEST(compareStrings("world", listGetFirst(list), NULL)==0);
	// checking if indeed the func inserted the element to the first place




	return true;
}


static bool testListRemoveCurrent() {
	char* arr[4] = {"hello","world","2017","2018"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<4; i++){
		ASSERT_TEST(listInsertLast(list,arr[i])==LIST_SUCCESS);
	}
	ASSERT_TEST(listRemoveCurrent(list)==LIST_INVALID_CURRENT); //iterator=NULL
	listGetFirst(list); // iterator = head
	ASSERT_TEST(listRemoveCurrent(list)==LIST_SUCCESS);
	ASSERT_TEST(listGetCurrent(list)==NULL); //should be NULL after
	//														calling the func.
	ASSERT_TEST(compareStrings(listGetFirst(list), "world",
													 NULL)==0);// after removing
	// the first element the head should be "world".
	return true;
}

static bool testListSort() {
	char* arr[3] = {"ccc","aaa","bbb",};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<3; i++){
		ASSERT_TEST(listInsertLast(list,arr[i])==LIST_SUCCESS);
	}
	ASSERT_TEST(listSort(list,compareStrings, NULL)==LIST_SUCCESS);
	ASSERT_TEST(listGetFirst(list)!=NULL); // iterator=head
	ASSERT_TEST(compareStrings(listGetCurrent(list),"aaa",NULL));
													// aaa should be the first
	//element.
	ASSERT_TEST(listGetNext(list)!=NULL);
	ASSERT_TEST(compareStrings(listGetCurrent(list),"bbb",NULL)); // bbb should
	// be the second element.
	ASSERT_TEST(listGetNext(list)!=NULL);
	ASSERT_TEST(compareStrings(listGetCurrent(list),"ccc",NULL)); // ccc should
	// be the third and last element.
	return true;
}

static bool testListFilter() {
    char* arr[5] = {"hello world","bye","true","more then 10","less"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i < 5; i++){
		ASSERT_TEST(listInsertLast(list,arr[i])==LIST_SUCCESS);
	}
	int key = 10;
	List filtered = listFilter(list,isLongerThan,&key);
	ASSERT_TEST(filtered != LIST_SUCCESS);
	ASSERT_TEST(!compareStrings("hello world",listGetFirst(filtered),NULL));
	ASSERT_TEST(!compareStrings(listGetNext(filtered),"more then 10",NULL));
	return true;
}

static bool testListClear() {
	char* arr[4] = {"hello","world","2017","2018"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<4; i++){
		ASSERT_TEST(listInsertLast(list,arr[i])==LIST_SUCCESS);
	}
	ASSERT_TEST(listClear(NULL)== LIST_NULL_ARGUMENT);
	ASSERT_TEST(listClear(list)== LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list)==0);
	return true;
}

static bool testListDestroy() {
	listDestroy(NULL); // should work
    char* arr[4] = {"hello","world","2017","2018"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<4; i++){
		ASSERT_TEST(listInsertLast(list,arr[i])==LIST_SUCCESS);
	}
	listDestroy(list);
	//ASSERT_TEST(list==NULL);
	return true;
}

static bool testListGetCurrent() {
    char* arr[4] = {"hello","world","2017","2018"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<4; i++){
		ASSERT_TEST(listInsertLast(list,arr[i])==LIST_SUCCESS);
	}
	listGetFirst(list); // iterator= list->head
	ASSERT_TEST(compareStrings(listGetCurrent(list), "hello", NULL)==0);
	return true;
}

int main() {
	RUN_TEST(testListCreate);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertAfterCurrent);
	RUN_TEST(testListCopy);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListGetNext);
	RUN_TEST(testListInsertBeforeCurrent);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListSort);
	RUN_TEST(testListClear);
	RUN_TEST(testListDestroy);
	RUN_TEST(testListFilter);
	RUN_TEST(testListGetCurrent);
	return 0;
}
