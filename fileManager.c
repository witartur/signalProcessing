#include <stdio.h>
#include <stdlib.h>

#include "signal.h"
#include "fileManager.h"


void writeCSV(int N, float *resultY, float *resultX)
{
    FILE* waveform;
    waveform = fopen("C:\\Users\\artur\\OneDrive\\Repos\\C\\signalProcessing\\generatedSignals\\signal.CSV","w+");
    char newLine []= {"\n"};

    if (waveform == NULL)
    {
        printf("Failed to write to csv file");
        return; 
    }
    for (int i = 0; i < N; i++)
    {
        fprintf(waveform, "%f,", resultX[i]);
    }

    fprintf(waveform, "%s", newLine);
    
    for (int i = 0; i < N; i++)
    {
        fprintf(waveform, "%f,", resultY[i]);
    }

    fclose(waveform);
    waveform = NULL;
}

void readCSV(float **resultX, float **resultY, int *amountOfAxisElements)
{
     FILE* waveform;
    waveform = fopen("C:\\Users\\artur\\OneDrive\\Repos\\C\\signalProcessing\\generatedSignals\\signal.CSV","r");
    // char newLine []= {"\n"}; 
    // char searchNewLine = {'/X'};
    int i = 0;
    int A;
    float *reallocBuffer= NULL;
    float *buffer = malloc(sizeof(float));

    if (waveform == NULL)
    {
        printf("Failed to open the csv file");
        return; 
    }
    else
    {
        for (i = 0; ; i++)
        {
            reallocBuffer = (float*)realloc(buffer, (i+1) * sizeof(float));
            
            if (reallocBuffer == NULL)
			{
				printf("Failled to reallocate memory\n");
                free(buffer);
                break;
			}
            else
            {
                buffer = reallocBuffer;
            }       

            if(fscanf(waveform, "%f,", &(buffer[i])) == EOF)
            {
                *amountOfAxisElements = (i+1)/2;

                *resultX = malloc((*amountOfAxisElements)*sizeof(float));
                *resultY = malloc((*amountOfAxisElements)*sizeof(float));
                
                for (int j = 0; j < *amountOfAxisElements ; j++)
                {
                    *(*resultX + j) = buffer[j];
                    *(*resultY + j) = buffer[j+*amountOfAxisElements];
                }
                
                break;
            }    
        }
    }

    if (buffer != NULL)
    {
        free(buffer);
        buffer = NULL;
    }

    fclose(waveform);
    waveform = NULL;
}