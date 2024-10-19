#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define AREAS 100000  // We need a large array to use zip as index
#define BUFFER 20000
#define MX 20

/* ZIP CODES */
typedef struct area {
    int zip;
    char *name;
    int pop;
} area;

typedef struct codes {
    area *areas;
    int n;
} codes;

codes *read_postcodes(char *file) {
    codes *postnr = (codes*)malloc(sizeof(codes));
    postnr->areas = (area*)calloc(AREAS, sizeof(area));
    postnr->n = 0;

    FILE *fptr = fopen(file, "r");
    if (fptr == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char *lineptr = malloc(sizeof(char) * BUFFER);
    size_t n = BUFFER;

    while (getline(&lineptr, &n, fptr) > 0) {
        char *copy = strdup(lineptr);

        char *zip_str = strtok(copy, ",");
        char *name = strtok(NULL, ",");
        char *pop_str = strtok(NULL, ",");

        int zip_part1 = atoi(zip_str);
        int zip_part2 = atoi(zip_str + 3);
        int zip = zip_part1 * 100 + zip_part2;

        //area structure
        area a;
        a.zip = zip;
        a.name = strdup(name);
        a.pop = atoi(pop_str);

        postnr->areas[zip] = a;
        postnr->n++;

        free(copy);
    }
    fclose(fptr);
    free(lineptr);

    return postnr;
}


void collisions(codes *postnr, int mod) {
    int data[mod];
    int cols[MX];

    for (int i = 0; i < mod; i++) {
        data[i] = 0;
    }
    for (int i = 0; i < MX; i++) {
        cols[i] = 0;
    }

    for (int i = 0; i < AREAS; i++) {
        if (postnr->areas[i].zip != 0) {
            int index = postnr->areas[i].zip % mod;
            data[index]++;
        }
    }

    int sum = 0;
    for (int i = 0; i < mod; i++) {
        sum += data[i];
        if (data[i] < MX) {
            cols[data[i]]++;
        }
    }

    printf("total entries: %d\n", sum);
    printf("collisions: \n");
    for (int i = 1; i < MX; i++) {
        printf("%2d entries: %6d indices\n", i, cols[i]);
    }
    printf("\n");
}

int main() {
    char *filename = "postnummer.csv";
    codes *postnr = read_postcodes(filename);
    if (!postnr) {
        return EXIT_FAILURE;
    }

    collisions(postnr, 10000);
    collisions(postnr, 12345);
    collisions(postnr, 13513);
    collisions(postnr, 13600);
    collisions(postnr, 14000);
    collisions(postnr, 17389);
    collisions(postnr, 20000);
    collisions(postnr, 50000);
    collisions(postnr, 100000);

    for (int i = 0; i < AREAS; i++) {
        if (postnr->areas[i].name != NULL) {
            free(postnr->areas[i].name);
        }
    }
    free(postnr->areas);
    free(postnr);

    return 0;
}
