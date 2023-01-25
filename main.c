#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "signal.c"
#include "fileManager.c"

int main()
{
    int firstUserChoice, secondUserChoice, thirdUserChoice, fourthUserChoice;       //aux variables for choosing option
    float arrayCoefficient[4] = {0, 1, 0.5, 1};          //array for function coefficients with defoult values
    float d1 = 0;                                   //beginning of the function with defoult value
    float d2 = 3;                                //end of the domain of the function with defoult value
    float dx = 1;                                  //sampling with defoult value
    float *resultY = NULL;              //pointer for first element of result array
    float *resultX = NULL;
    int N = 0;                          //size of resultX or result Y
    bool warning;   
    int amountOfAxisElements = 0;                         

    while(1)
    {
        // system("cls");
        fputs("\x1b[H\x1b[2J\x1b[3J", stdout); fflush(stdout); // alt for clearing terminal
        printf("*****     SIGNAL GENERATOR     *****\r\n\n");
        printf("Choose an option:\r\n1)   Generate new signal\r\n2)   Modify the existing signal\n3)   EXIT\n");
        printf("   I choose an option:  ");
        scanf("%d", &firstUserChoice);

        if (firstUserChoice == 1)
        {
            // system("cls");
            fputs("\x1b[H\x1b[2J\x1b[3J", stdout); fflush(stdout);  // alt for clearing terminal
            printf("*****     SIGNAL GENERATOR     *****\r\n\n");
            printf("Choose signal to generate:\r\n   1) Ax + Bsin(Cx) + D\r\n   2) Ax^3 + Bx^2 + Cx + D\r\n");
            printf("   I choose an option:  ");
            scanf("%d", &secondUserChoice);
            
            do
            {
                warning = false;
                // system("cls");
                fputs("\x1b[H\x1b[2J\x1b[3J", stdout); fflush(stdout); // alt for clearing terminal
                printf("*****     SIGNAL GENERATOR     *****\r\n\n");
                printf("Your function parameters:\r\n  d1= %.2f\r\n  d2= %.2f\r\n  A= %.2f\r\n  B= %f\r\n  C= %.2f\r\n  D= %f\r\n  dx= %f\r\n\n", d1, d2, arrayCoefficient[0], arrayCoefficient[1], arrayCoefficient[2], arrayCoefficient[3], dx);
                printf("Choose what parameter of function you want to add or change\r\n   1) Domain\r\n   2) Coefficients\r\n");
                printf("   3) dx - sampling\r\n   4) Preceed\r\n");
                printf("   I choose an option:  ");
                scanf("%d", &thirdUserChoice);

                switch (thirdUserChoice)
                {
                case 1:         //domain
                    loadDomain(&d1, &d2);
                    break;
                case 2:         //coefficients
                    loadCoefficient(arrayCoefficient);
                    break;
                case 3:         //sampling
                    loadSampling(&dx);
                    break;
                case 4:         //Proceed
                    break;
                default:
                    printf("Please choose the number from menu");
                    sleep(2); // sleep for 2 seconds
                    warning = true;
                    break;
                }
            } while (thirdUserChoice == 1 || thirdUserChoice == 2 ||thirdUserChoice == 3 || warning == true);

            N = resultArraySize(d1, d2, dx, firstUserChoice, amountOfAxisElements);
            resultY = malloc(N*sizeof(float));
            resultX = malloc(N*sizeof(float));
            functionGenerator(secondUserChoice, arrayCoefficient, d1, dx, resultY, resultX, N);
        }
        else if (firstUserChoice == 2)
        {
            // system("cls");
            fputs("\x1b[H\x1b[2J\x1b[3J", stdout); fflush(stdout);
            printf("*****     SIGNAL GENERATOR     *****\r\n\n");
            printf("Choose and option of modification\r\n   1) Add interference\r\n   2) Add median filter\r\n   3) Add moving average filter\r\n   4) Procced\r\n");
            readCSV(&resultX, &resultY, &amountOfAxisElements);
            scanf("%d", &fourthUserChoice);
            N = resultArraySize(d1, d2, dx, firstUserChoice, amountOfAxisElements);

            do
            {
                warning = false;
                
                switch (fourthUserChoice)
                {
                case 1:         //interference
                    printf("Picked interference");
                    sleep(1); // sleep for 1 seconds
                    interference(resultY, N);

                    break;
                case 2:         //median filter
                    printf("Picked median filter");
                    sleep(1); // sleep for 1 seconds
                    medianFilter(resultY, N);
                    break;
                case 3:         //moving average filter
                    printf("Picked moving average filter");
                    sleep(1); // sleep for 1 seconds
                    movingAverageFilter(resultY, N);
                    break;
                case 4:         //Proceed
                    break;
                default:
                    printf("Please choose the number from menu");
                    sleep(2); // sleep for 2 seconds
                    warning = true;
                    break;
                }

                // system("cls");
                fputs("\x1b[H\x1b[2J\x1b[3J", stdout); fflush(stdout);
                printf("*****     SIGNAL GENERATOR     *****\r\n\n");
                printf("What next?\r\n   1) Add interference\r\n   2) Add median filter\r\n   3) Add moving average filter\r\n   4) Procced and save signal\r\n");
                printf("   I choose an option:  ");
                scanf("%d", &fourthUserChoice);

            }while (fourthUserChoice == 1 || fourthUserChoice == 2 ||fourthUserChoice == 3 || warning == true);
        }
        else if (firstUserChoice == 3)
        {
            free(resultY);
            resultY = NULL;
            free(resultX);
            resultX = NULL;
            exit(0);
        }
        else
        {
            printf("Choose an option by entering the number\r\n");
            sleep(2);

        }

        N = resultArraySize(d1, d2, dx, firstUserChoice, amountOfAxisElements);
        writeCSV(N, resultY, resultX);

        free(resultY);
        resultY = NULL;
        free(resultX);
        resultX = NULL;
    }
    return 0;
}