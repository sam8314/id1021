#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

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

#define MOD 47
#define BUFFER 200 // for reading file
#define INF INT_MAX

typedef struct connection {
    struct city *destination;
    int time; // in minutes
    struct connection *next;
} connection;

typedef struct city {
    char *name;
    connection *connections; // adjacency list
    int id; // Unique ID assigned to each city for array indexing
} city;

typedef struct bucket {
    city *city_data;
    struct bucket *next;
} bucket;

typedef struct codes {
    bucket *buckets;
    int n; // number of buckets
    int city_count;
} codes;

typedef struct path_entry {
    city *city_data;
    int total_time;
    city *prev_city;
} path_entry;

typedef struct priority_queue {
    path_entry **data;
    int size;
    int capacity;
} priority_queue;

/* Priority Queue Functions */
priority_queue* create_priority_queue(int capacity) {
    priority_queue *pq = (priority_queue *)malloc(sizeof(priority_queue));
    pq->data = (path_entry **)malloc(sizeof(path_entry *) * capacity);
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void swap(path_entry **a, path_entry **b) {
    path_entry *temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(priority_queue *pq, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq->size && pq->data[left]->total_time < pq->data[smallest]->total_time)
        smallest = left;
    if (right < pq->size && pq->data[right]->total_time < pq->data[smallest]->total_time)
        smallest = right;

    if (smallest != i) {
        swap(&pq->data[i], &pq->data[smallest]);
        heapify(pq, smallest);
    }
}

path_entry* pop_min(priority_queue *pq) {
    if (pq->size == 0)
        return NULL;
    path_entry *min = pq->data[0];
    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;
    heapify(pq, 0);
    return min;
}

void push(priority_queue *pq, path_entry *entry) {
    if (pq->size == pq->capacity)
        return;

    pq->data[pq->size] = entry;
    int i = pq->size;
    pq->size++;

    // Bubble up
    while (i != 0 && pq->data[(i - 1) / 2]->total_time > pq->data[i]->total_time) {
        swap(&pq->data[i], &pq->data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

/* Graph and City Functions */
city* lookup_city(codes *cities, char *name) {
    int index = hash(name, cities->n);
    bucket *b = &cities->buckets[index];

    while (b != NULL) {
        if (b->city_data != NULL && strcmp(b->city_data->name, name) == 0)
            return b->city_data;
        b = b->next;
    }
    return NULL; // not found
}

void insert_city(codes *cities, char *name, int id) {
    int index = hash(name, cities->n);
    bucket *b = &cities->buckets[index];
    if (b->city_data == NULL) {
        b->city_data = (city *)malloc(sizeof(city));
        b->city_data->name = strdup(name);
        b->city_data->connections = NULL;
        b->city_data->id = id; // Assign unique ID to the city
        b->next = NULL;
    } else { // collision in bucket
        bucket *new_bucket = (bucket *)malloc(sizeof(bucket));
        new_bucket->city_data = (city *)malloc(sizeof(city));
        new_bucket->city_data->name = strdup(name);
        new_bucket->city_data->connections = NULL;
        new_bucket->city_data->id = id; // Assign unique ID
        new_bucket->next = b->next;
        b->next = new_bucket;
    }
}

void add_connection(city *src, city *dst, int time) {
    connection *new_connection = (connection *)malloc(sizeof(connection));
    new_connection->destination = dst;
    new_connection->time = time;
    new_connection->next = src->connections;
    src->connections = new_connection;
}

codes* build_graph(char *file) {
    codes *cities = (codes *)malloc(sizeof(codes));
    cities->n = MOD;
    cities->buckets = (bucket *)calloc(cities->n, sizeof(bucket));
    cities->city_count = 0;

    FILE *fptr = fopen(file, "r");
    if (fptr == NULL) {
        printf("Failed to open file\n");
        return NULL;
    }

    char *lineptr = malloc(sizeof(char) * BUFFER);
    size_t n = BUFFER;

    while (getline(&lineptr, &n, fptr) > 0) {
        char *from_city = strtok(lineptr, ",");
        char *to_city = strtok(NULL, ",");
        char *time_str = strtok(NULL, "\n");

        int time = atoi(time_str);
        city *src = lookup_city(cities, from_city);
        city *dst = lookup_city(cities, to_city);

        if (src == NULL) {
            insert_city(cities, from_city, cities->city_count++);
            src = lookup_city(cities, from_city);
        }

        if (dst == NULL) {
            insert_city(cities, to_city, cities->city_count++);
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

/* Dijkstra's Algorithm */
void dijkstra(codes *cities, city *source, city *target) {
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    //----------------------------
    int n=cities->city_count;
    path_entry **done = (path_entry **)calloc(n,sizeof(path_entry *));
    for (int i = 0; i < n; i++) {done[i] = NULL;}
    priority_queue *pq = create_priority_queue(n);

    path_entry *start = (path_entry *)malloc(sizeof(path_entry));
    start->city_data = source;
    start->total_time = 0;
    start->prev_city = NULL;
    push(pq, start); //push starting city to the queue

    while (pq->size > 0) {
        path_entry *current = pop_min(pq);
        if (current->city_data == target) {//reached target
            printf("Shortest path takes %d minutes.\n", current->total_time);
            printf("Path: ");
            path_entry *p = current;
            while (p != NULL) {
                printf("%s", p->city_data->name);
                if (p->prev_city != NULL) {printf(" <- ");}
                int prev_id =p->prev_city ? p->prev_city->id:-1;
                p=(prev_id!=-1) ? done[prev_id]:NULL;
            }
            printf("\n");
        }

        int current_id = current->city_data->id;
        if (done[current_id] == NULL) {
            done[current_id] = current;
            connection *conn = current->city_data->connections;
            while (conn != NULL) {//expolre neighbours
                city *neighbor = conn->destination;
                int new_time = current->total_time + conn->time;
                if (done[neighbor->id] == NULL || new_time < done[neighbor->id]->total_time) { //need to update shortest path for neighbour?
                    path_entry *neighbor_entry = (path_entry *)malloc(sizeof(path_entry));
                    neighbor_entry->city_data = neighbor;
                    neighbor_entry->total_time = new_time;
                    neighbor_entry->prev_city = current->city_data;
                    push(pq, neighbor_entry);
                }
                conn = conn->next;
            }
        }
    }
    free(pq->data);
    free(pq);
    for (int i = 0; i < n; i++) {
        free(done[i]);
    }
    free(done);
    //--------------------------
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);

    printf("execution time: %ld ns\n",wall);
}

/* functionality test */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./program <file>\n");
        return 1;
    }

    codes *cities = build_graph(argv[1]);

    if (cities != NULL) {
        char start_city[50], end_city[50];
        printf("Enter starting city: ");
        scanf("%s", start_city);
        printf("Enter destination city: ");
        scanf("%s", end_city);

        city *source = lookup_city(cities, start_city);
        city *target = lookup_city(cities, end_city);

        if (source != NULL && target != NULL) {
            dijkstra(cities, source, target);
        } else {
            printf("One or both cities not found in the graph.\n");
        }
    }

    return 0;
}


/*benchmark*/
/*
int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: ./program <file>\n");
        return 1;
    }
    codes *cities = build_graph(argv[1]);

    const char *target_cities[] = {"Stockholm", "Gothenburg", "Copenhagen", "Oslo", "Helsinki",
    "Berlin", "Amsterdam", "Paris", "Brussels", "Vienna",
    "Zurich", "London"};
    const char *start_city = "Malmö";

    for (int i = 0; i < sizeof(target_cities) / sizeof(target_cities[0]); i++) {
        benchmark(cities, start_city, target_cities[i]);
    }

    //clean up
    for (int i = 0; i < graph->num_cities; i++) {
        free(graph->city_names[i]);
    }
    free(graph->city_names);
    free(graph);

    return 0;

}

*/
