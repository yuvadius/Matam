#include <stdio.h>

typedef struct node_t* Node;
    struct node_t {
    int n;
    Node next;
};

Node createNode(int n) {
    Node ptr = malloc(sizeof(*ptr));
    if(!ptr) {
        return NULL;
    }
    ptr->n = n;
    ptr->next = NULL;
    return ptr;
}

void destroyList(Node ptr) {
    while(ptr) {
        Node toDelete = ptr;
        ptr = ptr->next;
        free(toDelete);
    }
}

//deep copies a list into a newly created list
//param1 - Node list: the list to be copied
//return: a list that is a deep copy of the parameter list
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


// Driver program to test above functions
int main()
{
    Node node1 = malloc(sizeof(*node1));
    Node node2 = malloc(sizeof(*node2));
    Node node3 = malloc(sizeof(*node3));
    Node node4 = malloc(sizeof(*node4));
    Node node5 = malloc(sizeof(*node5));
    node1->data = 1;
    node1->next = node2;
    node2->data = 3;
    node2->next = node3;
    node3->data = 0;
    node3->next = node4;
    node4->data = -2;
    node4->next = node5;
    node5->data = 17;
    node5->next = NULL;
    



    Node new_node= listCopy(node1); //testing.
    printNode(node1); 
    printNode(new_node); //should print the same lists.



    void printNode(Node list)
    {
        if(list!=NULL) {
            printf("The List: %d",list->n); // print the first node
            while(list->next!=NULL) { // while NOT the last node
                list=list->next;  // moving to the next node
                printf("%d ",list->n); // print the node
            }
        }

    }
}
