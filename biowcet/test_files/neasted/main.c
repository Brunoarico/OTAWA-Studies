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
    for(i = 0; i < 7; i++) {
        for (j = 0; j < 3; j++) {
            if(j % 2) c+=j*2;
            else c+=j*3;
        }
    }
#ifdef CRON
    uint32_t cycles = stopCronometer(start);
#endif
    return 0;
}