#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "signal.h"



int ResultArraySize(bool calculationNeeded, arraySizeParams params)
{
    int N = 0;
    if (calculationNeeded)
    {
        N = (params.endOfDomain - params.beginningOfDomain) / params.dx;
        N += 1;
	    return N;
    }
    else
    {
        N = params.amountOfAxisElementsFILE;
        return N;
    }
}

int GetAmountOfAxisElements(bool calcNeeded, float beginningOfDomain, float endOfDomain, int dx, int amountOfAxisElementsFILE)
{
    arraySizeParams params;

    if(calcNeeded) {
        params.calculationNeeded = calcNeeded;
        params.beginningOfDomain = beginningOfDomain;
        params.endOfDomain = endOfDomain;
        params.dx = dx;
    }
    else 
    {
        params.calculationNeeded = calcNeeded;
        params.amountOfAxisElementsFILE = amountOfAxisElementsFILE;
    }

    return ResultArraySize(calcNeeded, params);
}

void FunctionGenerator(int secondUserChoice, float *arrayCoefficient, float d1, float dx, float *resultY, float *resultX, int N)
{
    float x;
   
    switch (secondUserChoice)
    {
    case 1: //Ax + Bsin(Cx) + D
        for (int i = 0; i < N; i++)
        {
            x = d1 +(i*dx);
            resultY[i] = (arrayCoefficient[0] * x) + (arrayCoefficient[1] * sin(arrayCoefficient[2] * x)) + arrayCoefficient[3];
            resultX[i] = x;
        }
        break;
    case 2: //Ax^3 + Bx^2 + Cx + D
         for (int i = 0; i < N; i++)
        {
            x = d1 +(i*dx);
            resultY[i] = (arrayCoefficient[0] * pow(x, 3)) + (arrayCoefficient[1] *pow(x, 2)) + (arrayCoefficient[2] * x) + arrayCoefficient[3];
            resultX[i] = x;
        }
        break;
    default:
        break;
    }
}

void LoadDomain(float *d1_ptr, float *d2_ptr)
{
    float nb1;
    float nb2;      
    
    do
    {
        printf("   Enter the begin value of domain d1:  ");
        scanf("%f", &nb1);
        printf("   Enter the end value of domain d2:  ");
        scanf("%f", &nb2);

        *d1_ptr = nb1;
        *d2_ptr = nb2;

        if (nb1>=nb2)
        {
            printf("   d1<d2  ");
        }
    }
    while (nb1 >=nb2); 
}

void LoadCoefficient(float *arrayCoefficient)
{
    char coeff;
    for (int i = 0; i < 4; i++)
    {
        if (i == 0) 
            coeff = 'A'; 
        if (i == 1) 
            coeff = 'B';
        if (i == 2)     
            coeff = 'C';
        if (i == 3)     
            coeff = 'D';

        printf("Enter coefficient %c: ", coeff);
        scanf("%f", &arrayCoefficient[i]);
    }
}

void LoadSampling(float *dx_ptr)
{
    float nb;

    printf("   Enter the value of sampling dx:  ");
    scanf("%f", &nb);
    
    *dx_ptr = nb;
}

void Interference(float *resultY, int N)
{
    int i;
    srand(time(NULL));
    int randVar = rand();
    float noise = (randVar % 101) / 80.000;
    int a, b;

    for (i = 0; i < N; i++)
    {
        int randVar2 = rand();
        int randVar3 = rand();
        a = randVar2 % 5;
        b = randVar3 % 3;

        if (a != 2)
        {
            if (b == 2)
                resultY[i] += noise;
            else
                resultY[i] -= noise;
        }
    }
}

static void RewriteFirstValues(int *functionArg, int windSize, float *resultY, float *filterResult) {
    for (*functionArg = 0; ((*functionArg)+1) <= (windSize/2) ; (*functionArg)++)
    {
        filterResult[*functionArg] = resultY[*functionArg];
    }
}

static void RewriteLastValues(int *functionArg, int N, float *resultY, float *filterResult) {
    for (*functionArg; *functionArg < N ; (*functionArg)++)
    {
        filterResult[*functionArg] = resultY[*functionArg];
    }
}

static void SetMiddleValuesUsingMedianaFilter(int *functionArg, int windSize, int N, float *resultY, float *filterResult) {
    float window[windSize];

    for (*functionArg;  ((*functionArg)+1) <= N-(windSize/2); (*functionArg)++)
        {
            for (int i = 0; i < windSize ; i++)
            {
                window[i] = resultY[*functionArg+i-1];
            }    
            BubbleSort(window, windSize);
            filterResult[*functionArg] = window[windSize/2];
        }
}

void MedianFilter(float *resultY, int N)
{
    int windSize = 3;
    float sum;
    float filterResult[N];
    int functionArg = 0;

    if(N < windSize)
    {
        printf("Cannot perform median filter. Amount of arguments cannot be smaller than window size: %d", windSize);
        system("pause");
    }
    else
    {
        RewriteFirstValues(&functionArg, windSize, resultY, filterResult);
        SetMiddleValuesUsingMedianaFilter(&functionArg, windSize, N, resultY, filterResult);
        RewriteLastValues(&functionArg, N, resultY, filterResult);

        for (int i = 0; i < N; i++)
        {
            resultY[i] = filterResult[i];
        }
    }
}

void MovingAverageFilter(float *resultY, int N)
{
    int windSize = 3;
    float sum;
    float windAverage;
    float filterResult[N];
    int functionArg = 0;

    printf("\r\n\n   Choose the window size:");
    scanf("%d", &windSize);

    float window[windSize];

    if(N < windSize)
    {
        printf("Cannot perform moving average filter. Amount of arguments cannot be smaller than window size: %d", windSize);
        system("pause");
    }
    else
    {
        for (functionArg = 0; (functionArg+1) <= (windSize/2) ; functionArg++) // for first elements which not changed
        {
            filterResult[functionArg] = resultY[functionArg];
        }

        for (functionArg ;  (functionArg+1) <= N-(windSize/2); functionArg++)
        {
            
            for (int i = 0; i < windSize ; i++)     //saving the average
            {
                window[i] = resultY[functionArg+i-1];
                sum += window[i];
            }
            
            windAverage = sum/windSize;
            filterResult[functionArg] = windAverage;                     
            sum = 0;
        }

        for (functionArg; functionArg < N ; functionArg++)            //last elements not changed
        {
            filterResult[functionArg] = resultY[functionArg];
        }

        for (int i = 0; i < N; i++)
        {
            resultY[i] = filterResult[i];
        }
    }
}

void BubbleSort(float array[], int arraySize)
{
    float temp;

    for(int j = 0; j < arraySize-1; j++ )
    {    
        for(int i = 0 ; i < arraySize-1; i++)   
        {
            if (array[i] > array[i+1])
            {
                temp = array[i+1];
                array[i+1] = array[i];
                array[i] = temp;
            }
        }
    }  
}
