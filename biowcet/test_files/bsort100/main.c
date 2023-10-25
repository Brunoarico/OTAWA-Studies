/**
 * @file main.c
 * @brief This file contains the implementation of a bubble sort algorithm to sort an array of integers.
 * 
 * The program generates an array of integers in descending order and sorts it using the bubble sort algorithm.
 * The sorted array is then returned. The program also includes a timer to measure the execution time of the sorting algorithm.
 * 
 */
#ifdef CRON
#include "cron.h"
#endif

#define SIZE 100

void bubbleSort(int arr[]) {
    int temp;
    int swapped;
    int i, j;
    for (i = 0; i < SIZE - 1; i++) {
        swapped = 0;
        for (j = 0; j < SIZE - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }
}

void generateArray(int *arr) {
    int i;
    for (i = 0; i < SIZE; i++) {
        arr[i] = i;
    }
}

void generateArrayInvert(int *arr) {
    int i;
    for (i = 0; i < SIZE; i++) {
        arr[SIZE - i - 1] = i;
    }
}

int main(void) {
#ifdef CRON
    setupCronometer();
    uint32_t start = startCronometer();
#endif
    int arr[SIZE];
    generateArrayInvert(arr);
    bubbleSort(arr);

#ifdef CRON
    uint32_t cycles = stopCronometer(start);
#endif
    return 0;
}
