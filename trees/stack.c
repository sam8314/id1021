#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* TREE STRUCTURE */
typedef struct node {
    int value;
    struct node *right;
    struct node *left;
} node;

typedef struct tree {
    node *root;
} tree;

tree *construct_tree() {
    tree *tr = (tree*)malloc(sizeof(tree));
    tr->root = NULL;
    return tr;
}

void free_node(node *nd) {
    if (nd != NULL) {
        free_node(nd->left);
        free_node(nd->right);
        free(nd);
    }
}

void free_tree(tree *tr) {
    if (tr != NULL) {
        free_node(tr->root);
        free(tr);
    }
}

node *construct_node(int val) {
    node *nd = (node*)malloc(sizeof(node));
    nd->value = val;
    nd->left = NULL;
    nd->right = NULL;
    return nd;
}

node* add_recursive(node *nd, int val) {
    if (nd == NULL) {
        return construct_node(val);
    }
    if (val < nd->value) {
        nd->left = add_recursive(nd->left, val);
    } else if (val > nd->value) {
        nd->right = add_recursive(nd->right, val);
    }
    return nd;
}

void add(tree *tr, int val) {
    tr->root = add_recursive(tr->root, val);
}

bool lookkup_recursive(node *nd, int val) {
    if (nd == NULL) {
        return false;
    }
    if (val == nd->value) {
        return true;
    } else if (val < nd->value) {
        return lookkup_recursive(nd->left, val);
    } else {
        return lookkup_recursive(nd->right, val);
    }
}

bool lookup(tree *tr, int val) {
    return lookkup_recursive(tr->root, val);
}

/* STACK STRUCTURE */
typedef struct stack {
    int top;
    int size;
    node **array;
} stack;

stack *new_stack(int size) {
    node **array = (node**)malloc(size * sizeof(node*));
    stack *stk = (stack*)malloc(sizeof(stack));
    stk->top = 0;
    stk->size = size;
    stk->array = array;
    return stk;
}

void push(stack *stk, node *nd) {
    if (stk->top == stk->size) {
        int newSize = stk->size * 2;
        stk->size = newSize;
        node **copy = (node**)malloc(newSize * sizeof(node*));
        for (int i = 0; i < stk->top; i++) {
            copy[i] = stk->array[i];
        }
        free(stk->array);
        stk->array = copy;
    }
    stk->array[stk->top] = nd;
    stk->top++;
}

node *pop(stack *stk) {
    if (stk->top > 0) {
        stk->top--;
        return stk->array[stk->top];
    } else {
        printf("Stack underflow\n");
        return NULL;
    }
}

bool is_empty(stack *stk) {
    return stk->top == 0;
}

void free_stack(stack *stk) {
    free(stk->array);
    free(stk);
}

/* PRINT FUNCTION USING STACK */
void print_tree(tree *tr) {
    stack *stk = new_stack(10);
    node *cur = tr->root;
    while (cur != NULL || !is_empty(stk)) {
        while (cur != NULL) {
            push(stk, cur);
            cur = cur->left;
        }
        cur = pop(stk);
        printf("%d ", cur->value);
        cur = cur->right;
    }

    printf("\n");
    free_stack(stk);
}


int main() {
    // 1. Basic Tree Construction and Print
    tree *tr = construct_tree();
    add(tr, 50);
    add(tr, 30);
    add(tr, 20);
    add(tr, 40);
    add(tr, 70);
    add(tr, 60);
    add(tr, 80);

    printf("print tree: \t");
    print_tree(tr);

    printf("search 40 in tree (should return true): %s\n", lookup(tr, 40) ? "Found" : "Not Found");
    printf("search 100 in tree (should return false): %s\n ", lookup(tr, 100) ? "Found" : "Not Found");

    printf("add 40 to tree (shouodn't change)\n");
    add(tr, 40);
    printf("print tree: \t");
    print_tree(tr);

    free_tree(tr);

    return 0;
}
