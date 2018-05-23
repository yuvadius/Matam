#ifndef UTIL_H
#define UTIL_H

#include "list.h"
#include "set.h"

/**
 * Useful functions
 */

/** Element data type */
typedef void* Element;

/**
* Type of function used by the set to identify equal elements.
* This function will be used to deciding the iteration order of the set.
* This function should return:
* 		A positive integer if the first element is greater;
* 		0 if they're equal;
*		A negative integer if the second element is greater.
*/
typedef int(*compareElements)(Element, Element);

/**
* Type of function used by the copySetToList to manipulate the list element
* For example if we have a set of users and we want to a list of user names
* this function will receive a user and return the username
* If this will equal NULL then no filter action will occur
*/
typedef Element(*filterElement)(Element);

/**
 * This function will return the element in the set that is equal to the
 * parameter "element" as determined by the compare function
 * This function will return NULL on error or if no user was found
 */
Element getElement(Set set, Element element, compareElements compare);

/**
 * Copy set to list and then return the list
 * Will return NULL on error
 */
List copySetToList(Set set, filterElement filter);

/**
 * Copy a pointer and return it
 */
Element copyPointer(Element pointer);

/**
 * Free the pointer
 */
void freePointer(Element pointer);

#endif
