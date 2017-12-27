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
	List list= listCreate(copyString, destroyString);
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
	ASSERT_TEST(compareStrings("HELLO WORLD" , list->head->data, NULL)==0);
	// checking if the first element indeed is as expected.
	return true;
}

static bool testListInsertLast() {
	List list= listCreate(copyString, destroyString);
	ASSERT_TEST(listInsertFirst(list, "HELLO WORLD") == LIST_SUCCESS);
	list->iterator = list->head; // the iterator is the head node.
	ASSERT_TEST(listInsertLast(list, "last element") == LIST_SUCCESS);
	// checking if the function returned success.
	ASSERT_TEST(compareStrings("last element",list->head->next->data, NULL)==0);
	// checking if the new element indeed is the last one(second element);
	ASSERT_TEST(listInsertLast(list, NULL) == LIST_NULL_ARGUMENT);
	// the function should fail because a NULL element was sent.
	ASSERT_TEST(list->iterator == list->head); // the iterator shouldn`t change;
	// checking if it still is the head node.
	return true;
}

static bool testListInsertAfterCurrent() {
	List list= listCreate(copyString, destroyString);
	ASSERT_TEST(listInsertAfterCurrent(list,"hello") == LIST_INVALID_CURRENT);
	// should fail because iterator=NULL.
	ASSERT_TEST(listInsertFirst(list, "HELLO WORLD") == LIST_SUCCESS);
	list->iterator = list->head; // the iterator is the head of the list.
	ASSERT_TEST(listInsertAfterCurrent(list,"hello") == LIST_SUCCESS);
	// should pass.
	list->iterator = list->iterator->next; // the iterator is the second element
	ASSERT_TEST(listInsertLast(list, "last element") == LIST_SUCCESS);
	// this is the last element. iterator should be the second element
	ASSERT_TEST(listInsertAfterCurrent(list,"middle") == LIST_SUCCESS);
	// this element should be "hello" element. therefore "middle" is between
	// "hello" and "last element".
	list->iterator = list->iterator->next; // the iterator should be "middle"
	ASSERT_TEST(compareStrings(list->iterator->data, "middle", NULL)==0);
	// checking if the iterator indeed is "middle".
	return true;
}


static bool testListCopy() {
	char* arr[5] = {"hello","hi","hey","world"};
	List list1 = listCreate(copyString,destroyString);
	for (int i=0; i<4; i++){
		ASSERT_TEST(listInsertFirst(list1,arr[i])==LIST_SUCCESS);
	}
	List list2 = listCopy(list1);
	ASSERT_TEST(list2!=NULL); // if the function indeed returned the new list
	list1->iterator = list1->head; // the iterator is the head of the list
	list2->iterator = list2->head; // the iterator is the head of the list
	for (int i=0; i<4; i++){
		ASSERT_TEST(compareStrings(list1->iterator->data, list2->iterator->data, NULL)==0);
		list1->iterator = list1->iterator->next;
		list2->iterator = list2->iterator->next;
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
	char* arr[4] = {"hello","hi","hey","world"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<4; i++){
		ASSERT_TEST(listInsertFirst(list,arr[i])==LIST_SUCCESS);
	}
	ASSERT_TEST(listGetFirst(NULL)==NULL);
	ASSERT_TEST(compareStrings(list->head->data,listGetFirst(list),NULL)==0);
	// if list->head and listGetFirst(list) are the same.
	return true;
}

static bool testListGetNext() {
	char* arr[4] = {"hello","world","2017","2018"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<4; i++){
		ASSERT_TEST(listInsertFirst(list,arr[i])==LIST_SUCCESS);
	}
	ASSERT_TEST(listGetNext(list)==NULL); //should be NULL because iterator=NULL
	list->iterator = list->head->next; // iterator is the second element(world)
	ASSERT_TEST(compareStrings("world", listGetNext(list), NULL)==0);
	// listGetNext should return the next iterator, i.e. "2017".
	return true;
}

static bool testListInsertBeforeCurrent() {
		char* arr[1] = {"MATAM"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<1; i++){
		ASSERT_TEST(listInsertLast(list,arr[i])==LIST_SUCCESS);
	}
	Node starting_iterator= list->head; // it shouldn`t change. points on MATAM
	ASSERT_TEST(compareStrings("MATAM", listGetFirst(list), NULL)==0);
	ASSERT_TEST(listInsertBeforeCurrent(list,"INFI")==LIST_SUCCESS);
	ASSERT_TEST(listInsertBeforeCurrent(list,"ALGEBRA")==LIST_SUCCESS);
	ASSERT_TEST(listInsertBeforeCurrent(list,"ATAM")==LIST_SUCCESS);
	ASSERT_TEST(compareStrings("INFI",list->head->data,NULL)==0);
	ASSERT_TEST(compareStrings("MATAM",starting_iterator->data,NULL)==0);
	return true;
}


static bool testListRemoveCurrent() {
	char* arr[4] = {"hello","world","2017","2018"};
	List list = listCreate(copyString,destroyString);
	for (int i=0; i<4; i++){
		ASSERT_TEST(listInsertLast(list,arr[i])==LIST_SUCCESS);
	}
	ASSERT_TEST(listRemoveCurrent(list)==LIST_INVALID_CURRENT); //iterator=NULL
	list->iterator = list->head;
	ASSERT_TEST(listRemoveCurrent(list)==LIST_SUCCESS);
	ASSERT_TEST(list->iterator==NULL); //should be NULL after calling the func.
	ASSERT_TEST(compareStrings(list->head->data, "world", NULL)==0);// after removing
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
	list->iterator=list->head;
	ASSERT_TEST(compareStrings(list->iterator->data,"aaa",NULL));// aaa should be the

	//first element.
	list->iterator=list->iterator->next;
	ASSERT_TEST(compareStrings(list->iterator->data,"bbb",NULL)); // bbb should be the
	// second element.
	list->iterator=list->iterator->next;
	ASSERT_TEST(compareStrings(list->iterator->data,"ccc",NULL)); // ccc should be the
	// third and last element.
	return true;
}



static bool testListClear() {
	return true;
}

static bool testListDestroy() {
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
	return 0;
}
