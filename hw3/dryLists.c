#include <stdio.h>

typedef struct node_t* Node;
    struct node_t {
    int n;
    Node next;
};

//suumary: dynamically allocate a new node and copy the data into it
//param1 - int n: the data to be inserted into the new node
//return: the new node
Node createNode(int n) {
    Node ptr = malloc(sizeof(*ptr)); //create node
    if(!ptr) { //if the allocation failed then return NULL
        return NULL;
    }
    ptr->n = n; //set the data of the new node
    ptr->next = NULL; //set the next node to NULL for safety
    return ptr; //return the new node
}

void destroyList(Node ptr) {
    while(ptr) {
        Node toDelete = ptr;
        ptr = ptr->next;
        free(toDelete);
    }
}

//deep copies a list into a newly created list in the same order
//param1 - Node list: the list to be copied
//return: a list that is a deep copy of the parameter "list"
Node listCopy(Node list) {
    if(list == NULL) { //if the list is empty the return an empty list(NULL)
        return NULL; //return an empty list
    }
    else {
        Node return_node = createNode(list->n); //the head of the list
        Node next_new_node = return_node; //iterator for the new nodes
        Node next_old_node = list; //the iterator for the old nodes
        while(next_old_node->next != NULL) { //if end of the list was reached
            next_old_node = next_old_node->next; //go to next node
            next_new_node->next = createNode(next_old_node->n); //copy old node
            if(next_new_node->next == NULL) { //if memory allocation failed
                destroyList(return_node); //destroy all new nodes
                return NULL; //return an empty list
            }
            next_new_node = next_new_node->next; //go to next node

        }
        return return_node; //return the new list
    }
}

//pushes the "lastNode", to the end of the "list"
//param1 - Node lastNode: the node to be pushed to the end of "list"
//param2 - Node list: the list that the "lastNode" will be pushed into
//return: the head of the new list with the last_node at the end of the list
Node pushNodeToTheEndOfList(Node last_node, Node list) {
    if(list == NULL) { //if the list is empty then return the "last_node"
        return last_node;
    }
    Node temp_node = list;
    while(temp_node->next != NULL) { //loop until the end of the list
        temp_node = temp_node->next; //iterate to the next element in the list
    }
    //if last_node is NULL then the list will stay the same
    temp_node->next = last_node; //push the lastNode to the end of the list
    return list;
}

//deep copies a list into a newly created list in reversed order
//param1 - Node list: the list to be copied
//return: a list that is a reversed deep copy of the parameter "list"
Node listCopyReversed(Node list) {
    if(list == NULL) { //if the list is empty the return an empty list(NULL)
        return NULL; //return an empty list
    }
    else if(list->next == NULL) { //if the end of the original list was reached
        //make the last node in list the new first node in the list
        Node last_node = createNode(list->n);
        if(last_node == NULL) { //if memory allocation failed
            destroyList(last_node); //destroy new node
            return NULL; //return an empty list
        }
        return last_node; //return the head of the list
    }
    else {
        Node return_node = listCopyReversed(list->next);
        if(return_node == NULL) { //memory allocation failed
            return NULL; //return an empty list
        }
        Node last_node = createNode(list->n);
        if(last_node == NULL) { //if memory allocation failed
            destroyList(last_node); //destroy new node
            destroyList(return_node); //destroy all new nodes
            return NULL; //return an empty list
        }
        return_node = pushNodeToTheEndOfList(last_node, return_node);
        return return_node; //return the head of the list
    }
}

//summary: this function receives an array of lists and concatenates them
//one after the other, while lists in even indexes in the array will be
//concatenated in reversed order whill lists in odd indexes in the array will
//be concatenated in regular order 
//NOTE: the lists will be deep copied
//param1 - Node array_of_lists[]: an array of lists(lists will be concatenated)
//param2 - int n: size of the array "array_of_lists"
//return: the new concatenated list
Node listJoinAlternating(Node array_of_lists[], int n) {
    Node return_list = NULL; //start with an empty list
    for(int i = 0; i < n; ++i) { //loop over the array
        //no need to concatenate if the list is empty
        if(array_of_lists[i] != NULL) {
            if(i % 2 == 0) { //if i is even(odd if you start counting from 1)
                //retrieve the reversed list "array_of_lists[i]"
                Node temp_node = listCopyReversed(array_of_lists[i]);
                if(temp_node == NULL) { //if memory allocation failed
                    destroyList(return_list); //destroy all new nodes
                    return NULL; //return an empty list
                }
                //concatenate the list to the end
                return_list = pushNodeToTheEndOfList(temp_node, return_list);
            }
            else {
                //retrieve a copy of the list "array_of_lists[i]"
                Node temp_node = listCopy(array_of_lists[i]);
                if(temp_node == NULL) { //if memory allocation failed
                    destroyList(return_list); //destroy all new nodes
                    return NULL; //return an empty list
                }
                //concatenate the list to the end
                return_list = pushNodeToTheEndOfList(temp_node, return_list);
            }
        }
    }
    return return_list; //return the head of the list
}

void printNode(Node list)
{
    if(list!=NULL) {
        printf("The List: %d ",list->n); // print the first node
        while(list->next!=NULL) { // while NOT the last node
            list=list->next;  // moving to the next node
            printf("%d ",list->n); // print the node
        }
    }

}


// Driver program to test above functions
int main()
{
    Node node1 = malloc(sizeof(*node1));
    Node node2 = malloc(sizeof(*node2));
    Node node3 = malloc(sizeof(*node3));
    Node node4 = malloc(sizeof(*node4));
    Node node5 = malloc(sizeof(*node5));
    Node node6 = malloc(sizeof(*node3));
    Node node7 = malloc(sizeof(*node4));
    Node node8 = malloc(sizeof(*node5));
    node1->n = 1;
    node1->next = node2;
    node2->n = 2;
    node2->next = NULL;
    node3->n = 3;
    node3->next = node4;
    node4->n = 4;
    node4->next = NULL;
    node5->n = 5;
    node5->next = node6;
    node6->n = 6;
    node6->next = NULL;
    node7->n = 7;
    node7->next = node8;
    node8->n = 8;
    node8->next = NULL;
    Node arr[] = {node1, node3, node5, node7};

    Node new_node= listJoinAlternating(arr, 4); //testing.
    //printNode(node1);
    printNode(new_node); //should print the same lists.
    return 0;
}
