#include <stdlib.h>
#include "util.h"

Element getElement(Set set, Element element, compareElements compare) {
	if(set == NULL || element == NULL || compare == NULL) {
		return NULL;
	}
	SET_FOREACH(SetElement, setElement, set) {
		if(compare(element, setElement) == 0) {
			return setElement;
		}
	}
	return NULL;
}

List copySetToList(Set set, filterElement filter) {
	if(set == NULL) {
		return NULL;
	}
	List list = listCreate(copyPointer, freePointer);
	if(list == NULL) {
		return NULL;
	}
	SET_FOREACH(SetElement, element, set) {
		if(filter == NULL) {
			if(listInsertFirst(list, element) != LIST_SUCCESS) {
				listDestroy(list);
				return NULL;
			}
		}
		else {
			if(listInsertFirst(list, filter(element)) != LIST_SUCCESS) {
				listDestroy(list);
				return NULL;
			}
		}
	}
	return list;
}

Element copyPointer(Element pointer) {
	return pointer;
}

void freePointer(Element pointer) {
	//nothing to free because the pointer was not dynamically allocated
}
