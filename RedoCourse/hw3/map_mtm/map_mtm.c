#include "map_mtm.h"
#include <stdlib.h>

typedef struct Node {
	MapKeyElement key;
	MapDataElement data;
	struct Node* next;
} *Node;

struct Map_t {
	Node head;
	Node iterator;
	copyMapKeyElements copyKeyElement;
	copyMapDataElements copyDataElement;
	freeMapKeyElements freeKeyElement;
	freeMapDataElements freeDataElement;
	compareMapKeyElements compareKeyElements;
};

/**
 * Creates a Node.
 *
 * The new node will contain the "key", "data" and the "next" given as parameters
 *
 * @param key The target key to copy into the new node
 * @param data The target data to copy into the new node
 * a copy of data(using map->copyDataElement) will be copied into the new node
 * @param next The target next to copy into the new node
 * @param map The map's copy function will be used to create a copy of key, data
 * @return
 * NULL if key is NULL data is NULL or map is NULL or a memory allocation failed
 * otherwise return the new node
 */
static Node createNode(MapKeyElement key, MapDataElement data, Node next, Map map) {
	//if the key or data or map is NULL then return NULL
	if(key == NULL || data == NULL || map == NULL) {
		return NULL;
	}
	Node node = malloc(sizeof(*node)); //create a new node
	//if there was a memory allocation error then return NULL
	if(node == NULL) {
		return NULL;
	}
	//make a copy of the "key" using the map'ss copy function
	MapKeyElement new_key = map->copyKeyElement(key);
	//if there was a memory error in the copy function then return NULL
	if(new_key == NULL) {
		free(node); //free the previously allocated node
		return NULL;
	}
	//make a copy of the "data" using the list's copy function
	MapDataElement new_data = map->copyDataElement(data);
	//if there was a memory error in the copy function then return NULL
	if(new_data == NULL) {
		map->freeKeyElement(new_key); //free the previously allocated key
		free(node); //free the previously allocated node
		return NULL;
	}
	node->key = new_key; //set the node's key to the copied parameter "key"
	node->data = new_data; //set the node's data to the copied parameter "data"
	node->next = next; //set the node's next to the parameter "next"
	return node; //return the new node
}

/**
 * Type of function for deallocating a Node
 * @param1 node The node to be freed
 * @param2 map The Map that contains the freeElement function to be used
 * for freeing the node->key, node->data */
static void freeNode(Node node, Map map) {
	//if the node is NULL then there is nothing to free
	//if the map is NULL then there is no freeElement function
	if(node == NULL || map == NULL) {
		return;
	}
	map->freeKeyElement(node->key); //free the map key of the node
	map->freeDataElement(node->data); //free the map data of the node
	free(node); //free the node
}

Map mapCreate(copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement,
	freeMapDataElements freeDataElement, freeMapKeyElements freeKeyElement,
	compareMapKeyElements compareKeyElements) {
	//if one of the parameters is set to NULL
	if(copyDataElement == NULL || copyKeyElement == NULL || freeDataElement == NULL
		|| freeKeyElement == NULL || compareKeyElements == NULL) {
		return NULL;
	}
	Map map = malloc(sizeof(*map));
	if(map == NULL) { //if there was a memory allocation failure
		return NULL;
	}
	map->head = NULL;
	map->iterator = NULL;
	map->copyKeyElement = copyKeyElement;
	map->copyDataElement = copyDataElement;
	map->freeKeyElement = freeKeyElement;
	map->freeDataElement = freeDataElement;
	map->compareKeyElements = compareKeyElements;
	return map;
}

void mapDestroy(Map map) {
	if(map != NULL && mapClear(map) == MAP_SUCCESS) {
		free(map);
	}
}

Map mapCopy(Map map) {
	if(map == NULL) {
		return NULL;
	}
	Map mapCopy = mapCreate(map->copyDataElement, map->copyKeyElement, map->freeDataElement, map->freeKeyElement, map->compareKeyElements);
	if(mapCopy == NULL) {
		map->iterator = NULL;
		return NULL;
	}
	MAP_FOREACH(MapKeyElement, iterator, map) {
		if(mapPut(mapCopy, map->iterator->key, map->iterator->data) != MAP_SUCCESS) {
			mapDestroy(mapCopy);
			map->iterator = NULL;
			return NULL;
		}
	}
	map->iterator = NULL;
	mapCopy->iterator = NULL;
	return mapCopy;
}

int mapGetSize(Map map) {
	//if the map is NULL we'll return -1.
	if(map == NULL) {
		return -1;
	}
	int size = 0;
	MAP_FOREACH(MapKeyElement, iterator, map) {
		++size;
	}
	map->iterator = NULL;
	return size;
}

MapResult mapClear(Map map) {
	if(map == NULL) {
		return MAP_NULL_ARGUMENT;
	}
	MapKeyElement iterator;
	do { //loop while there are elements to remove
		iterator = mapGetFirst(map); //set the iterator to the first element
	} while(mapRemove(map, iterator) == MAP_SUCCESS);
	map->iterator = NULL;
	return MAP_SUCCESS;
}

bool mapContains(Map map, MapKeyElement element) {
	if(map == NULL || element == NULL) {
		return false;
	}
	MAP_FOREACH(MapKeyElement, iterator, map) {
		if(map->compareKeyElements(element, iterator) == 0) {
			map->iterator = NULL;
			return true;
		}
	}
	map->iterator = NULL;
	return false; //No element found
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
	if(map == NULL || keyElement == NULL || dataElement == NULL)
		return MAP_NULL_ARGUMENT;
	Node lastNode = map->head;
	MAP_FOREACH(MapKeyElement, iterator, map) {
		if(map->compareKeyElements(keyElement, iterator) < 0) {
			Node node = createNode(keyElement, dataElement, map->iterator, map);
			if(node == NULL) {
				map->iterator = NULL;
				return MAP_OUT_OF_MEMORY;
			}
			else if(map->compareKeyElements(map->head->key, iterator) == 0)
				map->head = node;
			else
				lastNode->next = node;
			map->iterator = NULL;
			return MAP_SUCCESS;
		}
		else if(map->compareKeyElements(keyElement, iterator) == 0) {
			MapDataElement newDataElement = map->copyDataElement(dataElement);
			if(newDataElement == NULL) {
				map->iterator = NULL;
				return MAP_OUT_OF_MEMORY;
			}
			map->freeDataElement(map->iterator->data); //free data with same key
			map->iterator->data = newDataElement;
			map->iterator = NULL;
			return MAP_SUCCESS;
		}
		lastNode = map->iterator;
	}
	Node node = createNode(keyElement, dataElement, NULL, map);
	if(node == NULL) {
		map->iterator = NULL;
		return MAP_OUT_OF_MEMORY;
	}
	else if(map->head == NULL) //if map is empty
		map->head = node;
	else
		lastNode->next = node;
	map->iterator = NULL;
	return MAP_SUCCESS;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement) {
	MapDataElement data = NULL;
	if(map != NULL && keyElement != NULL) {
		Node oldIterator = map->iterator;
		MAP_FOREACH(MapKeyElement, iterator, map) {
			if(map->compareKeyElements(keyElement, iterator) == 0) {
				data = map->iterator->data;
				break;
			}
		}
		map->iterator = oldIterator; //Iterator status unchanged
	}
	return data;
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
	if(map == NULL || keyElement == NULL)
		return MAP_NULL_ARGUMENT;
	Node lastNode = NULL;
	MAP_FOREACH(MapKeyElement, iterator, map) {
		if(map->compareKeyElements(keyElement, iterator) == 0) {
			if(map->compareKeyElements(map->head->key, iterator) == 0)
				map->head = map->iterator->next;
			else
				lastNode->next = map->iterator->next;
			freeNode(map->iterator, map);
			map->iterator = NULL;
			return MAP_SUCCESS;
		}
		lastNode = map->iterator;
	}
	map->iterator = NULL;
	return MAP_ITEM_DOES_NOT_EXIST;
}

MapKeyElement mapGetFirst(Map map) {
	if(map == NULL || map->head == NULL) {
		return NULL;
	}
	map->iterator = map->head; //Set iterator to head
	return map->iterator->key; //Return first key
}

MapKeyElement mapGetNext(Map map) {
	if(map != NULL && map->iterator != NULL) {
		map->iterator = map->iterator->next; //Set iterator to the next element
		if(map->iterator != NULL) {
			return map->iterator->key; //Return iterator key
		}
	}
	return NULL;
}
