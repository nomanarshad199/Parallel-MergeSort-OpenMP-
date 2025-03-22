#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define SIZE 1000000  // 1 Million Elements
#define MAX_THREADS 8 // Maximum threads

// Merge function to combine two sorted halves
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int leftArr[n1], rightArr[n2];

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++) leftArr[i] = arr[left + i];
    for (int i = 0; i < n2; i++) rightArr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    // Merging process
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) arr[k++] = leftArr[i++];
        else arr[k++] = rightArr[j++];
    }
    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];
}

// Parallel Merge Sort with Static Scheduling
void parallelMergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Static scheduling: Divide tasks equally among threads
        #pragma omp parallel num_threads(MAX_THREADS)
        {
            #pragma omp for schedule(static) nowait
            for (int i = 0; i < 2; i++) { // Splitting left and right
                if (i == 0) parallelMergeSort(arr, left, mid);
                else parallelMergeSort(arr, mid + 1, right);
            }
        }

        // Merge step with critical section to prevent race conditions
        #pragma omp critical
        merge(arr, left, mid, right);
    }
}

int main() {
    int arr[SIZE];

    // Generate random numbers
    srand(time(0));
    for (int i = 0; i < SIZE; i++) arr[i] = rand();

    // Measure time with OpenMP timing function
    double start = omp_get_wtime();

    // Start parallel execution
    parallelMergeSort(arr, 0, SIZE - 1);

    double end = omp_get_wtime();
    printf("Parallel Merge Sort with Static Scheduling Time: %.6f sec\n", end - start);

    return 0;
}
