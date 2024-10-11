 /*
This code was written on  by Sam Serbouti
from the work of Johan Montelius, teacher of
Algorithms and data structures ID1021 at KTH
*/

#include <stdlib.h>
#include <stdio.h>
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

/*TREE STRUCTURE*/
typedef struct node{
    int value;
    struct node *right;
    struct node *left;
} node;

typedef struct tree{
    node *root;
} tree;

tree *construct_tree(){
    tree *tr = (tree*)malloc(sizeof(tree));
    tr->root = NULL;
    return tr;
}

void free_node(node *nd){
    if (nd != NULL){
        free_node(nd->left);
        free_node(nd->right);
        free(nd);
    }
}

void free_tree(tree *tr){
    if (tr !=NULL){
        free_node(tr->root);
        free(tr);
    }
}

node *construct_node(int val){
    node *nd= (node*)malloc(sizeof(node));
    nd->value = val;
    nd->left=NULL;
    nd->right = NULL;
    return nd;
}

//assume the tree is sorted
node* add_recursive(node *nd, int val){
    if (nd == NULL){
        return construct_node(val);
    }
    if (val < nd->value) {
        nd->left = add_recursive(nd->left, val);
    } else if(val > nd->value){
        nd->right = add_recursive(nd->right, val);
    }
    return nd;
}

bool lookkup_recursive(node *nd, int val){
    if (nd ==NULL){
        return false;
    }
    if (val == nd->value){
        return true;
    } else if (val < nd->value) {
        return lookkup_recursive(nd->left, val);
    } else {
        return lookkup_recursive(nd->right, val);
    }
}

void add(tree *tr, int val){
    tr->root = add_recursive(tr->root, val);
}

void add_non_recursive(tree *tr, int val){
    node *nd = construct_node(val);
    if (tr->root == NULL){tr->root = nd;}
    node *cursor= tr->root;
    node *above =NULL;
    while (cursor!=NULL) {
        above=cursor;
        if (val<cursor->value) {cursor = cursor->left;}
        else if(val>cursor->value) {cursor = cursor->right;}
        else {free(nd);return;}
    }
    if(val<above->value) {
        above->left = nd;
    } else{
        above->right = nd;
    }
}

bool lookup(tree *tr, int val){
    return lookkup_recursive(tr->root, val);
}

static void print(struct node *nd) {
    if (nd!=NULL){
        print(nd->left);
        printf("%d ;", nd->value);
        print(nd->right);
    }
}

void print_tree(struct tree *tr) {
    if (tr->root != NULL) {
        print(tr->root);
        printf("\n");
    }
}

/*BENCHMARKS*/
long benchmark_balanced(int n) {
    struct timespec t_start, t_stop;
    tree *tr = construct_tree();
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for(int i =0; i<n;i++){
        add(tr, rand()%10);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);

    free_tree(tr);
    return wall;
}

long benchmark_unbalanced(int n) {
    struct timespec t_start, t_stop;
    tree *tr = construct_tree();
    int nxt=0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for(int i =0; i<n;i++){
        nxt += rand()%10 + 1;
        add(tr,nxt);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);

    free_tree(tr);
    return wall;
}

int main(){
    int numberOfExp = 5;
    int sizes[] = {100, 500, 1000,5000,10000,50000};

    /*
    benchmark(99000);


    long results[numberOfExp];
    for (int idx = 0; idx < sizeof(sizes)/sizeof(sizes[0]); idx++){
        int n = sizes[idx];
        for (int j = 0; j < numberOfExp ; j++){
            results[j] = benchmark(n);
        }
        printf("%ld\n", findMean(results, numberOfExp));
    }*/

    printf("size \t balanced \t unbalanced \n");
    for(int i=0; i<6;i++){
        printf("%d \t %ld \t \t %ld \t %ld \n", i, benchmark_balanced(i), benchmark_unbalanced(i));
    }

}
