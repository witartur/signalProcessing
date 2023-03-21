#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "signal.h"
#include "fileManager.h"

typedef enum  
{
    signalChoice,
    functionGenerating,
    parametersChange,
    functionModificationFirst,
    functionModificationSecond 
} PrintMenuCase;

enum UserSignalChoice
{
    functionGeneration = 1,
    functionReadingCSV,
    exitApp,
};

enum UserParamChoice
{
    domain = 1,
    coefficients,
    sampling,
    proceed,
};
enum UserModChoice
{
    interference = 1,
    medianFilter,
    movingAverageFilter,
    userProceed,
};

static void PrintMenu(PrintMenuCase menuCase, float beginningOfDomain, float endOfDomain, float *arrayCoefficient, float dx);
bool HandleParamChange(int choice, float *beginningOfDomain, float *endOfDomain, float *dx, float *arrayCoefficient);
bool HandleModification(int choice, float *resultY, int amountOfAxisElements);
void MemoryRelease(float *resultX, float *resultY);
int GetAmountOfAxisElements(bool calcNeeded, float beginningOfDomain, float endOfDomain, int dx, int amountOfAxisElementsFILE);



int main()
{
    int userSignalChoice, userFunctionChoice, paramToBeChanged, paramToBeChangedNext, amountOfAxisElements = 0, amountOfAxisElementsFILE = 0;    
    float arrayCoefficient[4] = {0, 1, 0.5, 1};          
    float beginningOfDomain = 0, endOfDomain = 3, dx = 1;                              
    float *resultY = NULL, *resultX = NULL;                      

    while(1)
    {
        PrintMenu(signalChoice, beginningOfDomain, endOfDomain, arrayCoefficient, dx);
        scanf("%d", &userSignalChoice);

        switch (userSignalChoice)
        {
        case functionGeneration:
            PrintMenu(functionGenerating, beginningOfDomain, endOfDomain, arrayCoefficient, dx);
            scanf("%d", &userFunctionChoice);
            
            while(1)
            {
                PrintMenu(parametersChange, beginningOfDomain, endOfDomain, arrayCoefficient, dx);
                scanf("%d", &paramToBeChanged);

                if(HandleParamChange(paramToBeChanged, &beginningOfDomain, &endOfDomain, &dx, arrayCoefficient) == false)
                    break;
            }

            amountOfAxisElements = GetAmountOfAxisElements((userSignalChoice == 1), beginningOfDomain, endOfDomain, dx, amountOfAxisElements);
            resultY = malloc(amountOfAxisElements*sizeof(float));
            resultX = malloc(amountOfAxisElements*sizeof(float));
            FunctionGenerator(userFunctionChoice, arrayCoefficient, beginningOfDomain, dx, resultY, resultX, amountOfAxisElements);
            break;
        case functionReadingCSV:
            PrintMenu(functionModificationFirst, beginningOfDomain, endOfDomain, arrayCoefficient, dx);
            readCSV(&resultX, &resultY, &amountOfAxisElementsFILE);
            scanf("%d", &paramToBeChangedNext);
            amountOfAxisElements = GetAmountOfAxisElements((userSignalChoice == 1), beginningOfDomain, endOfDomain, dx, amountOfAxisElements);
            
            while (1)
            {
                HandleModification(paramToBeChangedNext, resultY, amountOfAxisElements);
                if(HandleModification(paramToBeChangedNext, resultY, amountOfAxisElements) == false)
                    break;           
            }

            PrintMenu(functionModificationSecond, beginningOfDomain, endOfDomain, arrayCoefficient, dx);
            scanf("%d", &paramToBeChangedNext);
            break;
        case exitApp:
            MemoryRelease(resultX, resultY);
            exit(0);
            break;
        default:
            printf("Choose an option by entering the number\r\n");
            system("pause");
            break;
        }

        amountOfAxisElements = GetAmountOfAxisElements((userSignalChoice == 1), beginningOfDomain, endOfDomain, dx, amountOfAxisElements);
        writeCSV(amountOfAxisElements, resultY, resultX);
        MemoryRelease(resultX, resultY);
    }
    return 0;
}

static void PrintMenu(PrintMenuCase menuCase, float beginningOfDomain, float endOfDomain, float *arrayCoefficient, float dx)
{
    system("cls");
    // fputs("\x1b[H\x1b[2J\x1b[3J", stdout); fflush(stdout); // alt for clearing terminal
    printf("*****     SIGNAL GENERATOR     *****\r\n\n");

    switch (menuCase)
    {
    case signalChoice:
        printf("Choose an option:\r\n1)   Generate new signal\r\n2)   Modify the existing signal\n3)   EXIT\n");
        break;    
    case functionGenerating:
        printf("Choose signal to generate:\r\n   1) Ax + Bsin(Cx) + D\r\n   2) Ax^3 + Bx^2 + Cx + D\r\n");
        break;
    case parametersChange:
        printf("Your function parameters:\r\n  Beginning Of Domain= %.2f\r\n  End Of Domain= %.2f\r\n  A= %.2f\r\n  B= %f\r\n  C= %.2f\r\n  D= %f\r\n  sampling dx= %f\r\n\n", beginningOfDomain, endOfDomain, *arrayCoefficient, *(arrayCoefficient + 1), *(arrayCoefficient + 2), *(arrayCoefficient + 3), dx);
        printf("Choose what parameter of function you want to add or change\r\n   1) Domain\r\n   2) Coefficients\r\n");
        printf("   3) dx - sampling\r\n   4) Preceed\r\n");
        break;
    case functionModificationFirst:
        printf("Choose and option of modification\r\n   1) Add interference\r\n   2) Add median filter\r\n   3) Add moving average filter\r\n   4) Procced\r\n");   
        break;
    case functionModificationSecond:
        printf("What next?\r\n   1) Add interference\r\n   2) Add median filter\r\n   3) Add moving average filter\r\n   4) Procced and save signal\r\n");
        break;
    
    default:
        break;
    }
    
    printf("   I choose an option:  ");
}


bool HandleParamChange(int choice, float *beginningOfDomain, float *endOfDomain, float *dx, float *arrayCoefficient) {
    switch (choice)
    {
    case domain:
        LoadDomain(beginningOfDomain, endOfDomain);
        return true;
    case coefficients:
        LoadCoefficient(arrayCoefficient);
        return true;
    case sampling:
        LoadSampling(dx);
        return true;
    case proceed:
        return false;
    default:
        printf("Please choose the number from menu");
        system("pause");
        return true;
    }
}

bool HandleModification(int choice, float *resultY, int amountOfAxisElements)
{
    switch (choice)
    {
    case interference:
        printf("Picked interference");
        system("pause");
        Interference(resultY, amountOfAxisElements);
        return false;
    case medianFilter:
        printf("Picked median filter");
        system("pause");
        MedianFilter(resultY, amountOfAxisElements);
        return false;
    case movingAverageFilter:
        printf("Picked moving average filter");
        system("pause");
        MovingAverageFilter(resultY, amountOfAxisElements);
        return false;
    case userProceed:
        return false;
    default:
        printf("Please choose the number from menu");
        system("pause");
        return true;
    }
}

void MemoryRelease(float *resultX, float *resultY)
{
    free(resultY);
    resultY = NULL;
    free(resultX);
    resultX = NULL;
}
