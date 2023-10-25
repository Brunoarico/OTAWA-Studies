/**
 * @file main.c
 * @brief This file contains the main function that executes the program logic.
 * 
 * The program initializes four integer variables and checks if the value of 'a' is equal to 1.
 * If it is, the value of 'b' is set to 2 and the program checks if the value of 'c' is equal to 3.
 * If it is, the value of 'd' is set to 4. The program also includes the 'cron.h' header file
 * and measures the execution time of the code block between the 'startCronometer()' and 'stopCronometer()' functions
 * if the 'CRON' macro is defined.
 * 
 * @note This program is for testing purposes only and does not perform any useful function.
 */

#ifdef CRON
    #include "cron.h"
#endif

int main () {
#ifdef CRON
    setupCronometer();
    uint32_t start = startCronometer();
#endif
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    if (a == 1) {
        b = 2;
        if (c == 3) {
            d = 4;
        }
    }
#ifdef CRON
    uint32_t cycles = stopCronometer(start);
#endif
    return 0;
}