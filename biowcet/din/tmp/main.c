#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "cron.h"

#if defined(DET)
	#define DERMINISTIC	
#elif defined(RAND)
	#define RANDOM
#endif

int arr[SIZE];
int target = 7;


void generateArray();
void binarySearch();

int main(void) {
	setupCronometer();
    
	while (1) {
		generateArray();
		__uint32_t cycles = cronometer(binarySearch);
	}
	free(arr);
    return 0;
}

void binarySearch() {
    int left = 0;
    int right = SIZE - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) return;
        if (arr[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }
}

#if defined(WORST)
	void generateArray() {
		int i;
		for (i = 0; i < SIZE; i++) arr[i] = i;
		target = SIZE+1;
	}	

#elif defined(BEST)
	void generateArray() {
		int i;
		for (i = 0; i < SIZE; i++) arr[i] = i;
		target = SIZE/2;
	}	

#elif defined(RANDOM)
	int compare(const void *a, const void *b) {
    	return (*(int *)a - *(int *)b);
	}

	void generateArray() {
		srand(target);
		target= rand()%100;
		for (int i = 0; i < SIZE; i++) arr[i] = rand()%100;
		qsort(arr, SIZE, sizeof(int), compare);
	}

#endif
