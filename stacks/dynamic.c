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
    
    stk->top = 0;
    stk->size=size;
    stk->array=array;
    return stk;
}

void push(stack *stk, int val) {
    if (stk->top == stk->size) {
        int newSize = stk->size*2;
        stk->size = newSize;
        int *copy = (int*)malloc(newSize*sizeof(int));
        for (int i = 0; i < newSize/2; i++) {
            copy[i] = stk->array[i];
        }
        free(stk->array);
        stk->array = copy;
        stk->array[stk->top] = val;
        stk->top++;
    }
    else{
        stk->array[stk->top] = val;
        stk->top++;
    } 
}

int pop(stack *stk) {
    if (stk->top > 0){
        if(stk->top <= 0.3*stk->size && stk->size > 4){//copy array with half the size
            int newSize=stk->size/2;
            stk->size = newSize;
            int *copy = (int*)malloc(newSize*sizeof(int));
            for (int i = 0; i<stk->top;i++){
                copy[i] = stk->array[i];
            }
            free(stk->array);
            stk->array = copy;
        }
        stk->top--;
        return stk->array[stk->top];
    }
    else{
        printf("Stack underflow\n");
        return 0;
    }
}

int main() {
    stack *stk = new_stack(4);
    /*
    int n = 20;
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
    */

    for (int i = 0; i<20;i++){
        push(stk, 30+i);
        printf("(push %d) \t top: %d \t stack[%d]: %d \t array size: %d\n", 30+i,stk->top, stk->top-1, stk->array[stk->top-1], stk->size);
    }
    printf("--------\n");
    for (int i = 0; i<15;i++){
        int val = pop(stk);
        printf("(pop %d) \t top: %d \t stack[%d]: %d \t array size: %d\n", val, stk->top, stk->top-1, stk->array[stk->top-1], stk->size);
    }

}




