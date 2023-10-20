#ifndef CRON_H
#define CRON_H
#include <stdint.h>

#define  ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)
#define  ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)
#define  ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)

uint32_t cronometer(void (*targetFunc)()) {
    uint32_t start = ARM_CM_DWT_CYCCNT;
    targetFunc();
    return ARM_CM_DWT_CYCCNT - start;
}

uint32_t startCronometer () {
    return ARM_CM_DWT_CYCCNT;
}

uint32_t stopCronometer (uint32_t start) {
    return ARM_CM_DWT_CYCCNT - start;
}

void setupCronometer () {
    if (ARM_CM_DWT_CTRL != 0) {        // See if DWT is available
        ARM_CM_DEMCR      |= 1 << 24;  // Set bit 24
        ARM_CM_DWT_CYCCNT  = 0;
        ARM_CM_DWT_CTRL   |= 1 << 0;   // Set bit 0

    }
}
#endif
