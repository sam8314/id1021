/*
This code was written on 01/09/2024 by Sam Serbouti
from the work of Johan Montelius, teacher of
Algorithms and data structures ID1021 at KTH
*/


#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include<limits.h>
 
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

int main()
{
    struct timespec t_start, t_stop;
    int nbOfExperiments = 10000;
    int numberOfSearches=100;
    //int sizeOfArray = 100;
    int logScale[] = {100, 200, 300, 400, 500, 600, 700, 800, 900,
                    1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
                    10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};

    long bench(int n, int loop) 
    {
        int *array = (int*)malloc(n*sizeof(int));
        for (int i =0; i<n;i++) array[i]=i;

        int *indx= (int*)malloc(n*sizeof(int));
        for (int i =0;i<loop;i++) indx[i] = rand()%n;

        int sum=0;
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        for (int i=0; i<loop;i++) sum += array[indx[i]];
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop);
        return wall;
    }

    //ACCURACY OF CLOCK
    /*
    for(int i =0; i<nbOfAttempts;i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop);
        results[i] = wall;
        printf("%ld\n", wall);
    }*/

    //SINGLE ARRAY SEARCH
    /*
    
    int array[] = {1,2,3,4,5,6,7,8,9,0};
    int sum = 0;
    for(int i=0; i<nbOfAttempts;i++) {
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        sum+=array[i%10];
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop);
        printf("%ld\n", wall);
        results[i] = wall;
    }*/

    //MULTIPLE RANDOM ARRAY SEARCH  
    /*
    int array[] = {1,2,3,4,5,6,7,8,9,0};
    int sum = 0;

    for (int j = 0 ; j<nbOfExperiments; j++)
    {
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        for (int i = 0; i < nbOfSearches ; i++) 
        {
            sum += array[rand()%10];
        }
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        long wall = nano_seconds(&t_start, &t_stop);
        printf("%ld\n", wall/nbOfSearches);
        results[j] = wall/nbOfSearches;
    }*/

    //CHANGING ARRAY SIZE
    /*
        for (int i = 0; i<nbOfExperiments;i++){
            long wall = bench(1000, 1000);
            printf("%ld\n", wall); 
        }  */

    //IMPORTANCE OF MINIMUM
    /*
    int n = 1000;
    int loop = 100;
    int k = 10;

    for (int j = 0 ; j <nbOfExperiments; j++)
    {
        long min = LONG_MAX;
        long max = 0;
        long total = 0;
        for (int i = 0; i < k; i++)
        {
            long wall = bench(1000, loop);
            if (wall <  min) min = wall;
            if (wall > max) max = wall;
            total += wall;
        }
        //printf("%0.2f\n", (double)max/loop); //max
        //printf("%0.2f\n", (double)min/loop); //min
        printf("%0.2f\n", (((double)total)/loop/k)); //average
    }
    */

   //SEARCH FOR AN ITEM
   /*
    for (int idx = 0; idx < sizeof(logScale)/sizeof(logScale[0]); idx++)
    {
        int nbOfSearches = 100000;
        int sizeOfArray = logScale[idx];
        int sum = 0;
        int *array = (int*)malloc(sizeOfArray*sizeof(int));
        for (int i = 0; i< sizeOfArray; i++) array[i] = rand()%(sizeOfArray*2);

        int *keys = (int*)malloc(nbOfSearches*sizeof(int));
        for (int i = 0 ; i < nbOfSearches;i++) keys[i] = rand()%(sizeOfArray*2);

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        for (int i = 0 ; i < nbOfSearches; i++)
        {
            int key = keys[i];
            for (int j = 0; j < sizeOfArray; j++)
            {
                if(key==array[j]){
                    sum++;
                    break;
                }
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop)/nbOfSearches;
        printf("%ld\n", wall);
    }*/

   //SEARCH FOR DUPLICATE
   /*
    long duplicates(int n){
        int *array_a=(int*)malloc(n*sizeof(int));
        for (int i = 0; i < n;i++) array_a[i] = rand()%(n*2);

        int *array_b = (int*)malloc(n*sizeof(int));
        for (int i = 0; i < numberOfSearches; i++) array_b[i] = rand()%(n*2);
        
        int sum = 0;
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        for (int i = 0; i < numberOfSearches; i++) {
            int item = array_a[i];
            for (int j = 0; j < n; j++) {
                if (item == array_b[j]) {
                    sum++;
                    break;
                }
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop)/numberOfSearches;
        return wall;   
    }

    for (int idx = 0; idx < sizeof(logScale)/sizeof(logScale[0]); idx++)
    {
        int n = logScale[idx];
        printf("%ld\n", duplicates(n));
    }
    */

   //DISPLAY MIN MAX MEAN
   printf("\n for %d attemtps \n min runtime %ld ns \n mean runtime %ld ns \n max runtime %ld ns \n", nbOfExperiments, findMin(results, nbOfExperiments), findMean(results, nbOfExperiments), findMax(results, nbOfExperiments));

}


