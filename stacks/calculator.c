
/*
This code was written on 04/09/2024 by Sam Serbouti
from the work of Johan Montelius, teacher of
Algorithms and data structures ID1021 at KTH
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 
#include <string.h>


//STATIC
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

    else{
        printf("Stack overflow error\n");
    }
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


int main(){
    stack *stk=new_stack(10);
    printf("HP-35 pocket calculator\n");

    size_t n = 10;
    char *buffer = (char*)malloc(n*sizeof(char));

    bool run = true;

    while(run) {
        printf(" > ");
        getline(&buffer, &n, stdin);
        if (strcmp(buffer, "\n") == 0){
            run = false;
        } 

        else if (strcmp(buffer, "+\n") == 0){
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a+b);
        }

        else if (strcmp(buffer, "-\n") == 0){
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a-b);
        }

        else if (strcmp(buffer, "*\n") == 0){
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a*b);
        }

        else if (strcmp(buffer, "/\n") == 0){
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a/b);
        }

        else{
            int val = atoi(buffer);
            push(stk, val);
        }
    }
    printf("the result is: %d\n", pop(stk));
}