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
    node *head;  // front of the queue
    node *tail;   // end of the queue
} queue;

queue *create_queue() {
    queue *q = (queue*)malloc(sizeof(queue));
    q->head = NULL;
    return q;
}

int empty(queue *q) {
    return q->head == NULL;
}

void enque(queue* q, int v) {
    node *new_node = (node*)malloc(sizeof(node));
    new_node->value = v;
    new_node->next = NULL;

    if (q->head == NULL) {
        q->head = new_node;
        q->tail = new_node;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
}

int dequeue(queue *q) {
    int res = 0;

    if (q->head != NULL) { 
        node *temp = q->head;
        res = temp->value;
        q->head = temp->next;
        free(temp);
    }
    return res;//0 if emty queue
}

/*BENCHMARK*/
long bench_enqueue(int n) {
    struct timespec t_start, t_stop;
    queue *q = create_queue();

    for(int i =0; i<n;i++){
        enqueue(q,rand()%100);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    enqueue(q,10);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);

    free(q);
    return wall;
}

long bench_dequeue(int n) {
    struct timespec t_start, t_stop;
    queue *q = create_queue();

    for(int i =0; i<n;i++){
        enqueue(q,rand()%100);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    dequeue(q);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);

    free(q);
    return wall;
}

int main() {
    int numberOfExperiments = 20;
    int sizes[] = {100, 200, 300, 400, 500, 600, 700, 800, 900,
                    1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
                    10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000,
                    100000};
    long results[numberOfExperiments];

    bench_enqueue(90000); //warmup

    for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++) {
        int n = sizes[i];
        for (int j = 0;j<numberOfExperiments;j++){
            results[j]= bench_dequeue(n);
        }
        printf("%ld\n", findMin(results, numberOfExperiments));
    }
    
    return 0;
}
