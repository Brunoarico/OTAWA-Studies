#include <Arduino.h>
#include "Benchmark.h"
#include "Jsonserial.h"

#define BAUD 115200

#define N 10

//int size = 0;                   //ToDo

/*Variaveis para o teste*/
/*que feio, variaveis globais*/
int alea[N] = {74, 39, 102, 57, 86, 91, 32, 65, 78, 23};
int worst[N] = {102, 91, 86, 78, 74, 65, 57, 39, 32, 23};
int best[N] = {23, 32, 39, 57, 65, 74, 78, 86, 91, 102};
int target = 10;

Jsonserial js;
Array* parametersArray;
Array parameters;
String fname;
long mTime;

/*Prototipos das funcoes de teste*/
void binary_search_test(Array JsonArrayIterator, int target);
void bubble_sort_test(int arr[]);
void sum_test();
void nops();

/*Prototipos das funcoes auxiliares*/
void copyArray(int ref[], int cpy[]);

void setup() {
    Serial.begin(BAUD);
    Serial.println("Starting ...");
    Benchmark::init();
    js.begin(Serial);
}

void loop() {
    /*if(js.isJsonAvailable()) {
        js.processJSON();
        parametersArray = js.getArraysFromJson();
        parameters = js.getParamFromJson();
        fname = js.getFuncFromJson();
    }

    if(fname == "binary_search_test") {
        long mTimes[parameters.size];
        for (int i = 0; i < parameters.size; i++) {
            mTimes[i] = Benchmark::cronometer(binary_search_test, parametersArray[i], parameters.array[i]);
            
        }
        js.sendWcets(mTimes, parameters.size);
    }


    fname = "";*/

    //unsigned long mTime = Benchmark::cronometer(adpcm);
    //unsigned long mTime = Benchmark::cronometer(bs);
    //unsigned long mTime = Benchmark::cronometer(bsort100);
    //unsigned long mTime = Benchmark::cronometer(cnt);
    //unsigned long mTime = Benchmark::cronometer(compresss);
    //unsigned long mTime = Benchmark::cronometer(cover);
    //unsigned long mTime = Benchmark::cronometer(crc);
    //unsigned long mTime = Benchmark::cronometer(duff);
    //unsigned long mTime = Benchmark::cronometer(expintt);
    //unsigned long mTime = Benchmark::cronometer(facc);
    //unsigned long mTime = Benchmark::cronometer(fdctt);
    //unsigned long mTime = Benchmark::cronometer(fibcall);
    //unsigned long mTime = Benchmark::cronometer(insertsort);
    //unsigned long mTime = Benchmark::cronometer(janne_complex);
    //unsigned long mTime = Benchmark::cronometer(jfdctint);
    //unsigned long mTime = Benchmark::cronometer(lcdnum);
    //unsigned long mTime = Benchmark::cronometer(lmss);
    //unsigned long mTime = Benchmark::cronometer(ludcmp);
    //unsigned long mTime = Benchmark::cronometer(matmul);
    //unsigned long mTime = Benchmark::cronometer(minverr);
    //unsigned long mTime = Benchmark::cronometer(nc);
    //unsigned long mTime = Benchmark::cronometer(nsichneu);
    //unsigned long mTime = Benchmark::cronometer(primee);
    //unsigned long mTime = Benchmark::cronometer(qsort_exam);
    //unsigned long mTime = Benchmark::cronometer(recursion);
    //unsigned long mTime = Benchmark::cronometer(selectt);
    //unsigned long mTime = Benchmark::cronometer(sqrtt);
    //unsigned long mTime = Benchmark::cronometer(statemate);
    //unsigned long mTime = Benchmark::cronometer(ud);

    //unsigned long mTime = Benchmark::cronometer(nops);
    //unsigned long mTime = Benchmark::cronometer(sum_test);
    //unsigned long mTime = Benchmark::cronometer(binary_search_test, best, target);
    //unsigned long mTime = Benchmark::cronometer(bubble_sort_test, alea);
    //unsigned long mTime = Benchmark::cronometer(bubble_sort_test, best);
    //unsigned long mTime = Benchmark::cronometer(bubble_sort_test, worst);

    unsigned long mTime = Benchmark::cronometer(ifs);
    //unsigned long mTime = Benchmark::cronometer(bubble_sort_test, worst);
    // Imprimindo os tempos medidos no Monitor Serial
    Serial.println("Execution Time: " + String(mTime) + " cycles");
    delay(1000);
}

void copyArray(int ref[], int arrCpy[]) {
    for (int i = 0; i < N; i++) {
        arrCpy[i] = ref[i];
    }
}

/*---Funcoes de teste---*/

void binary_search_test(Array arr, int target) {
    int left = 0;
    int right = arr.size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr.array[mid] == target) return;
        if (arr.array[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }
}

void printArray(int arr[]) {
    for (int i = 0; i < N; i++) {
        Serial.println(arr[i]);
    }
    Serial.println();
}

void bubble_sort_test(int arr[]) {
    int k, j, aux;
    int arrCpy[N];
    copyArray(arr, arrCpy);
    for (k = 1; k < N; k++) {
        for (j = 0; j < N - k; j++) {
            if (arrCpy[j] > arrCpy[j + 1]) {
                aux = arrCpy[j];
                arrCpy[j] = arrCpy[j + 1];
                arrCpy[j + 1] = aux;
            }
        }
    }
}

void sum_test() {
    int sum =  2 + 2;
}

void nops() {
    asm volatile("nop");
}