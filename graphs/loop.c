#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>

/* CONSTANTS */
#define MOD 47
#define BUFFER 200
#define MAX_CITIES 1000
#define MAX_RECURSION_DEPTH 1000

/* DATA STRUCTURES */
typedef struct connection {
    struct city *destination;
    int time;
    struct connection *next;
} connection;

typedef struct city {
    char *name;
    connection *connections;
} city;

typedef struct bucket {
    city *city_data;
    int nbOfElements;
} bucket;

typedef struct codes {
    bucket *buckets;
    int n;
} codes;

/* HASH FUNCTION */
int hash(char *name, int mod) {
    int h = 0;
    unsigned char c;
    for (int i = 0; name[i]; i++) {
        c = name[i];
        h = (h * 31 + c) % mod;
    }
    return h;
}

/* Function to check for loops */
bool loop(city **path, int k, city *current) {
    for (int i = 0; i < k; i++) {
        if (path[i] == current) {
            return true;
        }
    }
    return false;
}

void add_connection(city *src, city *dst, int time) {
    if (src == NULL || dst == NULL) {
        fprintf(stderr, "Error: could not add connection, one or both cities are NULL\n");
        return;
    }

    connection *new_connection = (connection*)malloc(sizeof(connection));
    new_connection->destination = dst;
    new_connection->time = time;
    new_connection->next = src->connections;
    src->connections = new_connection;
    //printf("Added connection from %s to %s with time %d\n", src->name, dst->name, time);
}

/* Lookup city in the hash table */
city* lookup_city(codes *cities, char *name) {
    int index = hash(name, cities->n);
    bucket *b = &cities->buckets[index];

    if (b->nbOfElements == 0 || b->city_data == NULL) {
        return NULL;
    }

    if (strcmp(b->city_data->name, name) == 0) {
        return b->city_data;
    }

    return NULL;
}

/* Insert city into the hash table */
void insert_city(codes *cities, char *name) {
    int index = hash(name, cities->n);
    bucket *b = &cities->buckets[index];

    if (b->nbOfElements == 0) {
        b->city_data = (city*)malloc(sizeof(city));
        b->city_data->name = strdup(name);
        b->city_data->connections = NULL;
        b->nbOfElements = 1;
        //printf("Inserted city: %s\n", name);
    }
}

/*with dynamic max refinement */
int shortest_path(city *from, city *to, city **path, int k, int dynamic_max) {
    if (k > MAX_RECURSION_DEPTH) {
        printf("Max recursion depth reached\n");
        return -1;
    }

    if (from == to) {return 0;}

    int sofar = -1;
    connection *nxt = from->connections;
    if (loop(path, k, from)) {
        return -1;
    }
    path[k] = from;
    //printf("Visiting city: %s at depth %d\n", from->name, k);
    while (nxt != NULL) {
        //printf("Exploring connection from %s to %s with time %d\n", from->name, nxt->destination->name, nxt->time);

        if (dynamic_max == -1 || nxt->time <= dynamic_max) {
            int d = shortest_path(nxt->destination, to, path, k + 1, dynamic_max);
            if (d >= 0 && ((sofar == -1) || (d + nxt->time) < sofar)) {
                sofar = d + nxt->time;
                dynamic_max = sofar;
        }
        nxt = nxt->next;
    }
    path[k] = NULL;
    return sofar;
}

/* Build the graph from file */
codes *build_graph(char *file) {
    codes *cities = (codes *)malloc(sizeof(codes));
    cities->n = MOD;
    cities->buckets = (bucket *)calloc(cities->n, sizeof(bucket));

    FILE *fptr = fopen(file, "r");
    if (fptr == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    char *lineptr = malloc(sizeof(char) * BUFFER);
    size_t n = BUFFER;

    while (getline(&lineptr, &n, fptr) > 0) {
        char *from_city = strtok(lineptr, ",");
        char *to_city = strtok(NULL, ",");
        char *time_str = strtok(NULL, "\n");

        if (from_city == NULL || to_city == NULL || time_str == NULL) {
            continue;
        }

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
            add_connection(dst, src, time);  // For undirected graph
        }
    }

    fclose(fptr);
    free(lineptr);

    return cities;
}

/* Main function */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <from> <to>\n", argv[0]);
        return EXIT_FAILURE;
    }

    codes *city_graph = build_graph("trains.csv");
    if (!city_graph) {
        printf("Failed to build the graph from file.\n");
        return EXIT_FAILURE;
    }

    city *from = lookup_city(city_graph, argv[1]);
    city *to = lookup_city(city_graph, argv[2]);

    if (!from || !to) {
        printf("City not found in the graph.\n");
        return EXIT_FAILURE;
    }

    city *path[MAX_CITIES] = { NULL };  // Path buffer
    int dynamic_max = -1;  // Start with no max limit

    // Measure time
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    // Find shortest path
    int shortest_time = shortest_path(from, to, path, 0, dynamic_max);

    // Measure end time
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall_time = (t_stop.tv_sec - t_start.tv_sec) * 1e9 + (t_stop.tv_nsec - t_start.tv_nsec);

    if (shortest_time > 0) {
        printf("Shortest path found in %d minutes\n", shortest_time);
        printf("Path: ");
        for (int i = 0; path[i] != NULL; i++) {
            printf("%s", path[i]->name);
            if (path[i + 1] != NULL) {
                printf(" -> ");
            }
        }
        printf("\nTime taken: %.2f ms\n", wall_time / 1e6);
    } else {
        printf("No path found.\n");
    }

    return EXIT_SUCCESS;
}
