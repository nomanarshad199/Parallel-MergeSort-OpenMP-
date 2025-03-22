#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define SIZE 10000        // Array Size of 10,000
#define CHUNK_SIZE 1000   // Chunk Size for Dynamic Scheduling
#define MAX_THREADS 16    // Number of Threads

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

// Parallel Merge Sort using OpenMP with Dynamic Scheduling
void parallelMergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Parallelize recursive calls
        #pragma omp parallel sections num_threads(MAX_THREADS)
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
    int *arr = (int *)malloc(SIZE * sizeof(int));

    // Generate random numbers
    srand(time(0));

    // Parallel initialization using dynamic scheduling
    #pragma omp parallel for schedule(dynamic, CHUNK_SIZE) num_threads(MAX_THREADS)
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand();
    }

    // Measure time
    double start = omp_get_wtime();

    // Set number of threads
    omp_set_num_threads(MAX_THREADS);

    // Execute parallel merge sort
    parallelMergeSort(arr, 0, SIZE - 1);

    double end = omp_get_wtime();
    printf("Parallel Merge Sort with Dynamic Scheduling (Chunk Size = 10,000) Time: %.6f sec\n", end - start);

    free(arr);
    return 0;
}
