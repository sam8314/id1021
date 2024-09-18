#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

/*HELPER FUCNTIONS*/
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

//Function to help measure time
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

// Function to swap two integers
void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

//Function to display the arrays
void displayArray(int *array, int n){
    for (int i =0; i <n; i++){
        printf("%d, ", array[i]);
    }
    printf("\n\n");
}


/*SORTING ALGORITHMS*/
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int candidate = i; //candidate for smallest element
        
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[candidate]) {candidate = j;}  // update candidate index if smaller element is found
        }        
        if (candidate != i) {swap(&arr[i], &arr[candidate]);}
    }
}

void insertionSort(int array[], int n){
    for (int i = 1; i < n; i++) {
        for (int j = i; j > 0 && array[j] < array[j - 1]; j--) {
            swap(&array[j], &array[j - 1]);
        }
    }    
}

// Function to merge two sorted subarrays into one sorted array
void merge(int *org, int *aux, int lo, int mid, int hi) {
    for (int k = lo; k <= hi; k++) {aux[k] = org[k];} //copy from original to auxiliary

    int i = lo;        // start index for left subarray
    int j = mid + 1;   // start index for right subarray

    for (int k = lo; k <= hi; k++) {
        if (i > mid) {org[k] = aux[j++];}
        else if (j > hi) {org[k] = aux[i++];}
        else if (aux[i] <= aux[j]) {org[k] = aux[i++];} 
        else {org[k] = aux[j++];}
    }
}

// Recursive function to divide the array and sort the subarrays
void sort(int *org, int *aux, int lo, int hi) {
    if (lo >= hi) {return;}  //array has 1 or 0 elements
    
    int mid = (lo + hi) / 2; //midpoint

    sort(org, aux, lo, mid);      // sort left half
    sort(org, aux, mid + 1, hi);  // sort right half

    merge(org, aux, lo, mid, hi);
}

void mergeSort(int *org, int n) {
    int *aux = (int*)malloc(n * sizeof(int));
    sort(org, aux, 0, n - 1);
}

// Partition function: rearranges the array and returns the pivot index
int partition(int array[], int low, int high) {
    int pivot = array[high]; //last element as pivot
    int i = low - 1; //idx of smaller element

    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++; //move index of the smaller element
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]); //replace pivot
    return (i + 1); //index of pivot
}

// The main quicksort recursive function
void quickSort(int array[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(array, low, high);
        quickSort(array, low, pivotIndex - 1);   //sort the left part
        quickSort(array, pivotIndex + 1, high);  //sort the right part
    }
}

int main(){

    struct timespec t_start, t_stop;
    int logScale[] = {100, 200, 300, 400, 500, 600, 700, 800, 900,
                    1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
                    10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000};
    int numberOfExp = 5;

    long benchmark(int n){
        //int *array= (int*)malloc(n*sizeof(int));
        //for (int i = 0; i<n;i++) array[i] = rand()%(n*2);
        int *array = sorted(n);
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        insertionSort(array, n);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop);
        return wall;
        free(array);
    }

    benchmark(99000);

    long results[numberOfExp];        
    for (int idx = 0; idx < sizeof(logScale)/sizeof(logScale[0]); idx++){
        int n = logScale[idx];
        for (int j = 0; j < numberOfExp ; j++){
            results[j] = benchmark(n);
        }
        printf("%ld\n", findMean(results, numberOfExp));
    }
}