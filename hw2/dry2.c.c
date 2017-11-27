// Recursive program to check if a given linked list is palindrome
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Link list node */
typedef struct node_t* Node;
    struct node_t {
    int data;
    Node next;
};

// Initial parameters to this function are &head and head
bool isPalindromeUtil(struct node_t **left, struct node_t *right)
{
    /* stop recursion when right becomes NULL */
    if (right == NULL)
        return true;

    /* If sub-list is not palindrome then no need to
        check for current left and right, return false */
    bool isp = isPalindromeUtil(left, right->next);
    if (isp == false)
        return false;

    /* Check values at current left and right */
    bool isp1 = (right->data == (*left)->data);

    /* Move left to next node */
    *left = (*left)->next;

    return isp1;
}

// A wrapper over isPalindromeUtil()
bool isPalindrome(struct node_t *head)
{
    isPalindromeUtil(&head, head);
}

/* Push a node to linked list. Note that this function
changes the head */
void push(struct node_t** head_ref, int new_data)
{
	/* allocate node */
	struct node_t* new_node = (struct node*) malloc(sizeof(struct node_t));

	/* put in the data */
	new_node->data = new_data;

	/* link the old list off the new node */
	new_node->next = (*head_ref);

	/* move the head to pochar to the new node */
	(*head_ref) = new_node;
}

// A utility function to print a given linked list
void printList(struct node_t *ptr)
{
	while (ptr != NULL)
	{
		printf("%d->", ptr->data);
		ptr = ptr->next;
	}
	printf("NULL\n");
}

/* Drier program to test above function*/
int main()
{
	/* Start with the empty list */
	struct node* head = NULL;
	int str[] = {1,0,3,3,0,1};
	for (int i = 0; i < 6; i++)
	{
        push(&head, str[i]);
	}
    printf("%d", isPalindrome(head));
	return 0;
}

