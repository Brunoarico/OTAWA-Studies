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

#endif
