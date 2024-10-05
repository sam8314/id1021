 #include <stdio.h>
#include <stdlib.h>

/* QUEUE STRUCTURE */
typedef struct queue {
    int *arr;        
    int first;          //idx first used slot
    int last;           //idx last used slot  
    int nbOfElements;
    int nbOfSlots;   
} queue;

queue *create_queue(int nbOfSlots) {
    queue *q = (queue *)malloc(sizeof(queue));
    q->arr = (int *)malloc(nbOfSlots*sizeof(int));
    q->first = 0;
    q->last = -1;
    q->nbOfElements = 0;
    q->nbOfSlots = nbOfSlots;
    return q;
}

int empty(queue *q) {
    return q->nbOfElements == 0;
}

int full(queue *q) {
    return q->nbOfElements == q->nbOfSlots;
}

/*RESIZING*/
void double_queue(queue *q) {
    int new_nbOfSlots = q->nbOfSlots * 2;
    int *new_arr = (int *)malloc(new_nbOfSlots*sizeof(int));

    for (int i = 0; i < q->nbOfElements; i++) {
        new_arr[i] = q->arr[q->first + i]; 
    }

    free(q->arr);
    q->arr = new_arr;
    q->first = 0;
    q->last = q->nbOfElements -1;
    q->nbOfSlots = new_nbOfSlots;
}

void half_queue(queue *q) {
    if (q->nbOfSlots <= 1) {
        return;  //don't shrink if only one slot
    }
    
    if (q->nbOfElements < q->nbOfSlots / 3) {
        int new_nbOfSlots = q->nbOfSlots / 2;
        if (new_nbOfSlots < 1) new_nbOfSlots = 1;

        int *new_arr = (int *)malloc(new_nbOfSlots * sizeof(int));

        for (int i = 0; i < q->nbOfElements; i++) {
            new_arr[i] = q->arr[q->first + i];
        }

        free(q->arr);
        q->arr = new_arr;
        q->nbOfSlots = new_nbOfSlots;

        q->first = 0;
        q->last = q->nbOfElements - 1;
    }
}

/*ENQUEUEING & DEQUEUEING*/
void enqueue(queue *q, int v) {
    if (full(q)) {double_queue(q);}
    q->last++;
    q->arr[q->last] = v;
    q->nbOfElements++;
}

int dequeue(queue *q) {
    if (empty(q)) {
        printf("Queue is empty\n");
        return -1;
    }
    int value = q->arr[q->first];
    q->first++;
    q->nbOfElements--;
    half_queue(q); //shrink if needed
    if (q->nbOfElements == 0) {
        q->first = 0;
        q->last = -1;
    }
    return value;
}

/*TESTS*/
void print_queue(queue *q) {
    if (empty(q)) {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue elements: ");
    printf("[");
    for (int i = q->first; i <= q->last; i++) {
        printf("%d ", q->arr[i]);
    }
    for (int i = q->last+1; i<=q->nbOfSlots; i++){
        printf("_ ");
    }
    printf("]\n");
}

int main() {
    queue *q = create_queue(4);

    // Enqueue elements and print the queue
    for (int i = 10; i <= 40; i += 10) enqueue(q, i);  // Enqueues 10, 20, 30, 40
    printf("Initial queue:\n\t"); print_queue(q);

    // Enqueue more to trigger resize
    enqueue(q, 50); 
    printf("After 1 enqueueing (and double size):\n\t"); print_queue(q);

    // Dequeue a few elements
    dequeue(q); dequeue(q);  
    printf("After 2 dequeueing:\n\t"); print_queue(q);

    // Enqueue a few more elements
    enqueue(q, 60); enqueue(q, 70);
    printf("After 2 enqueueings:\n\t"); print_queue(q);

    // Dequeue more to trigger shrinking
    dequeue(q); dequeue(q); dequeue(q);
    printf("After 3 dequeueings (and shrinking):\n\t"); print_queue(q);

    // Clean up
    free(q->arr);
    free(q);

    return 0;
}

