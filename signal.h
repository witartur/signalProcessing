// #ifndef SIGNAL_H
// #define SIGNAL_H

#include <stdbool.h>

typedef struct 
{
    bool calculationNeeded;

    union 
    {
        struct 
        {
            float beginningOfDomain;
            float endOfDomain;
            float dx;
        };

        int amountOfAxisElementsFILE;
    };
} arraySizeParams;

void FunctionGenerator(int secondUserChoice, float *arrayCoefficient, float d1, float dx, float *resultY, float *resultX, int N);
void LoadDomain(float *d1_ptr, float *d2_ptr);
void LoadCoefficient(float *arrayCoefficient);
void LoadSampling(float *dx_ptr);
int ResultArraySize(bool calculationNeeded, arraySizeParams params);
int GetAmountOfAxisElements(bool calcNeeded, float beginningOfDomain, float endOfDomain, int dx, int amountOfAxisElementsFILE);
void Interference(float *resultY, int N);
void MedianFilter(float *resultY, int N);
void MovingAverageFilter(float *resultY, int N);
void BubbleSort(float array[], int arraySize);

// #endif  // SIGNAL_H