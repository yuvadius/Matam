#include <stdio.h>

// A utility function to swap two elements
static void swap(void *x, void *y, size_t l)
{
    char *a = x, *b = y, c;
    while(l--)
    {
        c = *a;
        *a++ = *b;
        *b++ = c;
    }
}

int cmpInt(void *a, void *b) {
    int *x = (int *)a;
    int *y = (int *)b;
    if ((*x) > (*y)) {
        return 1;
    }
    else {
        return 0;
    }
}

int cmpDbl(void *a, void *b) {
    double *x = (double *)a;
    double *y = (double *)b;
    if ((*x) > (*y)) {
        return 1;
    }
    else {
        return 0;
    }
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (void* arr, int low, int high, size_t size, int (*cmp)(void*,void*))
{
    void* pivot = arr + size*high;    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (cmp(pivot, arr + size*j))
        {
            i++;    // increment index of smaller element
            swap(arr + size*i, arr + size*j, size);
        }
    }
    swap(arr + size*(i+1), arr + size*high, size);
    return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(void* arr, int low, int high, size_t size, int (*cmp)(void*,void*))
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high, size, cmp);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1, size, cmp);
        quickSort(arr, pi + 1, high, size, cmp);
    }
}

// Driver program to test above functions
int main()
{
    int arr[] = {10.0f, 7.0f, 8.0f, 9.0f, 1.0f, 5.0f};
    int n = sizeof(arr)/sizeof(arr[0]);
    quickSort(arr, 0, n-1, sizeof(int), cmpInt);
    printf("%d ", arr[0]);
    printf("%d ", arr[1]);
    printf("%d ", arr[2]);
    printf("%d ", arr[3]);
    printf("%d ", arr[4]);
    printf("%d ", arr[5]);

	scanf("%d", &n);
    return 0;
}
