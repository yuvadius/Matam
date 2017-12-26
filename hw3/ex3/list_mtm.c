#include "list_mtm.h"

/** The List is implemented as a List of Elements.
* With iterator as an index to the current Element or NULL
*/
struct List_t {
	ListElement iterator;
	CopyListElement copyElement;
	FreeListElement freeElement;
};

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
	if (!copyFunction || !freeFunction) { //if one of the parameters is NULL
		return NULL;
	}
	List list = malloc(sizeof(*list)); //create a new List
	if (list == NULL) { //if memory allocation failed
		return NULL;
	}
	list->iterator = NULL;
	list->copyElement = copyElement;
	list->freeElement = freeElement;
	return stack;
}
