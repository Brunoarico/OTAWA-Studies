/**
 * @file main.c
 * @brief This file contains an implementation of binary search algorithm and a main function that generates an array of integers and performs a binary search on it.
 *
 * The binary search algorithm implemented in this file receives an array of integers and a target integer as input and returns the index of the target integer in the array, if it exists. Otherwise, it returns -1.
 *
 * The main function generates an array of integers using the generateArray function and performs a binary search on it using the binarySearch function. If the CRON macro is defined, it also measures the execution time of the binary search algorithm using the setupCronometer, startCronometer and stopCronometer functions from the cron.h library.
 *
 */
#include <stdint.h>
#include <stdlib.h>
#ifdef CRON
#include "cron.h"
#endif
#define SIZE 100

int binarySearch(int *arr, int target) {
    int left = 0;
    int right = SIZE - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

void generateArray(int *arr) {
    int i;
    for (i = 0; i < SIZE; i++) {
        arr[i] = i;
    }
}

int main(void) {
#ifdef CRON
    setupCronometer();
    uint32_t start = startCronometer();
#endif
    int arr[SIZE];
    int target = SIZE + 1;

    generateArray(arr);
    int pos = binarySearch(arr, target);

#ifdef CRON
    uint32_t cycles = stopCronometer(start);
#endif
    return 0;
}
