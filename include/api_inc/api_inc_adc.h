#ifndef __API_INC_ADC_H
#define __API_INC_ADC_H

#include "stdint.h"
#include "stdbool.h"

typedef enum
{
    ADC_CHANNEL_0 = 0 ,
    ADC_CHANNEL_1     ,
    ADC_CHANNEL_MAX
}ADC_Channel_t;


typedef enum
{
    ADC_SAMPLE_PERIOD_122US = 0        ,
    ADC_SAMPLE_PERIOD_1MS              ,
    ADC_SAMPLE_PERIOD_10MS             ,
    ADC_SAMPLE_PERIOD_100MS            ,
    ADC_SAMPLE_PERIOD_250MS            ,
    ADC_SAMPLE_PERIOD_500MS            ,
    ADC_SAMPLE_PERIOD_1S               ,
    ADC_SAMPLE_PERIOD_2S               ,
    ADC_SAMPLE_PERIOD_MAX              
} ADC_Sample_Period_t;//Acquisition Time Period

typedef struct
{
    ADC_Channel_t        channel;
    ADC_Sample_Period_t  samplePeriod;
}ADC_Config_t;


#endif
