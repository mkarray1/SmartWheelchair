#include "lowPassFilter.h"
#include <stdlib.h>

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

struct LowPassFilterStruct
{
    unsigned int windowSize;
    float *pastData;
};

#define MAX_WINDOW_SIZE 50

/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

static int compar(const void *num1, const void *num2);

/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/

void LowPassFilter_Init(void)
{

}


LowPassFilter_t LowPassFilter_CreateFilter(unsigned int windowSize)
{
    if(windowSize == 0)
    {
        return NULL;
    }

    LowPassFilter_t Filter = (LowPassFilter_t) malloc(sizeof(struct LowPassFilterStruct));

    if (Filter == NULL)
    {
        return NULL;
    }

    Filter->windowSize = windowSize;

    Filter->pastData = (float *) calloc(windowSize, sizeof(float));

    if (Filter->pastData == NULL)
    {
        goto ARRAY_FAILED;
    }

    unsigned int i;

    for (i = 0; i < windowSize; i++)
    {
        Filter->pastData[i] = 0;
    }

    return Filter;


ARRAY_FAILED:

    free(Filter);

    return NULL;

}

float LowPassFilter_Execute(LowPassFilter_t Filter, float dataPoint)
{
    #define MOVING_MEDIAN
    #ifdef MOVING_MEDIAN

    float sortedData[MAX_WINDOW_SIZE];

    unsigned int i;
    unsigned int j;
    unsigned int k;
    float temp;

    for (i = (Filter->windowSize - 1); i > 0; i-- )
    {
        Filter->pastData[i] = Filter->pastData[i-1];
        sortedData[i] = Filter->pastData[i];
    }
    Filter->pastData[0] = dataPoint;
    sortedData[0] = Filter->pastData[0];

    for(j=0; j <= Filter->windowSize-1; j++){
        for (k=0; k <= Filter->windowSize-1; k++){
            if (sortedData[k] > sortedData[k+1]){
                temp = sortedData[k];
                sortedData[k] = sortedData[k+1];
                sortedData[k+1] = temp;
            }

        }
    }

    //qsort(sortedData, Filter->windowSize, sizeof(float), compar);

    if ((Filter->windowSize % 2) == 0)
    {
        int index = (Filter->windowSize) / 2;

        return((sortedData[index] + sortedData[index - 1]) / 2.0f);
    }

    else
    {
        int index = Filter->windowSize / 2;

        return sortedData[index];
    }

    #endif


    #ifdef MOVING_AVERAGE

    unsigned int i;

    for (i = (Filter->windowSize - 1); i > 0; i-- )
    {
        Filter->pastData[i] = Filter->pastData[i-1];
    }
    Filter->pastData[0] = dataPoint;


    float sum = 0;

    for ( i = 0; i < Filter->windowSize; i++)
    {
        sum += Filter->pastData[i];
    }


    return sum / (float) Filter->windowSize;

    #endif

}

void LowPassFilter_DestroyFilter(LowPassFilter_t Filter)
{
    free(Filter->pastData);
    free(Filter);
}



static int compar(const void *num1, const void *num2)
{
    float el1 = *(float *) num1;
    float el2 = *(float *) num2;

    if (el1 < el2)
    {
        return -1;
    }

    else
    {
        return 1;
    }
}
