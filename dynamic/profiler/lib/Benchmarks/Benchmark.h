#ifndef BENCHMARK_H
#define BENCHMARK_H
#include <functions/adpcm.h>
#include <functions/bs.h>
#include <functions/bsort100.h>
#include <functions/cnt.h>
#include <functions/compress.h>
#include <functions/cover.h>
#include <functions/crc.h>
#include <functions/duff.h>
#include <functions/expint.h>
#include <functions/fac.h>
#include <functions/fdct.h>
#include <functions/fibcall.h>
#include <functions/insertsort.h>
#include <functions/janne_complex.h>
#include <functions/jfdctint.h>
#include <functions/lcdnum.h>
#include <functions/lms.h>
#include <functions/ludcmp.h>
#include <functions/matmul.h>
#include <functions/minver.h>
#include <functions/nc.h>
#include <functions/nsichneu.h>
#include <functions/prime.h>
#include <functions/qsort-exam.h>
#include <functions/recursion.h>
#include <functions/select.h>
#include <functions/sqrt.h>
#include <functions/statemate.h>
#include <functions/ud.h>
#include <functions/if.h>

#include <Arduino.h>  // Importante para usar a funcao millis()

#ifndef _ARRAY
typedef struct {
  int * array;
  int size;
} Array;
#define _ARRAY
#endif


class Benchmark {
   public:
    // Definindo a interface generica para as funcoes
    template <typename Func, typename... Args>

    static unsigned long cronometer(Func func, Args... args) {
        uint32_t startTime, endTime, executionCycles;

        noInterrupts();

#ifdef ARDUINO_ARCH_ESP32
        startTime = RV_READ_CSR(CSR_PCCR_MACHINE);
#elif defined(STM32F1)
        SysTick->VAL = 0;
#elif defined(ARDUINO_ARCH_AVR)
        startTime = TCNT1;
#else
        Serial.println("Plataforma não suportada");
        while (1)
            ;
#endif

        func(args...);

#ifdef ARDUINO_ARCH_ESP32
        endTime = RV_READ_CSR(CSR_PCCR_MACHINE);
#elif defined(STM32F1)
        return 0xFFFFFF - SysTick->VAL;
#elif defined(ARDUINO_ARCH_AVR)
        endTime = TCNT1;
#else
        // Código para outras plataformas ou tratamento de erro
        // Coloque aqui o código que será compilado para outras plataformas não suportadas
#endif

        interrupts();

        executionCycles = (endTime - startTime);
        return executionCycles;
    }

    static void init() {
#ifdef ARDUINO_ARCH_ESP32
        return;
#elif defined(STM32F1)
        SysTick->CTRL = 0;           // Desabilitar o contador
        SysTick->LOAD = 0xFFFFFF;    // Valor máximo de contagem
        SysTick->CTRL = 0x00000005;  // Habilitar o contador com o clock do processador
        SysTick->VAL = 0;
#elif defined(ARDUINO_ARCH_AVR)
        TCCR1A = 0;            // Configuração dos modos de operação do Timer/Counter1 (nenhuma configuração necessária)
        TCCR1B = (1 << CS10);  // Configuração do prescaler (divisão de clock = 1)
#else
        // Código para outras plataformas ou tratamento de erro
        // Coloque aqui o código que será compilado para outras plataformas não suportadas
#endif
    }
};

#endif
