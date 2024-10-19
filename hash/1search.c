#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

/* CONSTANTS */
#define AREAS 10000
#define BUFFER 200

/* HELPERS */
// Function for calculating mean
long findMean(long a[], int n) {
    long sum = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];

    return sum / n;
}

// Function for finding minimum
long findMin(long a[], int n) {
    long res = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] < res) {
            res = a[i];
        }
    }
    return res;
}

// Function for finding maximum
long findMax(long a[], int n) {
    long res = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] > res) {
            res = a[i];
        }
    }
    return res;
}

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
    area *areas;
    int n;
} codes;

//read postcodes from CSV file
codes *read_postcodes(char *file) {
    codes *postnr = (codes*)malloc(sizeof(codes));
    area *areas = (area*)malloc(sizeof(area) * AREAS);

    FILE *fptr = fopen(file, "r");
    int k = 0;
    char *lineptr = malloc(sizeof(char) * BUFFER);
    size_t n = BUFFER;

    while ((k < AREAS) && (getline(&lineptr, &n, fptr) > 0)) {
        char *copy = (char*)malloc(sizeof(char) * n);
        strcpy(copy, lineptr);

        area a;
        char *zip_str = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *pop_str = strtok(NULL, ",");

        int zip_part1 = atoi(zip_str);
        int zip_part2 = atoi(zip_str + 3);
        a.zip = zip_part1 * 100 + zip_part2;

        a.name = strdup(name);
        a.pop = atoi(pop_str);

        areas[k++] = a;
        free(copy);
    }
    fclose(fptr);
    free(lineptr);

    postnr->areas = areas;
    postnr->n = k;
    return postnr;
}


area *linear_search(codes *postnr, int target_zip) {
    for (int i = 0; i < postnr->n; i++) {
        if (postnr->areas[i].zip == target_zip) {
            return &postnr->areas[i];
        }
    }
    return NULL;
}

area *binary_search(codes *postnr, int target_zip) {
    int left =0;
    int right =postnr->n-1;

    while (left <=right) {
        int mid =left+(right-left)/2;
        if (postnr->areas[mid].zip == target_zip) {
            return &postnr->areas[mid];
        }
        if (postnr->areas[mid].zip < target_zip) {
            left =mid+1;
        } else {
            right =mid-1;
        }
    }
    return NULL;
}

/* BENCHMARKS */
long benchmark_linear(codes *postnr, int zip) {
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    linear_search(postnr, zip);
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
    for(int i =0; i<10000; i++){
        benchmark_linear(postnr, 11115);
        benchmark_binary(postnr, 11115);
        benchmark_linear(postnr, 98499);
        benchmark_binary(postnr, 98499);
    }

    printf("direct linear 11115 : %ld ns\n", benchmark_linear(postnr, 11115));
    printf("binary search 11115 : %ld ns\n", benchmark_binary(postnr, 11115));
    printf("direct linear 98499 : %ld ns\n", benchmark_linear(postnr, 98499));
    printf("binary search 98499 : %ld ns\n", benchmark_binary(postnr, 98499));

    for (int i = 0; i < postnr->n; i++) {
        free(postnr->areas[i].name);
    }
    free(postnr->areas);
    free(postnr);

    return 0;
}
