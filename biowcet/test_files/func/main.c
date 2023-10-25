#ifdef CRON
#include "cron.h"
#endif

/**
 * Calculates the factorial of a given integer using an iterative approach.
 * 
 * @param n The integer to calculate the factorial of.
 * @return The factorial of the given integer.
 */
int fat(int n) {
    int i = 1;
    int fat = 1;
    while (i <= n) {
        fat = fat * i;
        i = i + 1;
    }
    return fat;
}

int sum (int a, int b) {
    int c = a + b;
    return c;
}

/**
 * The main function of the program.
 * 
 * @return 0 if the program runs successfully.
 */
int main() {
#ifdef CRON
    setupCronometer();
    uint32_t start = startCronometer();
#endif
    int b = 40;
    int a = 10;

    int sa = fat(a);
    int sb = fat(b);
    int s = sum(sa, sb);
#ifdef CRON
    uint32_t cycles = stopCronometer(start);
#endif
    return 0;
}
