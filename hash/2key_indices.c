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

/* HASH TABLE */
typedef struct area {
    int zip;
    char *name;
    int pop;
} area;

typedef struct codes {
    area *areas[AREAS];
    int n;
} codes;

codes *read_postcodes(char *file) {
    codes *postnr = (codes*)malloc(sizeof(codes));
    postnr->n = 0;

    for (int i = 0; i < AREAS; i++) {
        postnr->areas[i] = NULL;
    }

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

        //make area strcutre
        area *a = (area*)malloc(sizeof(area));
        a->zip = zip;
        a->name = strdup(name);
        a->pop = atoi(pop_str);

        //add into array using zip as index
        postnr->areas[zip] = a;
        postnr->n++;

        free(copy);
    }
    fclose(fptr);
    free(lineptr);

    return postnr;
}

area *lookup(codes *postnr, int zip) {
    if (zip >= 0 && zip < AREAS && postnr->areas[zip] != NULL) {
        return postnr->areas[zip];
    }
    return NULL;  //not found
}

area *binary_search(codes *postnr, int target_zip) {
    int left = 0;
    int right = postnr->n - 1;
    area *sorted_areas[AREAS];

    int j = 0;
    for (int i = 0; i < AREAS; i++) {
        if (postnr->areas[i] != NULL) {
            sorted_areas[j++] = postnr->areas[i];
        }
    }

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (sorted_areas[mid]->zip == target_zip) {
            return sorted_areas[mid];//found
        }
        if (sorted_areas[mid]->zip < target_zip) {
            left =mid+1;
        } else {
            right =mid-1;
        }
    }
    return NULL;
}

/* BENCHMARKS */
long benchmark_lookup(codes *postnr, int zip) {
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    lookup(postnr, zip);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    return nano_seconds(&t_start, &t_stop);
}

long benchmark_binary(codes *postnr, int zip) {
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    binary_search(postnr, zip);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    return nano_seconds(&t_start, &t_stop);
}


int main() {
    char *filename = "postnummer.csv";
    codes *postnr = read_postcodes(filename);

    if (!postnr) {
        return EXIT_FAILURE;
    }

    printf("direct lookup 11115 : %ld ns\n", benchmark_lookup(postnr, 11115));
    printf("binary search 11115 : %ld ns\n", benchmark_binary(postnr, 11115));
    printf("direct lookup 98499 : %ld ns\n", benchmark_lookup(postnr, 98499));
    printf("binary search 98499 : %ld ns\n", benchmark_binary(postnr, 98499));

    for (int i = 0; i < AREAS; i++) {
        if (postnr->areas[i] != NULL) {
            free(postnr->areas[i]->name);
            free(postnr->areas[i]);
        }
    }
    free(postnr);

    return 0;
}
