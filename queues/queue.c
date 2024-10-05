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

/*QUEUE STRUCUTRE*/
typedef struct node {
    int value;
    struct node *next;
} node;

typedef struct queue {
    node *first;
} queue;

queue *create_queue() {
    queue *q = (queue*)malloc(sizeof(queue));
    q->first = NULL;
    return q;
}

int empty(queue *q) {
    return q->first == NULL;
}

void enque(queue* q, int v) {
    node *nd = (node*)malloc(sizeof(node));
    nd->value = v;
    nd->next = NULL;

    node *prv = NULL;
    node *nxt = q->first;

    while (nxt != NULL) {
        prv = nxt;
        nxt = nxt->next; 
    }

    if (prv != NULL) {
        prv->next = nd;
    } else {
        q->first = nd;
    }
}

int dequeue(queue *q) {
    int res = 0;

    if (q->first != NULL) { 
        node *temp = q->first;
        res = temp->value;
        q->first = temp->next;
        free(temp);
    }
    return res;//0 if emty queue
}

/*BENCHMARK*/
long bench_enque(int n) {
    struct timespec t_start, t_stop;
    queue *q = create_queue();

    for(int i =0; i<n;i++){
        enque(q,rand()%100);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    enque(q,10);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);

    free(q);
    return wall;
}

long bench_dequeue(int n) {
    struct timespec t_start, t_stop;
    queue *q = create_queue();

    for(int i =0; i<n;i++){
        enque(q,rand()%100);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    dequeue(q);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);

    free(q);
    return wall;
}

int main() {

    int sizes[] = {100, 200, 300, 400, 500, 600, 700, 800, 900,
                    1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
                    10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000,
                    100000};

    bench_enque(90000); //warmup

    for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++) {
        int n = sizes[i];
        printf("%ld\n", bench_dequeue(n));
    }
    
    return 0;
}
