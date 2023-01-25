#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "signal.h"


int resultArraySize(float d1, float d2, float dx, int firstUserChoice, int amountOfAxisElements)
{
    int N = 0;
    if (firstUserChoice == 1)
    {
        N = (d2 - d1) / dx;
        N += 1;
	    return N;
    }
    else
    {
        N = amountOfAxisElements;
        return N;
    }
}

void functionGenerator(int secondUserChoice, float *arrayCoefficient, float d1, float dx, float *resultY, float *resultX, int N)
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

            //  *(*resultY + i) = *(*(arrayCoefficient + 1)) * x + *((*arrayCoefficient + 2)) * sin(*(*arrayCoefficient + 3)) + *(*arrayCoefficient + 4);         //alt syntax
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

void loadDomain(float *d1_ptr, float *d2_ptr)
{
    float nb1;      //aux var
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

void loadCoefficient(float *arrayCoefficient)
{
    char coeff;
    for (int i = 0; i < 4; i++)
    {
        if (i == 0) 
            {
            coeff = 'A'; 
            }
        if (i == 1) 
            {
            coeff = 'B';
            }
        if (i == 2)     
            {
            coeff = 'C';
            }
        if (i == 3)     
            {
            coeff = 'D';
            }
        printf("Enter coefficient %c: ", coeff);
        scanf("%f", &arrayCoefficient[i]);
    }
    
}

void loadSampling(float *dx_ptr)
{
    float nb;

    printf("   Enter the value of sampling dx:  ");
    scanf("%f", &nb);
    
    *dx_ptr = nb;
}

void interference(float *resultY, int N)
{
    int i;
    srand(time(NULL)); //rand() need it for generating pseudo-random number - each time different
    int dupa = rand();
    float noise = (dupa % 101) / 80.000;
    int a, b;

    for (i = 0; i < N; i++)
    {
        int dupa2 = rand();
        int dupa3 = rand();
        a = dupa2 % 5;
        b = dupa3 % 3;

        if (a != 2)
        {
            if (b == 2)
            {
                resultY[i] += noise;
            }
            else
            {
                resultY[i] -= noise;
            }
        }
    }
}

void medianFilter(float *resultY, int N)
{
    int windSize = 3;
    float window[windSize];
    float sum;
    // float *filterResult = (float*)calloc(N, sizeof(float));
    float filterResult[N];
    int argNb = 0;  //function argument in array

    if(N < windSize)
    {
        printf("Cannot perform median filter. Amount of arguments cannot be smaller than window size: %d", windSize);
        sleep(3); // sleep for 3 seconds
    }
    else
    {
        for (argNb = 0; (argNb+1) <= (windSize/2) ; argNb++)
        {
            filterResult[argNb] = resultY[argNb];
        }

        for (argNb ;  (argNb+1) <= N-(windSize/2); argNb++)
        {
            for (int i = 0; i < windSize ; i++)
            {
                window[i] = resultY[argNb+i-1];
            }    

            for(int j = 0; j < windSize-1; j++ )    // sort from largest to smallest 
            {    
                for(int i = 0 ; i < windSize-1; i++)   
                {
                    if (window[i] > window[i+1])
                    {
                        sum = window[i+1];
                        window[i+1] = window[i];
                        window[i] = sum;
                    }
                }
            }    
            filterResult[argNb] = window[windSize/2];                     //saving the middle value 
        }

        for (argNb; argNb < N ; argNb++)
        {
            filterResult[argNb] = resultY[argNb];
        }

        for (int i = 0; i < N; i++)
        {
            resultY[i] = filterResult[i];
        }
    }
}

void movingAverageFilter(float *resultY, int N)
{
    int windSize = 3;
    float sum;
    float windAverage;
    float filterResult[N];
    int argNb = 0;  //function argument in array

    printf("\r\n\n   Choose the window size:");
    scanf("%d", &windSize);

    float window[windSize];

    if(N < windSize)
    {
        printf("Cannot perform moving average filter. Amount of arguments cannot be smaller than window size: %d", windSize);
        sleep(3); // sleep for 3 seconds
    }
    else
    {
        for (argNb = 0; (argNb+1) <= (windSize/2) ; argNb++) // for first elements which not changed
        {
            filterResult[argNb] = resultY[argNb];
        }

        for (argNb ;  (argNb+1) <= N-(windSize/2); argNb++)
        {
            
            for (int i = 0; i < windSize ; i++)     //saving the average
            {
                window[i] = resultY[argNb+i-1];
                sum += window[i];
            }
            
            windAverage = sum/windSize;
            filterResult[argNb] = windAverage;                     
            sum = 0;
        }

        for (argNb; argNb < N ; argNb++)            //last elements not changed
        {
            filterResult[argNb] = resultY[argNb];
        }

        for (int i = 0; i < N; i++)
        {
            resultY[i] = filterResult[i];
        }
    }
}