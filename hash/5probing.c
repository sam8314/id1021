#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define INITIAL_SIZE 50000
#define BUFFER 200

/* HELPERFUCNTIONS */
// Function to help measure time
long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

/* HASH FUNCTION */
int hash_function(int zip, int mod) {
    return zip % mod;
}

/* DATA STRUCTURES */
typedef struct area {
    int zip;
    char *name;
    int pop;
} area;

typedef struct codes {
    area *areas;
    int size;
    int n;
} codes;

int* extract_all_zip_codes(codes *postnr, int *total_zips) {
    int *zip_codes = (int*)malloc(postnr->n*sizeof(int));
    int count = 0;

    for (int i = 0; i < postnr->size; i++) {
        if (postnr->areas[i].zip != -1) {
            zip_codes[count++] = postnr->areas[i].zip;
        }
    }

    *total_zips = count;
    return zip_codes;
}

/* INSERT FUNCTION WITH LINEAR PROBING */
void insert_with_probing(codes *postnr, area *a) {
    int index = hash_function(a->zip, postnr->size);
    int checks = 0;
    while (postnr->areas[index].zip != -1) {
        index=(index+1)%postnr->size;
        checks++;
    }

    postnr->areas[index]=*a;
    postnr->n++;
    //printf("inserted %d after %d checks\n", a->zip, checks);
}

/* LOOKUP FUNCTION WITH LINEAR PROBING */
area* lookup_with_probing(codes *postnr, int zip) {
    int index =hash_function(zip, postnr->size);
    int checks =0;
    while (postnr->areas[index].zip != -1) {
        if (postnr->areas[index].zip == zip) {
            printf("%d\n", checks);
            return &postnr->areas[index];
        }
        index=(index+1)%postnr->size;
        checks++;
    }

    //printf("not found %d after %d checks\n", zip, checks);
    return NULL;
}

/* READ POSTCODES FROM FILE */
codes *read_postcodes_with_probing(char *file) {
    codes *postnr = (codes*)malloc(sizeof(codes));
    postnr->size = INITIAL_SIZE;  // Initialize the size of the array
    postnr->n = 0;
    postnr->areas = (area*)malloc(postnr->size * sizeof(area));

    // Initialize the array slots to be empty
    for (int i = 0; i < postnr->size; i++) {
        postnr->areas[i].zip = -1;
    }

    FILE *fptr = fopen(file, "r");
    char *lineptr = malloc(sizeof(char) * BUFFER);
    size_t n = BUFFER;

    while (getline(&lineptr, &n, fptr) > 0) {
        char *copy = (char*)malloc(sizeof(char) * n);
        strcpy(copy, lineptr);

        // Divide the line into components (zip, name, pop)
        char *zip_str = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *pop_str = strtok(NULL, ",");

        // Convert the zip code to an integer
        int zip_part1 = atoi(zip_str);
        int zip_part2 = atoi(zip_str + 3);
        int zip = zip_part1 * 100 + zip_part2;

        // Create and populate the `area` struct
        area *a = (area*)malloc(sizeof(area));
        a->zip = zip;
        a->name = strdup(name);
        a->pop = atoi(pop_str);

        // Insert into the array using linear probing
        insert_with_probing(postnr, a);

        free(copy);
    }
    fclose(fptr);
    free(lineptr);

    return postnr;
}

/* FUNCTIONALITY TESTS */
void functionality_tests(codes *postnr) {
    // existing code
    int test_zip = 11115;
    printf("search for 11115 (exists): \t");
    area *result = lookup_with_probing(postnr, test_zip);
    if (result != NULL) {
        printf("found: Zip %d \t name: %s \t population: %d\n", result->zip, result->name, result->pop);
    } else {
        printf("not found\n");
    }

    // non-existing
    test_zip = 59500;
    printf("search for 59500 (does not exist): \t");
    result = lookup_with_probing(postnr, test_zip);
    if (result == NULL) {
        printf("not found\n");
    } else {
        printf("found: Zip %d \t name: %s \t population: %d\n", result->zip, result->name, result->pop);
    }
}

/* SEARCH RUNTIME BENCHMARKS */
void search_runtime_benchmark(codes *postnr) {
    int nbOfTests = 10000;
    struct timespec t_start, t_stop;
    int total_zips = 0;
    int *zip_codes = extract_all_zip_codes(postnr, &total_zips);

    long total_time = 0;

    for (int i = 0; i < total_zips; i++) {
        int current_zip = zip_codes[i];

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        area *result = lookup_with_probing(postnr, current_zip);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        long search_time = nano_seconds(&t_start, &t_stop);
        total_time += search_time;

        //printf("%ld\n", search_time);
    }
    free(zip_codes);
}

/* MAIN FUNCTION */
int main() {
    char *filename = "postnummer.csv";
    codes *postnr = read_postcodes_with_probing(filename);
    if (!postnr) {return EXIT_FAILURE;}

    //functionality_tests(postnr);
    search_runtime_benchmark(postnr);

    for (int i = 0; i < postnr->size; i++) {
        if (postnr->areas[i].zip != -1) {
            free(postnr->areas[i].name);
        }
    }
    free(postnr->areas);
    free(postnr);

    return 0;
}
