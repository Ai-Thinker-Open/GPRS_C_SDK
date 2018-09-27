#ifndef __LCD_H
#define __LCD_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"
#include "stdbool.h"

typedef enum{
    LCD_ERROR_NONE              = 0,
    LCD_ERROR_RESOURCE_BUSY     = 1,
    LCD_ERROR_INVALID_PARAMETER = 2
}LCD_Error_t;

// ============================================================================
// LCD_Screen_Info_t
// ----------------------------------------------------------------------------
/// This structures stores relevant information about the LCD screen
// ============================================================================
typedef struct
{
    /// Specifies the width of the LCD screen in pixel.
    uint16_t width;
    /// Specifies the heigth of the LCD screen in pixel.
    uint16_t height;
    /// Specifies the bit depth of the LCD device, such as 8bit,16bit etc...
    uint8_t bitdepth;
    /// Reserved for future uses
    uint16_t nReserved;

} LCD_Screen_Info_t;

//=============================================================================
// LCD_ROI_t
//-----------------------------------------------------------------------------
/// Region of interest in a LCD screen or a buffer.
//=============================================================================
typedef struct
{
    /// x coordinate of the top left corner of the region.
    uint16_t x;
    /// y coordinate of the top left corner of the region.
    uint16_t y;
    /// Width of the region.
    uint16_t width;
    /// Height of the region.
    uint16_t height;

} LCD_ROI_t;



// =============================================================================
// LCD_Color_Format_t
// -----------------------------------------------------------------------------
/// This types describes how the color are encoded within a frame buffer.
// =============================================================================
typedef enum
{
    LCD_COLOR_FORMAT_RGB_INVALID,
    LCD_COLOR_FORMAT_RGB_565,
    LCD_COLOR_FORMAT_MAX
} LCD_Color_Format_t;


//=============================================================================
// LCD_FB_t
//-----------------------------------------------------------------------------
/// Frame buffer type: this type describes a frame buffer through its dimensions
/// and a pointer to the data of the buffer. It is mandatory for the buffer to
/// be able to hold the data describing all the pixels at the color format
/// choosen.
///(ie sizeof(buffer) = width*height*NbOfBytesPerPixelAtTheFormat(format))
//=============================================================================
typedef struct
{
    uint16_t*             buffer;
    uint16_t              width;
    uint16_t              height;
    LCD_Color_Format_t    colorFormat;
} LCD_FB_t;


//=============================================================================
// LCD_FBW_t
//-----------------------------------------------------------------------------
/// Frame buffer window. This type describes a window in a frame buffer as a
/// region of interest within this frame buffer. When the region of interest
/// starts at (0,0) and has the same dimensions as the frame buffer, the
/// window is in fact the whole frame buffer.
//=============================================================================
typedef struct
{
    LCD_FB_t   fb;
    LCD_ROI_t  roi;
} LCD_FBW_t;

typedef struct
{
    LCD_Error_t (*Open)(void);
    LCD_Error_t (*Close)(void);
    LCD_Error_t (*SetContrast)(uint32_t contrast);
    LCD_Error_t (*SetBrightness)(uint8_t brightness);
    LCD_Error_t (*SetStandbyMode)(bool standbyMode);
    LCD_Error_t (*Sleep)(void);
    LCD_Error_t (*PartialOn)(uint16_t vsa, uint16_t vea);
    LCD_Error_t (*PartialOff)(void);
    LCD_Error_t (*WakeUp)(void);
    LCD_Error_t (*GetScreenInfo)(LCD_Screen_Info_t* screenInfo);
    LCD_Error_t (*SetPixel16)(uint16_t x, uint16_t y, uint16_t pixelData);
    LCD_Error_t (*FillRect16)(const LCD_ROI_t* regionOfInterrest, uint16_t bgColor);
    LCD_Error_t (*Blit16)(const LCD_FBW_t* frameBufferWin, uint16_t startX, uint16_t startY);
    bool        (*Busy)(void);
    bool        (*SetDirRotation)(void);
    bool        (*SetDirDefault)(void);
    char*       (*GetStringId)(void);
    uint16_t    (*GetLcdId)(void);
    // void        (*OnBlt)(void*);
} LCD_OP_t;


extern LCD_Error_t LCD_ili9341_Register(LCD_OP_t* reg,void(*pOnBlit)(void*));

#ifdef __cplusplus
}
#endif

#endif
