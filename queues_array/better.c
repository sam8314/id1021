#include <stdio.h>
#include <stdlib.h>

/* QUEUE STRUCTURE */
typedef struct queue {
    int *arr;
    int first;          
    int last;           
    int nbOfElements;   
    int nbOfSlots;      
} queue;

queue *create_queue(int nbOfSlots) {
    queue *q = (queue *)malloc(sizeof(queue));
    q->arr = (int *)malloc(nbOfSlots * sizeof(int));
    q->first = 0;
    q->last = 0;        // start at 0
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

/* RESIZING */
void double_queue(queue *q) {
    int new_nbOfSlots=q->nbOfSlots*2;
    int *new_arr=(int *)malloc(new_nbOfSlots*sizeof(int));

    for (int i = 0; i < q->nbOfElements; i++) {
        new_arr[i] = q->arr[(q->first + i)%q->nbOfSlots];
    }

    free(q->arr);
    q->arr = new_arr;
    q->first = 0;
    q->last = q->nbOfElements;
    q->nbOfSlots = new_nbOfSlots;
}

void half_queue(queue *q) {
    if (q->nbOfSlots <= 1) {return;}

    if (q->nbOfElements<q->nbOfSlots/3){
        int new_nbOfSlots=q->nbOfSlots/2;
        if (new_nbOfSlots<1) new_nbOfSlots=1;

        int *new_arr = (int *)malloc(new_nbOfSlots*sizeof(int));
        for (int i = 0; i < q->nbOfElements; i++) {
            new_arr[i] = q->arr[(q->first + i)%q->nbOfSlots];
        }

        free(q->arr);
        q->arr = new_arr;
        q->nbOfSlots = new_nbOfSlots;
        q->first = 0;
        q->last = q->nbOfElements;
    }
}

/* ENQUEUEING & DEQUEUEING*/
void enqueue(queue *q, int v) {
    if (full(q)) {double_queue(q);}
    q->arr[q->last] = v;
    q->last = (q->last + 1) % q->nbOfSlots;//wrap
    q->nbOfElements++;
}

int dequeue(queue *q) {
    if (empty(q)) {
        printf("Queue is empty\n");
        return -1;
    }

    int value = q->arr[q->first];
    q->first = (q->first + 1) % q->nbOfSlots;//wrap
    q->nbOfElements--;
    half_queue(q);
    if (q->nbOfElements == 0) {
        q->first = 0;
        q->last = 0;
    }
    return value;
}

/*TESTS*/
void print_queue(queue *q) {
    printf("Memory block: [");
    for (int i =0; i<q->nbOfSlots;i++){
        if(q->arr[i]==0) printf("_ ");
        else {printf("%d ", q->arr[i]);}
    }
    printf("]\n\t");

    if (empty(q)) {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue elements: [");
    for (int i = 0; i < q->nbOfElements; i++) {
        printf("%d ", q->arr[(q->first + i) % q->nbOfSlots]);
    }
    printf("]\n");
}



int main() {
    queue *q = create_queue(4);    
    printf("Initial queue:\n\t");
    print_queue(q);

    enqueue(q, 10); enqueue(q, 20); enqueue(q, 30); 
    printf("After 3 enqueues:\n\t");
    print_queue(q);

    enqueue(q, 40); enqueue(q, 50);
    printf("After 2 enqueue:\n\t");
    print_queue(q);

    dequeue(q); dequeue(q);  
    printf("After 2 dequeues:\n\t");
    print_queue(q);


    enqueue(q, 60); enqueue(q, 70); enqueue(q, 80); enqueue(q, 90);
    printf("After 4 enqueues:\n\t");
    print_queue(q);

    dequeue(q); dequeue(q); dequeue(q); dequeue(q); dequeue(q);  
    printf("After 5 dequeues:\n\t");
    print_queue(q);


    enqueue(q, 100); enqueue(q, 110);
    printf("After 2 enqueues:\n\t");
    print_queue(q);

    free(q->arr);
    free(q);

    return 0;
}


