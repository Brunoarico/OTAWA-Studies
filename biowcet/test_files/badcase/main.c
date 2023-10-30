#ifdef CRON
    #include "cron.h"
#endif
int main () {
#ifdef CRON
    setupCronometer();
    uint32_t start = startCronometer();
#endif
    int i = 0;
    int j = 0;
    int c = 0;
    if(c%2) {
        for(i = 0; i < 7; i++) {
            if(i%2){
                for (j = 0; j < 3; j++) {
                    c = c + j;
                }
            }
        }
    }
   
#ifdef CRON
    uint32_t cycles = stopCronometer(start);
#endif
    return 0;
}