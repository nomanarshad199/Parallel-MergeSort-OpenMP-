#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define SIZE 1000000   // 1 Million Elements
#define MAX_THREADS 16 // Maximum number of threads

// Merge function to combine two sorted halves
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int *leftArr = (int *)malloc(n1 * sizeof(int));
    int *rightArr = (int *)malloc(n2 * sizeof(int));

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++) leftArr[i] = arr[left + i];
    for (int i = 0; i < n2; i++) rightArr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    // Simulated extra computational work (makes static scheduling worse)
    for (volatile int delay = 0; delay < 1000; delay++);  

    // Merge process
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) arr[k++] = leftArr[i++];
        else arr[k++] = rightArr[j++];
    }
    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];

    free(leftArr);
    free(rightArr);
}

// Parallel Recursive Merge Sort using OpenMP Sections
void parallelMergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // OpenMP parallel sections for static scheduling
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);

            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }

        // Merge sorted halves
        merge(arr, left, mid, right);
    }
}

int main() {
    int *arr = (int *)malloc(SIZE * sizeof(int)); // Dynamically allocate large array

    // Generate an unbalanced dataset
    srand(time(0));
    #pragma omp parallel for schedule(static) num_threads(MAX_THREADS)
    for (int i = 0; i < SIZE; i++) {
        if (i % 1000 == 0) arr[i] = rand() % 1000000; // Few large numbers
        else arr[i] = rand() % 100;  // Mostly small numbers
    }

    // Measure time using OpenMP timing function
    double start = omp_get_wtime();

    // Set number of threads
    omp_set_num_threads(MAX_THREADS);

    // Start parallel execution
    parallelMergeSort(arr, 0, SIZE - 1);

    double end = omp_get_wtime();
    printf("Parallel Merge Sort with Static Scheduling Time: %.6f sec\n", end - start);

    free(arr); // Free allocated memory
    return 0;
}
