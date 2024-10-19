#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

/* CONSTANTS */
#define AREAS 100000
#define BUFFER 200

/* HELPERS */
// Function to help measure time
long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

/* HASH FUNCTION */
int hash_function(int zip) {
    int mod = 17389;
    return zip % mod;
}

/* DATA STRUCTURES */
typedef struct area {
    int zip;
    char *name;
    int pop;
} area;

typedef struct bucket {
    area *entries;
    int nbOfElements;
    int nbOfSlots;
} bucket;

typedef struct codes {
    bucket *buckets;
    int n;
} codes;

int* extract_all_zip_codes(codes *postnr, int *total_zips) {
    int *zip_codes = (int*)malloc(postnr->n * sizeof(int));
    int count = 0;
    for (int i = 0; i < 17389; i++) {
        bucket *b = &postnr->buckets[i];
        if (b->entries != NULL) {
            for (int j = 0; j < b->nbOfElements; j++) {
                zip_codes[count++] = b->entries[j].zip;
            }
        }
    }

    *total_zips = count;
    return zip_codes;
}

/* FUNCTION TO INSERT INTO BUCKET */
void insert_to_bucket(codes *postnr, area *a) {
    int index = hash_function(a->zip);
    bucket *b = &postnr->buckets[index];

    if (b->entries == NULL) {
        b->entries = (area*)malloc(sizeof(area));
        b->nbOfElements = 0;
        b->nbOfSlots = 1;
    } else if (b->nbOfElements >= b->nbOfSlots) {
        b->nbOfSlots *= 2;
        b->entries = (area*)realloc(b->entries, b->nbOfSlots * sizeof(area));
    }
    b->entries[b->nbOfElements] = *a;
    b->nbOfElements++;
}

/* LOOKUP FUNCTION */
area* lookup_in_bucket(codes *postnr, int zip) {
    int index = hash_function(zip);
    bucket *b = &postnr->buckets[index];
    if (b->entries == NULL) {
        return NULL;
    }
    for (int i = 0; i < b->nbOfElements; i++) {
        if (b->entries[i].zip == zip) {
            return &b->entries[i];
        }
    }
    return NULL;
}

/* READ POSTCODES FROM FILE */
codes *read_postcodes(char *file) {
    codes *postnr = (codes*)malloc(sizeof(codes));
    postnr->buckets = (bucket*)calloc(17389, sizeof(bucket));
    postnr->n = 0;

    FILE *fptr = fopen(file, "r");
    char *lineptr = malloc(sizeof(char) * BUFFER);
    size_t n = BUFFER;

    while (getline(&lineptr, &n, fptr) > 0) {
        char *copy = (char*)malloc(sizeof(char) * n);
        strcpy(copy, lineptr);

        char *zip_str = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *pop_str = strtok(NULL, ",");

        int zip_part1 = atoi(zip_str);
        int zip_part2 = atoi(zip_str + 3);
        int zip = zip_part1 * 100 + zip_part2;

        //build area struct
        area *a = (area*)malloc(sizeof(area));
        a->zip = zip;
        a->name = strdup(name);
        a->pop = atoi(pop_str);

        insert_to_bucket(postnr, a);
        postnr->n++;

        free(copy);
    }
    fclose(fptr);
    free(lineptr);

    return postnr;
}

/* FUNCTIONALITY TESTS*/
void functionality_tests(codes *postnr) {
    //existing code
    int test_zip = 11115;
    printf("search for 11115 (exists): \t");
    area *result = lookup_in_bucket(postnr, test_zip);
    if (result != NULL) {
        printf("found: Zip %d \t name: %s \t population: %d\n", result->zip, result->name, result->pop);
    } else {
        printf("not found\n");
    }

    //non-existing
    test_zip = 59500;
    printf("search for 59500 (does not exist): \t");
    result = lookup_in_bucket(postnr, test_zip);
    if (result == NULL) {
        printf("not found\n");
    } else {
        printf("found: zip %d \t name: %s \t population: %d\n", result->zip, result->name, result->pop);
    }
}

/* SEARCH RUNTIME BENCHMARKS */
void search_runtime_benchmark(codes *postnr) {
    int nbOfTests = 10000;
    struct timespec t_start, t_stop;
    int total_zips = 0;
    int *zip_codes = extract_all_zip_codes(postnr, &total_zips);

    long total_time = 0;

    for (int i = 0; i<nbOfTests;i++){
        int random_index = rand()%total_zips;
        int random_zip = zip_codes[random_index];

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        area *result = lookup_in_bucket(postnr, random_zip);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        long search_time = nano_seconds(&t_start, &t_stop);
        total_time += search_time;

        printf("%ld\n", search_time);
    }
    free(zip_codes);
}

/* MAIN FUNCTION */
int main() {
    char *filename = "postnummer.csv";
    codes *postnr = read_postcodes(filename);
    if (!postnr) {return EXIT_FAILURE;}

    //functionality_tests(postnr);
    search_runtime_benchmark(postnr);

    for (int i = 0; i < 17389; i++) {
        if (postnr->buckets[i].entries != NULL) {
            for (int j = 0; j < postnr->buckets[i].nbOfElements; j++) {
                free(postnr->buckets[i].entries[j].name);
            }
            free(postnr->buckets[i].entries);
        }
    }
    free(postnr->buckets);
    free(postnr);

    return 0;
}
