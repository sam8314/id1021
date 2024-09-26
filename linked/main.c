#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

/*HELPER FUCNTIONS*/
// Function for calculating mean found at geeks for geeks
long findMean(long a[], int n)
{
    long sum = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];
 
    return (long)sum / (long)n;
}

//Function for finding minimum
long findMin(long a[], int n)
{
    long res = a[0];
    for(int i=1; i<n; i++)
    {
        if (a[i]<res){res = a[i];}
    }
    return res;
}

//Function for finding maximum
long findMax(long a[], int n)
{
    long res = a[0];
    for(int i=1; i<n; i++)
    {
        if (a[i]>res){res = a[i];}
    }
    return res;
}

//Function to help measure time
long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
            (t_stop->tv_sec - t_start->tv_sec)*1000000000;
} 


/*LINKED LIST STRUCTURE*/
typedef struct cell {
    int value;
    struct cell *tail;
} cell;

typedef struct linked {
    cell *first;
} linked;

linked *create_linked() {
    linked *new = (linked *)malloc(sizeof(linked));
    if (!new) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new->first = NULL;
    return new;
}

void free_linked(linked *lnk) {
    if (lnk == NULL) return;
    cell *nxt = lnk->first;
    while (nxt != NULL) {
        cell *tmp = nxt->tail;
        free(nxt);
        nxt = tmp;
    }
    free(lnk);
}

//Function that adds an item as the first cell in the list
void add(linked *lnk, int item) {
    cell *new_cell = (cell *)malloc(sizeof(cell));
    if (!new_cell) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_cell->value = item;
    new_cell->tail = lnk->first;
    lnk->first = new_cell;
}

//Function that returns the length of the list
int length(linked *lnk) {
    int count = 0;
    cell *current = lnk->first;
    while (current != NULL) {
        count++;
        current = current->tail;
    }
    return count;
}

//Function that returns true if the item is found in the list
bool find(linked *lnk, int item) {
    cell *current = lnk->first;
    while (current != NULL) {
        if (current->value == item) {
            return true;
        }
        current = current->tail;
    }
    return false;
}

//Function that removes an existing item from the list
void remove_item(linked *lnk, int item) {
    cell *current = lnk->first;
    cell *previous = NULL;
    while (current != NULL) {
        if (current->value == item) {
            if (previous == NULL) {lnk->first = current->tail;}             
            else {previous->tail = current->tail;}
            free(current);
            return;
        }
        previous = current;
        current = current->tail;
    }
}

//Function that appends one linked list to another
void append(linked *a, linked *b) {
    if (a == NULL || b == NULL) return;
    if (a->first == NULL) {
        a->first = b->first;
        return;
    }
    cell *current = a->first;
    while (current->tail != NULL) {current = current->tail;}
    current->tail = b->first;
}

//Function that generates a linked lists of length n
linked *init_list(int n) {
    linked *a = create_linked();
    for (int i = 0; i < n; i++) {
        add(a, i);
    }
    return a;
}

/*BENCHMARK append()*/
// Benchmark function with varying size of list 'a'
long benchmark1(int n) {
    struct timespec t_start, t_stop;
    linked *a = init_list(n);
    linked *b = init_list(100);

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    append(a, b);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    free_linked(a);
    free(b);
    
    return wall;
}

long benchmark2(int n) {
    struct timespec t_start, t_stop;
    linked *a = init_list(100);
    linked *b = init_list(n);
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    append(a, b);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);

    free_linked(a); 
    free(b);    
    return wall;
}

int main() {
    int sizes[] = {100, 200, 300, 400, 500, 600, 700, 800, 900,
                    1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
                    10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000,
                    100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000,
                    1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000,
                    10000000};

    benchmark1(9000000); //warmup

    for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++) {
        int n = sizes[i];
        printf("%ld\n", benchmark1(n));
    }
    
    return 0;
}
