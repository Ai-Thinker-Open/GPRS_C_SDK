/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/
/*
#ifndef TGT_LCDD_CONFIG
#define LCDD_CONFIG {                                                   \
    {.cs            =   HAL_GOUDA_LCD_CS_0,                             \
    .outputFormat   =   HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565,      \
    .cs0Polarity    =   FALSE,                                          \
    .cs1Polarity    =   FALSE,                                          \
    .rsPolarity     =   FALSE,                                          \
    .wrPolarity     =   FALSE,                                          \
    .rdPolarity     =   FALSE}}
///@todo check bellow timings with LCD spec

// Write:
// tas >= 10 ns
// tah >=  5 ns
// pwl >= 50 ns
// pwh >= 50 ns

#define LCDD_TIMING_32K {                                               \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  1,                                                   \
    .pwh        =  3}}                                                  \


#define LCDD_TIMING_13M {                                               \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  1,                                                   \
    .pwh        =  3}}                                                  \

#define LCDD_TIMING_26M {                                               \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  2,                                                   \
    .pwh        =  3}}                                                  \

#define LCDD_TIMING_39M {                                               \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  2,                                                   \
    .pwh        =  3}}                                                  \

#define LCDD_TIMING_52M {                                               \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  3,                                                   \
    .pwh        =  3}}                                                  \

#define LCDD_TIMING_78M {                                               \
    {.tas       =  2,                                                   \
    .tah        =  2,                                                   \
    .pwl        =  5,                                                   \
    .pwh        =  5}}                                                  \

#define LCDD_TIMING_104M {                                              \
    {.tas       =  2,                                                   \
    .tah        =  2,                                                   \
    .pwl        =  5,                                                   \
    .pwh        =  5}}                                                  \

#define LCDD_TIMING_156M {                                              \
    {.tas       =  4,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  9,                                                   \
    .pwh        =  9}}                                                  \

#define LCDD_TIMING_178M {                                              \
    {.tas       =  3,                                                   \
    .tah        =  2,                                                   \
    .pwl        =  6,                                                   \
    .pwh        =  6}}                                                  \

#define LCDD_TIMING_208M {                                              \
    {.tas       =  4,                                                   \
    .tah        =  2,                                                   \
    .pwl        =  8,                                                   \
    .pwh        =  8}}                                                  \

#define LCDD_TIMING_250M {                                              \
    {.tas       =  5,                                                   \
    .tah        =  3,                                                   \
    .pwl        =  10,                                                   \
    .pwh        =  10}}                                                  \

#define LCDD_TIMING_312M {                                              \
    {.tas       =  6,                                                   \
    .tah        =  3,                                                   \
    .pwl        =  12,                                                   \
    .pwh        =  12}}                                                  \


#define TGT_LCDD_CONFIG                                                 \
    {                                                                   \
        .config     = LCDD_CONFIG,                                      \
        .timings    = {                                                 \
            LCDD_TIMING_32K,                                            \
            LCDD_TIMING_13M,                                            \
            LCDD_TIMING_26M,                                            \
            LCDD_TIMING_39M,                                            \
            LCDD_TIMING_52M,                                            \
            LCDD_TIMING_78M,                                            \
            LCDD_TIMING_104M,                                           \
            LCDD_TIMING_156M,                                           \
            LCDD_TIMING_178M,                                           \
            LCDD_TIMING_208M,                                           \
            LCDD_TIMING_250M,                                           \
            LCDD_TIMING_312M,                                           \
        }                                                               \
    }

// Read:
// tas >=   5 ns
// tah >=   5 ns
// pwl >= 150 ns
// pwh >= 150 ns

#define LCDD_TIMING_READ_32K {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \


#define LCDD_TIMING_READ_13M {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_26M {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_39M {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_52M {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_78M {                                          \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_104M {                                         \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_156M {                                         \
    {.tas       =  2,                                                   \
    .tah        =  4,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  50}}                                                 \

#define LCDD_TIMING_READ_178M {                                         \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_208M {                                         \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_250M {                                         \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \

#define LCDD_TIMING_READ_312M {                                         \
    {.tas       =  1,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  40,                                                  \
    .pwh        =  40}}                                                 \


#define LCDD_READ_CONFIG                                               \
  {                                                                     \
     .config  =  LCDD_CONFIG,                                           \
     .timings    = {                                                    \
            LCDD_TIMING_READ_32K,                                       \
            LCDD_TIMING_READ_13M,                                       \
            LCDD_TIMING_READ_26M,                                       \
            LCDD_TIMING_READ_39M,                                       \
            LCDD_TIMING_READ_52M,                                       \
            LCDD_TIMING_READ_78M,                                       \
            LCDD_TIMING_READ_104M,                                      \
            LCDD_TIMING_READ_156M,                                      \
            LCDD_TIMING_READ_178M,                                      \
            LCDD_TIMING_READ_208M,                                      \
            LCDD_TIMING_READ_250M,                                      \
            LCDD_TIMING_READ_312M,                                      \
            }                                                           \
    }

typedef struct LCDD_CONFIG_STRUCT_T LCDD_CONFIG_T;

#endif // TGT_LCDD_CONFIG
*/

// =============================================================================
/// Serial LCDD config
// =============================================================================
#include "api_hal_gouda.h"
#define LCDD_SPI_LINE_TYPE     (HAL_GOUDA_SPI_LINE_4)
#define LCDD_SPI_FREQ          (50000000)
#define LCDD_TIME_MUTEX_RETRY   200   //ms
// =============================================================================
// Screen properties
// =============================================================================
// Number of actual pixels in the display width
#define LCDD_DISP_X             240

// Number of pixels in the display height
#define LCDD_DISP_Y             320

#define LCD_NO_PRODUCT_ID

#define LCD_ILI9341_ID 0x9341


#include "assert.h"
#define LCDD_ASSERT(c,fmt)  Assert(c,fmt)


