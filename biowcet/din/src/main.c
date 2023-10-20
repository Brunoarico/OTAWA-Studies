#include <stdint.h>
#include <stdlib.h>
#ifdef CRON
    #include "cron.h"
#endif
#define SIZE 500

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
        }
        else {
            right = mid - 1;
        }
    }
    return -1;
}

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
        arr[SIZE-i-1] = i;
    }
}	

int main(void) {
    #ifdef CRON
        setupCronometer();
        uint32_t start = startCronometer();
    #endif
    int arr[SIZE];
    int target = SIZE + 1;

	/*generateArray(arr);
	int pos = binarySearch(arr, target);*/
    generateArrayInvert(arr);
    bubbleSort(arr);
    #ifdef CRON
	    uint32_t cycles = stopCronometer(start);
    #endif
    return 0;
}



