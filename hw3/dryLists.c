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
    return ptr;
}

Node listCopy(Node list) {
    if(list == NULL) {
        return NULL;
    }
    else {
        Node returnNode = malloc(sizeof(*node));
        returnNode->n = list->n;
        returnNode->next = NULL;
        Node nextNewNode = returnNode;
        Node nextOldNode = list;
        while(nextOldNode->next != NULL) {
            nextNewNode->next = malloc(sizeof(*node));
            nextNewNode = nextNewNode->next;
            nextOldNode = nextOldNode->next;
            nextNewNode->n = nextOldNode->n;

        }
        return returnNode;
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
