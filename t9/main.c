#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER 256

typedef struct node {
    bool valid;
    struct node *next[27];
} node;

typedef struct trie {
    node *root;
} trie;

node *new_node() {
    node *nd = (node *)malloc(sizeof(node));
    nd->valid = false;
    for (int i = 0; i < 27; i++) {
        nd->next[i] = NULL;
    }
    return nd;
}

static int code(wchar_t w) {
    switch (w) {
        case 'a': return 0;
        case 'b': return 1;
        case 'c': return 2;
        case 'd': return 3;
        case 'e': return 4;
        case 'f': return 5;
        case 'g': return 6;
        case 'h': return 7;
        case 'i': return 8;
        case 'j': return 9;
        case 'k': return 10;
        case 'l': return 11;
        case 'm': return 12;
        case 'n': return 13;
        case 'o': return 14;
        case 'p': return 15;
        case 'r': return 16;
        case 's': return 17;
        case 't': return 18;
        case 'u': return 19;
        case 'v': return 20;
        case 'x': return 21;
        case 'y': return 22;
        case 'z': return 23;
        case 229: return 24;  // 'å'
        case 228: return 25;  // 'ä'
        case 246: return 26;  // 'ö'
    }
    printf("Strange character in word: %d\n", w);
    return -1;
}


node *add(node *nd, wchar_t *rest) {
    if(nd==NULL){nd = new_node();} //build new node
    int c=code((int)*rest);
    if(c==-1) return nd; //wrong char
    if (rest[1] == '\n') {nd->valid = true;}
    else {nd->next[c]=add(nd->next[c],rest+1);}
    return nd;
}


trie *dict() {
    setlocale(LC_ALL, "en_US.UTF-8");
    FILE *fptr = fopen("kelly.txt", "r");
    if (fptr == NULL)
        return NULL;

    trie *kelly = (trie *)malloc(sizeof(trie));
    kelly->root = NULL;

    wchar_t ws[BUFFER];
    while (fgetws(ws, BUFFER, fptr) != NULL) {
        kelly->root = add(kelly->root, ws);
    }

    fclose(fptr);
    return kelly;
}

void key_to_indices(int key, int *start, int *end) {
    switch (key) {
        case '2': *start = 0; *end = 2; break;  //a b c
        case '3': *start = 3; *end = 5; break;  //d e f'
        case '4': *start = 6; *end = 8; break;  //g h i
        case '5': *start = 9; *end = 11; break; //j k l
        case '6': *start = 12; *end = 14; break;//m n o
        case '7': *start = 15; *end = 17; break;//p r s
        case '8': *start = 18; *end = 20; break;//t u v
        case '9': *start = 21; *end = 26; break;//x y z å ä ö
        default: *start = -1; *end = -1; break; //wrong
    }
}

void collect(node *current, wchar_t *keys, int depth, char *word, int word_len, char **results, int *result_count)
{
    if (keys[depth] == '\0') { //reach end of key seaquence
        if (current->valid) { //valid word
            word[word_len] = '\0';//end word with null character
            results[*result_count]=strdup(word);//copy word into our results
            (*result_count)++;//increment result count
        }
        return;
    }
    int start, end;
    key_to_indices(keys[depth], &start, &end); //map current key to its letter
    for (int i=start; i<=end; i++){ //iterate over possible letters
        if (current->next[i]){
            word[word_len]="abcdefghijklmnoprstuvxyzåäö"[i];
            collect(current->next[i], keys, depth+1, word, word_len+1,results, result_count);
        }
    }
}

char **decode(trie *t9, wchar_t *keys){
    char word[BUFFER];
    char **results =malloc(sizeof(char*)*100);
    int result_count =0;
    collect(t9->root, keys, 0, word, 0, results, &result_count);
    results[result_count]=NULL;
    return results;
}

void print_results(char **results) {
    for (int i = 0; results[i] != NULL; i++) {
        printf("%s\n", results[i]);
        free(results[i]);
    }
    free(results);
}

int main() {
    trie *t9 = dict();
    wchar_t keys[] = L"43556"; //for hello
    char **results = decode(t9, keys);
    print_results(results);

    return 0;
}
