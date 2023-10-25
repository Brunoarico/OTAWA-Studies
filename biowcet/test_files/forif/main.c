/**
 * The main function of the program.
 * 
 * This function initializes a variable i and a variable sum, and then enters a for loop that iterates 10 times.
 * During each iteration, the value of i is checked to see if it is even or odd. If it is even, i is added to the sum.
 * If it is odd, i is multiplied by 2 and then added to the sum. If the CRON macro is defined, the function also
 * measures the execution time of the loop using the setupCronometer(), startCronometer(), and stopCronometer() functions.
 * 
 * @return 0 indicating successful program execution.
 */

#ifdef CRON
#include "cron.h"
#endif

int main() {
#ifdef CRON
    setupCronometer();
    uint32_t start = startCronometer();
#endif
    int i = 0;
    int sum = 0;
    for (i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            sum += i;
        } else {
            sum += i * 2;
        }
    }
#ifdef CRON
    uint32_t cycles = stopCronometer(start);
#endif
    return 0;
}
