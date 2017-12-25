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

void pushNodeToTheEndOfList(Node lastNode, Node list) {
    if(list == NULL) {
        return;
    }
    Node tempNode = list;
    while(tempNode->next != NULL) {
        tempNode = tempNode->next;
    }
    tempNode->next = lastNode;
}

//deep copies a list into a newly created list in reversed order
//param1 - Node list: the list to be copied
//return: a list that is a reversed deep copy of the parameter "list"
Node listCopyReversed(Node list) {
    if(list == NULL) { //if the list is empty the return an empty list(NULL)
        return NULL; //return an empty list
    }
    else if(list->next == NULL) {
        Node lastNode = createNode(list->n);
        if(lastNode == NULL) { //if memory allocation failed
            destroyList(lastNode); //destroy new node
            return NULL; //return an empty list
        }
        return lastNode;
    }
    else {
        Node return_node = listCopyReversed(list->next);
        if(return_node == NULL) { //memory allocation failed
            return NULL; //return an empty list
        }
        Node lastNode = createNode(list->n);
        if(lastNode == NULL) { //if memory allocation failed
            destroyList(lastNode); //destroy new node
            destroyList(return_node); //destroy all new nodes
            return NULL; //return an empty list
        }
        pushNodeToTheEndOfList(lastNode, return_node);
        return return_node;
    }
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
    node1->n = 1;
    node1->next = node2;
    node2->n = 3;
    node2->next = node3;
    node3->n = 0;
    node3->next = node4;
    node4->n = -2;
    node4->next = node5;
    node5->n = 17;
    node5->next = NULL;

    Node new_node= listCopyReversed(node1); //testing.
    printNode(node1);
    printNode(new_node); //should print the same lists.
    return 0;
}
