#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

// Function for calculating mean found at geeks for geeks
long findMean(long a[], int n)
{
    long sum = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];
 
    return (long)sum / (long)n;
}

//Function for finding minimum
long findMin(long a[], int n)
{
    long res = a[0];
    for(int i=1; i<n; i++)
    {
        if (a[i]<res){res = a[i];}
    }
    return res;
}

//Function for finding maximum
long findMax(long a[], int n)
{
    long res = a[0];
    for(int i=1; i<n; i++)
    {
        if (a[i]>res){res = a[i];}
    }
    return res;
}


long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
            (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}

//Function to generate a sorted array
int *sorted(int n) {
    int *array = (int*)malloc(n*sizeof(int));
    int nxt = 0;
    for (int i = 0; i < n ; i++) {
        nxt += rand()%10 + 1;
        array[i] = nxt;
    }
    return array;
}

int recursive_search(int array[], int length, int key, int first, int last, int *call_count) {
    (*call_count)++;

    if (first > last) {return 0;} //key not found

    int mid = first + (last - first) / 2;

    if (array[mid] == key) {return 1;}

    if (array[mid] < key) {
        return recursive_search(array, length, key, mid + 1, last, call_count);
    }

    if (array[mid] > key) {
        return recursive_search(array, length, key, first, mid - 1, call_count);
    }

    return 0;
}

int main(){

    struct timespec t_start, t_stop;
    int logScale[] = {10, 20, 30, 40, 50, 60, 70, 80, 90,
                    100, 200, 300, 400, 500, 600, 700, 800, 900,
                    1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
                    10000,20000,30000,40000,50000,60000,70000,80000,90000,
                    100000};
    int numberofSearches = 1000;

    long benchmark(int n, int *call_count){
        int *array= sorted(n);
        int *keys = (int*)malloc(numberofSearches*sizeof(int));
        for (int i = 0; i<numberofSearches; i++){keys[i] = rand()%(2*n);}

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        for(int idx = 0; idx < numberofSearches; idx++){
            int key = keys[idx];
            if(recursive_search(array, n, key, 0, n-1, call_count)==1){
                break;
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop);
        free(array);
        free(keys);
        return wall;
    }

    int call_count = 0;
    for(int idx = 0; idx<sizeof(logScale);idx++){
        int n = logScale[idx];
        printf("%d\t \t %d \t \t %ld\n", n, call_count, benchmark(n, &call_count));
        call_count = 0;
    }

}