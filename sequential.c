#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000 // 1 Million Elements

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

// Sequential Merge Sort
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Recursive calls
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge sorted halves
        merge(arr, left, mid, right);
    }
}

int main() {
    int *arr = (int *)malloc(SIZE * sizeof(int)); // Dynamically allocate large array

    // Generate random numbers
    srand(time(0));
    for (int i = 0; i < SIZE; i++) arr[i] = rand();

    // Measure time
    clock_t start = clock();

    // Execute sequential merge sort
    mergeSort(arr, 0, SIZE - 1);

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Sequential Merge Sort Time: %.6f sec\n", time_taken);

    free(arr); // Free allocated memory
    return 0;
}
