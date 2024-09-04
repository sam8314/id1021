/*
This code was written on 04/09/2024 by Sam Serbouti
from the work of Johan Montelius, teacher of
Algorithms and data structures ID1021 at KTH
*/

#include <stdlib.h>
#include <stdio.h>

typedef struct stack {
    int top;
    int size;
    int *array;
} stack;

stack *new_stack(int size) {
    int *array = (int*)malloc(size*sizeof(int));
    stack *stk = (stack*)malloc(sizeof(stack));
    
    stk->top=0; //initialize top to 0
    stk->size=size; //set stack size
    stk->array=array; //assign array to stack
    return stk;
}

void push(stack *stk, int val) {
    if(stk->top < stk->size){ //check for overflow
        stk->array[stk->top] = val; //insert value
        stk->top++;  //increment top
    } 

    else{printf("Stack overflow error\n");}
}

int pop(stack *stk) {
    if (stk->top > 0){ //check for underflow
        stk->top--; //decrement top
        return stk->array[stk->top]; //return the top value
    }
    else{
        printf("Stack underflow\n");
        return 0;
    }
}

int main() {
    stack *stk = new_stack(4);
    int n = 10;
    for(int i = 0; i < n; i++) {
        push(stk, i+30);
    }
    for(int i = 0; i < stk->top; i++) {
        printf("stack[%d] : %d\n", i, stk->array[i]);
    }
    int val = pop(stk);
    while(val != 0) { // assuming 0 is returned when the stack is empty
        printf("pop : %d\n", val);
    val = pop(stk);
}
}
