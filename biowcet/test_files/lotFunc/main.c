#ifdef CRON
#include "cron.h"
#endif

void funcao1() {
    int a = 1;
}

void funcao2() {

    funcao1();
}

void funcao3() {

    funcao1();
}

void funcao4() {

    funcao2();
}

void funcao5() {

    funcao3();
}

void funcao6() {

    funcao4();
    funcao5();
    funcao7();
    funcao8();
}

void funcao7() {

    funcao5();
}

void funcao8() {

    funcao4();
    funcao7();
}

void funcao9() {

    funcao6();
    funcao8();
}

void funcao10() {
    
    funcao9();
}

int main() {
#ifdef CRON
    setupCronometer();
    uint32_t start = startCronometer();
#endif
    funcao10();
#ifdef CRON
    uint32_t cycles = stopCronometer(start);
#endif
    return 0;
}
