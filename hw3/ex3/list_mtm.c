#include <stdlib.h>
#include <assert.h>
#include "list_mtm.h"

/** The List is implemented as a List of Elements.
* With iterator as an index to the current Element or NULL
*/
typedef struct node {
	ListElement data;
	struct node* next;
} *Node;
struct List_t {
	Node head;
	Node iterator;
	CopyListElement copyElement;
	FreeListElement freeElement;
};

/**
 * Creates a Node.
 *
 * The new node will contain the "data" and the "next" given as parameters
 *
 * @param data The target data to copy into the new node
 * a copy of data(using list->copyElement) will be copied into the new node
 * @param next The target next to copy into the new node 
 * @param list The list's copy function will be used to create a copy of "data"
 * @return
 * NULL if data is NULL or list is NULL or a memory allocation failed.
 * otherwise return the new node
 */
static Node createNode(ListElement data, Node next, List list) {
	//if the data is NULL the return NULL
	if(data == NULL || list == NULL) {
		return NULL;
	}
	Node node = malloc(sizeof(*node)); //create a new node
	//if there was a memory allocation error then return NULL
	if(node == NULL) {
		return NULL;
	}
	//make a copy of the "data" using the list's copy function
	ListElement new_element = list->copyElement(data);
	//if there was a memory error in the copy function then return NULL
	if(new_element == NULL) {
		free(node); //free the previously allocated node
		return NULL;
	}
	//set the node's data to the parameter "data"
	node->data = data;
	//set the node's next to the parameter "next"
	node->next = next;
	return node; //return the new node
}

/**
 * Swaps between the ListElements in the two Nodes
 *
 * @param node1 A Node to be swapped with node2
 * @param node2 A Node to be swapped with node1
 */
static void swapElements(Node node1, Node node2) {
	ListElement temp = node1->data; //save the node1 list element
	node1->data = node2->data;
	node2->data = temp;
}

/**
 * Allocates a new List.
 *
 * Creates a new empty list. This function receives the functions which will be
 * used for copying elements into the list and freeing them when needed.
 * For example, in order to create a list of strings one need to define these
 * two functions:
 * @code
 * ListElement copyString(ListElement str);
 * void freeString(ListElement str);
 * @endcode
 * And then create a list as follows:
 * @code
 * List listOfStrings = listCreate(copyString, freeString);
 * @endcode
 *
 * The iterator of the new list should point to NULL.
 *
 * @param copyElement Function pointer to be used for copying elements into
 * the list or when copying the list.
 * @param freeElement Function pointer to be used for removing elements from
 * the list.
 * @return
 * NULL - if one of the parameters is NULL or allocations failed.
 * A new List in case of success.
 */
List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
	if (!copyElement || !freeElement) { //if one of the parameters is NULL
		return NULL;
	}
	List list = malloc(sizeof(*list)); //create a new List
	if (list == NULL) { //if memory allocation failed
		return NULL;
	}
	list->head = NULL;
	list->iterator = NULL;
	list->copyElement = copyElement;
	list->freeElement = freeElement;
	return list;
}

/**
 * Creates a copy of target list.
 *
 * The new copy will contain all the elements from the source list in the same
 * order and will use the same functions as the original list for copying and
 * freeing elements.
 *
 * The iterator of the new list should point to the same element that the
 * original iterator is pointing to in the original list.
 *
 * @param list The target list to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A List containing the same elements with same order as list otherwise.
 */
List listCopy(List list) {
	//if a NULL was sent then return NULL
	if(list == NULL) {
		return NULL;
	}
	//create a copy of the list with its copyElement/freeElement functions
	List list_copy = listCreate(list->copyElement, list->freeElement);
	//if there was a memory allocation failure then return NULL
	if(list_copy == NULL) {
		return NULL;
	}
	int counter = 0; //the number of times the foreach was executed
	Node original_iterator = list->iterator;
	LIST_FOREACH(ListElement, element, list) {
		//insert the current node in list to be the last node in list_copy
		ListResult result = listInsertLast(list_copy, element);
		assert(result == LIST_NULL_ARGUMENT);//should never happen
		//if there was a memory allocation failure
		if(result == LIST_OUT_OF_MEMORY) {
			//reset the iterator to its original value
			list->iterator = original_iterator;
			listDestroy(listCopy);
			return NULL;
		}
		//if the original iterator location was reached in list
		if(list->iterator == original_iterator) {
			//The iterator of the new list will point to the same element 
			//that the original iterator is pointing to in the original list
			int temp_counter = counter;
			LIST_FOREACH(ListElement, list_copy_element, list_copy) {
				//if the appropriate iterator was reached, break
				if(temp_counter == 0) {
					break;
				}
				--temp_counter;
			}
		}
		++counter;
	}
	//reset the iterator to its original value
	list->iterator = original_iterator;
	return list_copy;
}

/**
 * Removes the currently pointed element of the list using the stored freeing
 * function
 *
 * The iterator should be set to NULL.
 *
 * @param list The list for which the current element will be removed
 * @return
 * LIST_NULL_ARGUMENT if list is NULL
 * LIST_INVALID_CURRENT if the current pointer of the list points to NULL
 * LIST_SUCCESS the current element was removed successfully
 */
ListResult listRemoveCurrent(List list) { //The iterator points to the element
	if(list==NULL)                       // we would like to remove.
		return LIST_NULL_ARGUMENT;
	if(listGetCurrent(list)==NULL)
		return LIST_INVALID_CURRENT;
	Node remove_iterator = list->iterator; // the iterator of the element we
										  // are willing to remove from the list
	if(list->head == remove_iterator || remove_iterator->next == NULL) { 
										 // if the element we are willing to 
										// remove is the first\last element of  
									   // the list.
		freeElement(remove_iterator->data); // the element is freed.
		list->iterator=NULL; 
		return LIST_SUCCESS; //GREAT SUCCESS! 👍
	}

	list->iterator = list->head; // the iterator is the first element of 
								// the list.

	LIST_FOREACH(ListElement,element,list) {
		if(list->iterator->next==remove_iterator) { // if the next element
										// is the one we are willing to remove
			list->iterator->next = remove_iterator->next; // then the next
			// element of the iterator is the element after the element we are
			// removing. It shouldn`t be NULL.
			freeElement(remove_iterator->data); // the element is freed.
			list->iterator = NULL;
			return LIST_SUCCESS; //GREAT SUCCESS! 👍
		}
	}
	// Shouldn`t get here because the element we are removing isn`t the last one
}

/**
 * Returns the current element (pointed by the iterator).
 *
 * Note: the element which is stored in the list is returned, not a copy.
 *
 * The iterator should not change.
 *
 * @param list The list for which to get the iterator
 * @return
 * NULL if the iterator points to NULL or a NULL sent as argument
 * The current element on the list in case of success
 */
ListElement listGetCurrent(List list) {
	//if a NULL was sent then return NULL
	if(list == NULL) {
		return NULL;
	}
	//if the iterator points to NULL then return NULL
	if(list->iterator == NULL) {
		return NULL;
	}
	//return the current element on the list
	return list->iterator->data;
}

/**
 * Sets the internal iterator to the first element and retrieves it.
 *
 * The list has an internal iterator (also called current element) to allow
 * iteration over the list's elements. This function sets the iterator to point
 * to the first element in the list and return it.
 * Use this to start iteraing over the list, searching from the beginning of
 * the list and/or get the first element in the list.
 * (To continue iteration use listGetNext)
 * @code
 * void f(List list) {
 *   ListElement first = listGetFirst(list);
 *   printf("The first element is at address %x\n", first);
 * }
 * @endcode
 *
 * Note: the element which is stored in the list is returned, not a copy.
 *
 * @param list The list for which to set the iterator and return the first
 * element.
 * @return
 * NULL is a NULL pointer was sent or the list is empty.
 * The first element of the list otherwise
 */
ListElement listGetFirst(List list) {
	//if a NULL was sent then return NULL
	if(list == NULL) {
		return NULL;
	}
	//sets the iterator to the first element of the list
	//if the list is empty then the iterator/head will be NULL
	list->iterator = list->head;

	//if the list is empty then this will return NULL;
	//else this will return the current Element, which is the first Element
	return listGetCurrent(list);
}

/**
 * Advances the list's iterator to the next element and returns it. If the
 * iterator points to the last element, the iterator should be set to NULL and
 * returned.
 *
 * Note: the element which is stored in the list is returned, not a copy.
 *
 * @param list The list for which to advance the iterator
 * @return
 * NULL if reached the end of the list, the iterator points to NULL or a NULL
 * sent as argument
 * The next element on the list in case of success
 */
ListElement listGetNext(List list) {
	//if a NULL was sent then return NULL
	if(list == NULL) {
		return NULL;
	}
	//if the iterator points to NULL then return NULL
	if(list->iterator == NULL) {
		return NULL;
	}
	//set the list iterator to the next Node
	//if the iterator points to the last element then the iterator will be NULL
	list->iterator = list->iterator->next;

	//will return the current Element in the list
	return listGetCurrent(list);
}

/**
 * Adds a new element to the list, the new element will be the first element.
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element in its start
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertFirst(List list, ListElement element) {
	//if a NULL was sent as list or element return LIST_NULL_ARGUMENT
	if(list == NULL || element == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	Node first_node = createNode(element, list->head, list); //create Node
	//if there was a memory allocation error then return LIST_OUT_OF_MEMORY
	if(first_node == NULL) {
		return LIST_OUT_OF_MEMORY;
	}
	//insert the first node to the start of the list
	list->head = first_node;
	return LIST_SUCCESS;
}


/**
 * Removes all elements from target list.
 * The elements are deallocated using the stored freeing function.
 *
 * The iterator should be set to NULL.
 *
 * @param list Target list to remove all element from
 * @return
 * LIST_NULL_ARGUMENT - if a NULL pointer was sent.
 * LIST_SUCCESS - Otherwise.
 */
ListResult listClear(List list) {
	if(list==NULL)
		return LIST_NULL_ARGUMENT;
	listGetFirst(list); //moves the iterator to the first element.
	iterator_next = listGetNext(list); // iterator moves to the second element.
	listGetFirst(list); //moves the iterator to the first element.
	while(iterator_next!=NULL) {
		listRemoveCurrent(list); // First element removed. iterator=NULL.
		list->iterator = iterator_next; // The iterator moves to the first 
										// element.
		iterator_next= listGetNext(list); // iterator moves to the second 
										  // element.
		listGetFirst(list); //moves the iterator to the first element.
	}
	listRemoveCurrent(list); // first(and last) element removed. iterator=NULL.
	return LIST_SUCCESS; // empty list. Iterator=NULL.
	}



/**
 * Adds a new element to the list, the new element will be the last element
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element in its end
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertLast(List list, ListElement element) {
	//if a NULL was sent as list or element return LIST_NULL_ARGUMENT
	if(list == NULL || element == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	Node original_iterator = list->iterator; //save the iterator location
	LIST_FOREACH(ListElement, list_element, list) {
		//the LIST_FOREACH ensures that the list->iterator != NULL
		if(list->iterator->next == NULL) {
			//store a copy of the element at the end of the list
			ListResult result = listInsertAfterCurrent(list, element);
			//reset the iterator to its original value
			list->iterator = original_iterator;
			return result;
		}
	}
	//if we reached this place then the list is empty
	//insert the element to the start of list(same as end because list is empty)
	return listInsertFirst(list, element);
}

/**
 * Adds a new element to the list, the new element will be place right after
 * the current element (as pointed by the inner iterator be of the list)
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element after its current element
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_INVALID_CURRENT if the list's iterator points to NULL
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertAfterCurrent(List list, ListElement element) {
	//if a NULL was sent as list or element return LIST_NULL_ARGUMENT
	if(list == NULL || element == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	//if the list's iterator points to NULL then return LIST_INVALID_CURRENT
	if(listGetCurrent(list) == NULL) {
		return LIST_INVALID_CURRENT;
	}
	//if the iterator is at the end of the list insert the element at the end
	if(list->iterator->next == NULL) {
		return listInsertLast(list, element);
	}
	Node node = createNode(element, list->iterator->next, list);
	//if there was a memory allocation error then return LIST_OUT_OF_MEMORY
	if(node == NULL) {
		return LIST_OUT_OF_MEMORY;
	}
	//insert the node to the list after the current node
	list->iterator->next = node;
	return LIST_SUCCESS;
}




/**
 * Returns the number of elements in a list
 *
 * The iterator should not change.
 *
 * @param list The target list which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the list.
 */
int listGetSize(List list) {
	if(list==NULL)
		return -1; // if the list is NULL we`ll return -1.
	Node original_iterator = list->iterator; // saving the original iterator.
	int count=0; // counting from 0.
	LIST_FOREACH(ListElement,element_data,list) { 
		if(list->iterator!== NULL) // if iterator ISN`T the last Node.
			++count; // increment the count var.
		else { // else iterator IS the last Node
			break; // then break the LIST_FOREACH loop.
		}
	}
	list->iterator=original_iterator; // currently iterator points to NULL.
	// So we give it the original value back.
	return count;

/**
 * Adds a new element to the list, the new element will be place right before
 * the current element (as pointed by the inner iterator of the list)
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element before its current element
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_INVALID_CURRENT if the list's iterator points to NULL
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertBeforeCurrent(List list, ListElement element) {
	//if a NULL was sent as list or element return LIST_NULL_ARGUMENT
	if(list == NULL || element == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	//if the list's iterator points to NULL then return LIST_INVALID_CURRENT
	if(listGetCurrent(list) == NULL) {
		return LIST_INVALID_CURRENT;
	}
	//if the current node equals to head node insert to the start of the list
	if(list->iterator == list->head) {
		return listInsertFirst(list, element);
	}
	Node original_iterator = list->iterator; //save the iterator location
	LIST_FOREACH(ListElement, list_element, list) {
		//the LIST_FOREACH ensures that the list->iterator != NULL
		if(list->iterator->next == original_iterator) {
			Node node = createNode(element, original_iterator, list);
			if(node == NULL) { //if there was a memory allocation error
				//reset the iterator to its original value
				list->iterator = original_iterator;
				return LIST_OUT_OF_MEMORY;
			}
			//insert the node to the list after the current node
			list->iterator->next = node;
			break;
		}
	}
	//reset the iterator to its original value
	list->iterator = original_iterator;
	return LIST_SUCCESS;

}

/**
 * Creates a new filtered copy of a list.
 *
 * This creates a new list with only the elements for which the filtering
 * function returned true.
 *
 * For example, the following code creates a new list, given a list of strings
 * containing only the strings which are longer than 10 characters.
 * @code
 *
 * bool isLongerThan(ListElement string, ListFilterKey key) {
 *   return strlen(string) > *(int*)key;
 * }
 *
 * List createFilteredList(List listOfStrings) {
 *   int key = 10;
 *   return listFilter(listOfStrings, isLongerThan, &key);
 * }
 * @endcode
 *
 * The iterator of the new list should point to NULL. The iterator of the
 * source list should not change.
 *
 * @param list The list for which a filtered copy will be made
 * @param filterElement The function used for determining whether a given
 * element should be in the resulting list or not.
 * @param key Any extra values that need to be sent to the filtering function
 * when called
 * @return
 * NULL if list or filterElement are NULL or a memory allocation failed.
 * A List containing only elements from list which filterElement returned true
 * for.
 */
List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
	//if a NULL was sent as list or filterElement return NULL
	if(list == NULL || filterElement == NULL) {
		return NULL;
	}
	List new_list = listCopy(list); //copy the list into new_list
	//if there was a memory allocation failure then return NULL
	if(new_list == NULL) {
		return NULL;
	}
	//remove all elements that don't pass the filter test in the new_list
	bool removed_element = false; //bool to check if an element was removed
	do {
		removed_element = false;
		//loop over all elements in the new_list
		LIST_FOREACH(ListElement, element, new_list) {
			//if the element didn't pass the filter test then remove the element
			if(filterElement(element, key) == false) {
				ListResult result = listRemoveCurrent(new_list);//remove element
				assert(result != LIST_SUCCESS); //this shouldn't happen
				removed_element = true; //an element was removed
				break; //restart iteration of the new_list
			}
		}
	} while(removed_element == true);
	return new_list;
}

/**
 * Sorts the list according to the given function.
 *
 * For example, the following code will sort a list of integers according to
 * their distance from 0.
 * @code
 * int closerTo(ListElement num1, ListElement num2, ListSortKey value) {
 *   int distance1 = abs(*(int*)num1 - *(int*)value);
 *   int distance2 = abs(*(int*)num2 - *(int*)value);
 *   return distance1 - distance2;
 * }
 *
 * void sortInts(List listOfInts) {
 *   int key = 0;
 *   listSort(listOfInts, closerTo, &key);
 * }
 * @endcode
 *
 * Elements which are treated as equal by the comparison function don't have a
 * defined order.
 *
 * After sorting, the iterator points to the same node in order. That is, if the
 * list before sorting was (1, 5, 2, 6), and the iterator pointed to "5", after
 * the sorting, (1, 2, 5, 6), the iterator will still point to the second
 * element in the list, which is "2".
 *
 * @param list the target list to sort
 * @param compareElement A comparison function as defined in the type
 * CompareListElements. This function should return an integer indicating the
 * relation between two elements in the list
 *
 * @return
 * LIST_NULL_ARGUMENT if list or compareElement are NULL
 * LIST_OUT_OF_MEMORY if a memory allocation failed, the list will be intact
 * in this case.
 * LIST_SUCCESS if sorting completed successfully.
 */
ListResult listSort(List list, CompareListElements compareElement,
	ListSortKey key) {
	//if a NULL was sent as list or compareElement return LIST_NULL_ARGUMENT
	if(list == NULL || compareElement == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	//sort the new_list in ascending order according to compareElement
	//this sort algorithim works like bubble sort
	bool swapped = false; //bool to check if an element was swapped
	do {
		swapped = false;
		//loop over all elements in the new_list
		LIST_FOREACH(ListElement, element, list) {
			if(list->iterator->next != NULL) { //if not at last element
				//if the elements aren't in ascending order
				if(compareElement(element, list->next->data, key) == false) {
					swap(list->iterator, list->iterator->next);
					swapped = true; //the elements were swapped
					break; //restart the sort algorithim
				}
			}
		}
	} while(removed_element == true);
	return LIST_SUCCESS;
}
