#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>

#define MOD 47
#define BUFFER 200 // for reading file

/* HELPERS */
// Function to help measure time
long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

/* HASH FUNCTION */
int hash(char *name, int mod) {
    int h = 0;
    int i = 0;
    unsigned char c = 0;
    while ((c = name[i]) != 0) {
        h = (h * 31 + c) % mod;
        i++;
    }
    return h;
}

/* DATA STRUCTURES */
typedef struct connection {
    struct city *destination;
    int time; // in minutes
    struct connection *next;
} connection;

typedef struct city {
    char *name;
    connection *connections; // adjacency list
} city;

typedef struct bucket {
    city *city_data; //to first city in list
    struct bucket *next; //to next bucket
} bucket;


typedef struct codes {
    bucket *buckets;
    int n;//nb of buckets
} codes;

/* LOOKUP FUNCTION */
city* lookup_city(codes *cities, char *name) {
    int index = hash(name, cities->n);
    bucket *b = &cities->buckets[index];

    while (b != NULL) {
        if (b->city_data != NULL && strcmp(b->city_data->name, name)==0){return b->city_data;}
        b = b->next;
    }
    return NULL; //not found
}


/* INSERT CITY INTO THE HASH TABLE */
void insert_city(codes *cities, char *name) {
    int index=hash(name, cities->n);
    bucket *b=&cities->buckets[index];
    if(b->city_data==NULL){//emty bucket
        b->city_data=(city*)malloc(sizeof(city));
        b->city_data->name=strdup(name);
        b->city_data->connections=NULL;
        b->next=NULL;
    } else {//collision in bucket
        bucket *new_bucket=(bucket*)malloc(sizeof(bucket));
        new_bucket->city_data=(city*)malloc(sizeof(city));
        new_bucket->city_data->name=strdup(name);
        new_bucket->city_data->connections=NULL;
        new_bucket->next=b->next;
        b->next=new_bucket;
    }
}

/* ADD CONNECTION BETWEEN TWO CITIES */
void add_connection(city *src, city *dst, int time) {
    if (src==NULL||dst==NULL){
        printf("Could not add connection, one or both cities are NULL\n");
        return;
    }

    connection *new_connection=(connection *)malloc(sizeof(connection));
    new_connection->destination=dst;
    new_connection->time=time;
    new_connection->next=src->connections;
    src->connections=new_connection;
}

/* FUNCTION TO BUILD THE GRAPH FROM FILE */
codes *build_graph(char *file) {
    codes *cities=(codes *)malloc(sizeof(codes));
    cities->n=MOD;
    cities->buckets=(bucket *)calloc(cities->n, sizeof(bucket));

    FILE *fptr = fopen(file, "r");
    if (fptr == NULL) {printf("Failed to open file\n");return NULL;}

    char *lineptr=malloc(sizeof(char)*BUFFER);
    size_t n=BUFFER;

    while (getline(&lineptr, &n, fptr) > 0) {
        char *from_city=strtok(lineptr, ",");
        char *to_city=strtok(NULL, ",");
        char *time_str=strtok(NULL, "\n");

        int time = atoi(time_str);
        city *src = lookup_city(cities, from_city);
        city *dst = lookup_city(cities, to_city);

        if (src == NULL) {
            insert_city(cities, from_city);
            src = lookup_city(cities, from_city);
        }

        if (dst == NULL) {
            insert_city(cities, to_city);
            dst = lookup_city(cities, to_city);
        }

        if (src != NULL && dst != NULL) {
            add_connection(src, dst, time);
            add_connection(dst, src, time);
        }
    }

    fclose(fptr);
    free(lineptr);

    return cities;
}
/*--------------------------------------------------------------------*/
/*SEARCH*/
int dfs(city *current, city *target, int limit, int current_time, bool *visited, char **path, int path_index) {
    if (current==target) {
        path[path_index]=current->name;
        return current_time;
    }
    connection *conn = current->connections;
    visited[hash(current->name, MOD)] = true;
    path[path_index] = current->name;

    int shortest = -1;
    while (conn) {
        if (!visited[hash(conn->destination->name, MOD)]) {
            int time = dfs(conn->destination, target, limit - 1, current_time + conn->time, visited, path, path_index + 1);
            if (time != -1 && (shortest == -1 || time < shortest)) {
                shortest = time;
            }
        }
        conn = conn->next;
    }
    visited[hash(current->name, MOD)] = false;
    return shortest;
}

int search(city *from, city *to, int limit, char **path) {
    bool visited[MOD] = { false };
    return dfs(from, to, limit, 0, visited, path, 0);
}

/*DEBUG*/
void print_graph(codes *cities) {
    printf("Printing the graph:\n");
    for (int i = 0; i < cities->n; i++) {
        bucket *b = &cities->buckets[i];

        while (b != NULL && b->city_data != NULL) {
            printf("City: %s\n", b->city_data->name);
            connection *conn = b->city_data->connections;
            while (conn != NULL) {
                printf("  -> Connected to %s in %d minutes\n", conn->destination->name, conn->time);
                conn = conn->next;
            }
            b = b->next;
        }
    }
}


/*-----------------------------------------------------------------------------------------*/
/* MAIN FUNCTION */
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("usage: %s <from> <to> <limit>\n", argv[0]);
        return EXIT_FAILURE;
    }

    codes *city_graph = build_graph("trains.csv");
    if (!city_graph) {
        printf("Failed to build the graph from file.\n");
        return EXIT_FAILURE;
    }

    city *from = lookup_city(city_graph, argv[1]);
    city *to = lookup_city(city_graph, argv[2]);
    int limit = atoi(argv[3]);

    if (!from || !to) {
        printf("City not found in the graph.\n");
        return EXIT_FAILURE;
    }

    char *path[limit + 1];

    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    int shortest_time = search(from, to, limit, path);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);

    long wall = nano_seconds(&t_start, &t_stop);
    if (shortest_time > 0) {
        printf("Shortest path found in %d minutes in %.2fms\n", shortest_time, ((double)wall)/1000000);

        printf("Path: ");
        for (int i = 0; path[i] != NULL; i++) {
            printf("%s", path[i]);
            if (path[i + 1] != NULL) {
                printf(" -> ");
            }
        }
        printf("\n");
    } else {
        printf("No path found within the limit.\n");
    }

    return EXIT_SUCCESS;
}

