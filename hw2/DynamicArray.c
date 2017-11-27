#include <stdlib.h>
#include <assert.h>
#include "Course.h"
#include "DynamicArray.h"
#include <stdio.h>

DynamicArray createDynamicArray() {
    DynamicArray dynamic_array = malloc(sizeof(*dynamic_array));
    if(dynamic_array == NULL) { //could not create array
        return NULL;
    }
    dynamic_array->len = 0;
    dynamic_array->elements = NULL;
    return dynamic_array;
}

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
    //shift all array elements after "element index" one to the left
    for (int i = da->len - 1; i > index; --i) {
        da->elements[i] = da->elements[i - 1];
        da->elements[i - 1] = NULL;
    }
    da->elements[index] = element;
    return DA_OK;
}

DAResult addElementStart(DynamicArray da, Element element) {
    da->len++;
    da->elements = realloc(da->elements,da->len * sizeof(Element));
    if(da->elements == NULL) {
        return DA_MEMORY_ERROR;
    }
    //shift all array elements one to the left
    for (int i = da->len - 1; i >= 1; --i) {
        da->elements[i] = da->elements[i - 1];
        da->elements[i - 1] = NULL;
    }
    da->elements[0] = element;
    return DA_OK;
}

DAResult addElementEnd(DynamicArray da, Element element) {
    da->len++;
    da->elements = realloc(da->elements,da->len * sizeof(Element));
    if(da->elements == NULL) {
        return DA_MEMORY_ERROR;
    }
    da->elements[da->len - 1] = element;
    return DA_OK;
}

DAResult indexOfElement(DynamicArray da, Element c , int base_index,
                        int *result_index) {
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

DAResult removeElement(DynamicArray da, int index) {
    if(index < 0 || index >= da->len) { //if index is out of the array
        return DA_ILLEGAL_INDEX;
    }
    //shift array elements after(and including)"element index" one to the left
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

DAResult updateElement(DynamicArray da, int index, Element element) {
    if(index < 0 || index >= da->len) { //if index is out of the array
        return DA_ILLEGAL_INDEX;
    }
    da->elements[index] = element;
    return DA_OK;
}

DAResult displayElement(DynamicArray da, int i) {
    if(i < 0 || i >= da->len) { //if index is out of the array
        return DA_ILLEGAL_INDEX;
    }
    displayCourse(da->elements[i]);
    return DA_OK;
}

void displayAllElements(DynamicArray dynamic_array) {
    for(int i = 0; i < dynamic_array->len; ++i) {
        assert(dynamic_array->elements[i] != NULL);
        displayCourse(dynamic_array->elements[i]);
        printf("\n");
    }
}

int size(DynamicArray dynamic_array) {
    return dynamic_array->len;
}

void destroyDynamicArray(DynamicArray dynamic_array) {
    free(dynamic_array->elements);
    free(dynamic_array);
}