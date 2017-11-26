#include <stdlib.h>
#include <assert.h>
#include "Course.h"
#include "DynamicArray.h"

//------------------------------------------------------------------------------------------
// Create a new empty array
// returns the new array or NULL if can not create one
DynamicArray createDynamicArray() {
    DynamicArray dynamic_array = malloc(sizeof(*dynamic_array));
    if(!dynamic_array) { //could not create array
        return NULL;
    }
    dynamic_array->len = 0;
    dynamic_array->elements = NULL;
    return dynamic_array;
}

//------------------------------------------------------------------------------------------
// Enlarge the array and add a given element to the array in a position that appears
// just before a given existing index. First index in the array is 0, as in a regular array.
// the added element is inserted to the array by a simple assignment (without duplication).
// return values : DA_OK, DA_MEMORY_ERROR, DA_ILLEGAL_INDEX
DAResult addElementBefore(DynamicArray da, Element element, int index) {
    //if index is out of the array
    if(index < 0 || index >= da->len) {
        return DA_ILLEGAL_INDEX;
    }
    da->len++;
    da->elements = realloc(da->elements,da->len * sizeof(Element));
    if(da->elements == NULL) {
        return DA_MEMORY_ERROR;
    }
    for (int i = da->len - 1; i > index; --i) {
        da->elements[i] = da->elements[i - 1];
        da->elements[i - 1] = NULL;
    }
    da->elements[index] = element;
    return DA_OK;
}

//------------------------------------------------------------------------------------------
// Enlarge the array and add a given element to the array in a position at the start
// of the array. the added element is inserted to the array by a simple assignment (without duplication).
// return values : DA_OK, DA_MEMORY_ERROR.
DAResult addElementStart(DynamicArray da, Element element) {
    da->len++;
    da->elements = realloc(da->elements,da->len * sizeof(Element));
    if(da->elements == NULL) {
        return DA_MEMORY_ERROR;
    }
    for (int i = da->len - 1; i >= 1; --i) {
        da->elements[i] = da->elements[i - 1];
        da->elements[i - 1] = NULL;
    }
    da->elements[0] = element;
    return DA_OK;
}

//------------------------------------------------------------------------------------------
// Enlarge the array and add a given element to the array in a position at the end
// of the array. the added element is inserted to the array by a simple assignment (without duplication).
// return values : DA_OK, DA_MEMORY_ERROR.
DAResult addElementEnd(DynamicArray da, Element element) {
    da->len++;
    da->elements = realloc(da->elements,da->len * sizeof(Element));
    if(da->elements == NULL) {
        return DA_MEMORY_ERROR;
    }
    da->elements[da->len - 1] = element;
    return DA_OK;
}

//------------------------------------------------------------------------------------------
// result_index gets an index in the array da at which there exists an element that its id is equal to the id if a given element c.
// the result index is the first (lowest) index that is equal or greater to base_index.
// The comparison between 2 elements is done by using the function coursesEqualId that is guaranteed to be supplied by the data type Course.
// if No element is found, then result_index gets -1.
// da ,c and result_index must not be NULL. (checked by assert).
// return values : DA_OK, DA_ILLEGAL_INDEX.
DAResult indexOfElement(DynamicArray da, Element c , int base_index, int *result_index) {
    //if index is out of the array
    if(base_index < 0 || base_index >= da->len) {
        return DA_ILLEGAL_INDEX;
    }
    assert((da != NULL) && (c != NULL) && (result_index != NULL));
    for(int i = base_index; i < da->len; ++i) {
        if(coursesEqualId(c, da->elements[i])) {
            *result_index = i;
            return DA_OK;
        }
    }
    *result_index = -1;
    return DA_OK;
}

//------------------------------------------------------------------------------------------
// Delete an element (a course) at a given position (index)
// of the array. The size of the array is updated.
// return values : DA_OK, DA_MEMORY_ERROR , DA_ILLEGAL_INDEX
DAResult removeElement(DynamicArray da, int index) {
    if(index < 0 || index >= da->len) { //if index is out of the array
        return DA_ILLEGAL_INDEX;
    }
    for (int i = index; i < da->len - 1; i++) {
        da->elements[i] = da->elements[i + 1];
        da->elements[i + 1] = NULL;
    }
    da->len--;
    da->elements = realloc(da->elements,da->len * sizeof(Element));
    //if len is 0 then u are supposed to get a NULL pointer
    if(da->elements == NULL && da->len > 0) {
        return DA_MEMORY_ERROR;
    }
    return DA_OK;
}

//------------------------------------------------------------------------------------------
// update an existing element in a given position (index) to be identical
// to a given element. the given element must not be changed.
// no need to duplicate element. just do a simple assignment.
// return values : DA_OK, DA_MEMORY_ERROR , DA_ILLEGAL_INDEX
DAResult updateElement(DynamicArray da, int index, Element element) {
    if(index < 0 || index >= da->len) { //if index is out of the array
        return DA_ILLEGAL_INDEX;
    }
    da->elements[index] = element;
    return DA_OK;
}

//------------------------------------------------------------------------------------------
// display the element in the given index i.
// this function uses the displayCourse function that is guaranteed to exist in the Course data type.
// no need to do here '\n'.
// return values : DA_OK, DA_ILLEGAL_INDEX
DAResult displayElement(DynamicArray da, int i) {
    if(i < 0 || i >= da->len) { //if index is out of the array
        return DA_ILLEGAL_INDEX;
    }
    displayCourse(da->elements[i]);
    return DA_OK;
}

//------------------------------------------------------------------------------------------
// display all the elements in the array according to their position in the array, from left to right.
// each element is displayed in a single line and '\n at the end of the line.
// the elements are displayed by using the displayCourse function that is guaranteed to exist in the Course data type.
// return values : DA_OK, DA_ILLEGAL_INDEX
void displayAllElements(DynamicArray dynamic_array) {
    for(int i = 0; i < dynamic_array->len; ++i) {
        assert(dynamic_array->elements[i] != NULL);
        displayCourse(dynamic_array->elements[i]);
        printf("\n");
    }
}

//------------------------------------------------------------------------------------------
// returns the number of elements in the array.
// return values : CDA_OK
int size(DynamicArray dynamic_array) {
    return dynamic_array->len;
}

//------------------------------------------------------------------------------------------
// deallocate all relevant memory and stop using the array.
void destroyDynamicArray(DynamicArray dynamic_array) {
    free(dynamic_array->elements);
    free(dynamic_array);
}

//------------------------------------------------------------------------------------------
