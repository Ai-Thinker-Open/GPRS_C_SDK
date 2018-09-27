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



#include "stdint.h"
#include "stdbool.h"

#include "api_hal_gouda.h"
#include "lcd_config_params.h"
#include "api_debug.h"
#include "api_os.h"
#include "api_sys.h"
#include "hal_iomux.h"

#include "lcd.h"




// =============================================================================
//
// -----------------------------------------------------------------------------
// =============================================================================
//
// =============================================================================
//  MACROS
// =============================================================================

#define LCM_WR_REG(Addr, Data)  { while(hal_GoudaWriteReg(Addr, Data)!= HAL_ERR_NO);}
#define LCM_WR_DAT(Data)        { while(hal_GoudaWriteData(Data)     != HAL_ERR_NO);}
#define LCM_WR_CMD(Cmd)         { while(hal_GoudaWriteCmd(Cmd)       != HAL_ERR_NO);}



// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

const HAL_GOUDA_LCD_CONFIG_T g_tgtLcddCfg = {
    {.cs             =   HAL_GOUDA_LCD_CS_0,
    .outputFormat   =   HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565,
    .cs0Polarity    =   false,
    .cs1Polarity    =   false,
    .rsPolarity     =   false,
    .wrPolarity     =   false,
    .rdPolarity     =   false,
    .resetb         =   true}
    };

bool g_lcddRotate = false;

// wheter lcddp_GoudaBlitHandler() has to close ovl layer 0
bool g_lcddAutoCloseLayer = false;

// Sleep status of the LCD
bool g_lcddInSleep = false;

// =============================================================================
//  FUNCTIONS
// =============================================================================

// Lock to protect the access to the LCD screen during a DMA access.
// When 1, access is allowed. When 0, it is denied.
static volatile uint32_t g_lcddLock = 0;

// =============================================================================
// lcdd_MutexFree
// -----------------------------------------------------------------------------
/// Free a previously taken mutex. The ownership of the mutex is not checked.
/// Only free a mutex you have previously got.
/// @param
// =============================================================================
static void lcdd_MutexFree(void)
{
    // Writing is an atomic operation
    g_lcddLock = 1;
}

// =============================================================================
// lcdd_MutexGet
// -----------------------------------------------------------------------------
/// This function enter in critical section, read the value of the mutex and,
/// if this is a '1', returns '1' and set the mutex to 0. If this is a '0',
/// it does nothing. Then, in both cases, it exists the Critical Section.
///
/// @param
/// @return '1' if the mutex was taken, '0' otherwise.
// =============================================================================
static uint32_t lcdd_MutexGet(void)
{
    uint32_t status;
    uint32_t result;

    status = SYS_EnterCriticalSection();
    if (g_lcddLock == 1)
    {
        // Take the mutex
        g_lcddLock = 0;
        result = 1;
    }
    else
    {
        // Too bad ...
        result = 0;
    }

    SYS_ExitCriticalSection(status);
    return result;
}

static void  (*pOnBlt)(void*) = NULL;

// =============================================================================
// lcddp_GoudaBlitHandler
// -----------------------------------------------------------------------------
/// This function frees the lock to access the screen. It is set as the user
/// handler called by the DMA driver at the end of the writings on the screen.
// =============================================================================
void lcddp_GoudaBlitHandler(void)
{
    lcdd_MutexFree();
    if(g_lcddAutoCloseLayer)
    {
        hal_GoudaOvlLayerClose(HAL_GOUDA_OVL_LAYER_ID0);
        g_lcddAutoCloseLayer = false;
    }
    if(pOnBlt)
        pOnBlt(NULL);
}


// =============================================================================
// lcddp_Init
// -----------------------------------------------------------------------------
/// This function initializes LCD registers after powering on or waking up.
// =============================================================================
static void lcddp_Init(void)
{
//************* Start Initial Sequence **********//
    LCM_WR_CMD(0x11); //Exit Sleep
    OS_Sleep(50);
    
    LCM_WR_CMD(0xCB); //AP[2:0]
    LCM_WR_DAT (0x01);

    LCM_WR_CMD(0xC0); //Power control
    LCM_WR_DAT (0x26); //VRH[5:0]
    LCM_WR_DAT (0x08); //VC[2:0]

    LCM_WR_CMD(0xC1); //Power control
    LCM_WR_DAT (0x10); //SAP[2:0];BT[3:0]

    LCM_WR_CMD(0xC5); //VCM control
    LCM_WR_DAT (0x35);
    LCM_WR_DAT (0x3E);

    LCM_WR_CMD(0x36); // Memory Access Control
    LCM_WR_DAT (0x48);

    LCM_WR_CMD(0xB1); // Frame Rate Control
    LCM_WR_DAT (0x00);
    LCM_WR_DAT (0x16);

    LCM_WR_CMD(0xB6); // Display Function Control
    LCM_WR_DAT (0x0A);
    LCM_WR_DAT (0x82);

    LCM_WR_CMD(0xC7); // VCOM Control , VMF[6:0]
    LCM_WR_DAT (0xB5);

    LCM_WR_CMD(0xF2); // 3Gamma Function Disable
    LCM_WR_DAT (0x00);

    LCM_WR_CMD(0x26); //Gamma curve selected
    LCM_WR_DAT (0x01);

    LCM_WR_CMD(0x3A);
    LCM_WR_DAT (0x55);

    LCM_WR_CMD(0xE0); //Set Gamma
    LCM_WR_DAT (0x1F);
    LCM_WR_DAT (0x1A);
    LCM_WR_DAT (0x18);
    LCM_WR_DAT (0x0A);
    LCM_WR_DAT (0x0F);
    LCM_WR_DAT (0x06);
    LCM_WR_DAT (0x45);
    LCM_WR_DAT (0x87);
    LCM_WR_DAT (0x32);
    LCM_WR_DAT (0x0A);
    LCM_WR_DAT (0x07);
    LCM_WR_DAT (0x02);
    LCM_WR_DAT (0x07);
    LCM_WR_DAT (0x05);
    LCM_WR_DAT (0x00);

    LCM_WR_CMD(0XE1); //Set Gamma
    LCM_WR_DAT (0x00);
    LCM_WR_DAT (0x25);
    LCM_WR_DAT (0x27);
    LCM_WR_DAT (0x05);
    LCM_WR_DAT (0x10);
    LCM_WR_DAT (0x09);
    LCM_WR_DAT (0x3A);
    LCM_WR_DAT (0x78);
    LCM_WR_DAT (0x4D);
    LCM_WR_DAT (0x05);
    LCM_WR_DAT (0x18);
    LCM_WR_DAT (0x0D);
    LCM_WR_DAT (0x38);
    LCM_WR_DAT (0x3A);
    LCM_WR_DAT (0x1F);
    LCM_WR_CMD(0x29); //Display on

}


// ============================================================================
// lcddp_Open
// ----------------------------------------------------------------------------
/// Open the LCDD driver.
/// It must be called before any call to any other function of this driver.
/// This function is to be called only once.
/// @return #LCD_ERROR_NONE or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
static LCD_Error_t lcddp_Open(void)
{
    hwp_iomux->pad_LCD_RSTB_cfg = IOMUX_PAD_LCD_RSTB_SEL(IOMUX_PAD_LCD_RSTB_SEL_FUN_LCD_RSTB_SEL);
    hwp_iomux->pad_SPI_LCD_CS_cfg  = IOMUX_PAD_SPI_LCD_CS_SEL(IOMUX_PAD_SPI_LCD_CS_SEL_FUN_SPI_LCD_CS_SEL);
    hwp_iomux->pad_SPI_LCD_SCK_cfg = IOMUX_PAD_SPI_LCD_SCK_SEL(IOMUX_PAD_SPI_LCD_SCK_SEL_FUN_SPI_LCD_SCK_SEL);
    hwp_iomux->pad_SPI_LCD_DIO_cfg = IOMUX_PAD_SPI_LCD_DIO_SEL(IOMUX_PAD_SPI_LCD_DIO_SEL_FUN_SPI_LCD_DIO_SEL);
    hwp_iomux->pad_SPI_LCD_SDC_cfg = IOMUX_PAD_SPI_LCD_SDC_SEL(IOMUX_PAD_SPI_LCD_SDC_SEL_FUN_SPI_LCD_SDC_SEL);
    Trace(1,"ili9341 lcddp_Open");
    hal_GoudaSerialOpen(LCDD_SPI_LINE_TYPE, LCDD_SPI_FREQ, &g_tgtLcddCfg, 0);

    // hal_GoudaSetBlock(1);//block wait for op complete
    // Init code
    OS_Sleep(50);

    lcddp_Init();

    g_lcddInSleep = false;    

    return LCD_ERROR_NONE;
}


// ============================================================================
// lcddp_Close
// ----------------------------------------------------------------------------
/// Close the LCDD driver
/// No other functions of this driver should be called after a call to
/// #lcddp_Close.
/// @return #LCD_ERROR_NONE or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
LCD_Error_t lcddp_Close(void)
{

    hal_GoudaClose();

    return LCD_ERROR_NONE;
}


// ============================================================================
// lcddp_SetContrast
// ----------------------------------------------------------------------------
/// Set the contrast of the 'main'LCD screen.
/// @param contrast Value to set the contrast to.
/// @return #LCD_ERROR_NONE, #LCD_ERROR_NONET_OPENED or
/// #LCD_ERROR_INVALID_PARAMETER.
// ============================================================================
LCD_Error_t lcddp_SetContrast(uint32_t contrast)
{
    //#warning This function is not implemented yet
    return LCD_ERROR_NONE;
}

// ============================================================================
// lcddp_Sleep
// ----------------------------------------------------------------------------
/// Set the main LCD screen in sleep mode.
/// @return #LCD_ERROR_NONE or #LCD_ERROR_NONET_OPENED
// ============================================================================
LCD_Error_t lcddp_Sleep(void)
{
    while (0 == lcdd_MutexGet())
    {
        OS_Sleep(LCDD_TIME_MUTEX_RETRY);
        Trace(1,"LCDD: Sleep while another LCD operation in progress. Sleep %d ticks",
                   LCDD_TIME_MUTEX_RETRY);
    }

    if (g_lcddInSleep)
    {
        lcdd_MutexFree();
        return LCD_ERROR_NONE;
    }

    LCM_WR_CMD(0x0028); // Display off
    LCM_WR_CMD(0x0010); // Enter Standby mode

    Trace(1,"lcddp_Sleep: calling hal_GoudaClose");
    hal_GoudaClose();

    g_lcddInSleep = true;

    lcdd_MutexFree();

    return LCD_ERROR_NONE;
}


// ============================================================================
// lcddp_PartialOn
// ----------------------------------------------------------------------------
/// Set the Partial mode of the LCD
/// @param vsa : Vertical Start Active
/// @param vea : Vertical End Active
/// @return #LCD_ERROR_NONE, #LCD_ERROR_NONET_OPENED
// ============================================================================
LCD_Error_t lcddp_PartialOn(uint16_t vsa, uint16_t vea)
{
    return LCD_ERROR_NONE;
}


// ============================================================================
// lcddp_PartialOff
// ----------------------------------------------------------------------------
/// return to Normal Mode
/// @return #LCD_ERROR_NONE, #LCD_ERROR_NONET_OPENED
// ============================================================================
LCD_Error_t lcddp_PartialOff(void)
{
    return LCD_ERROR_NONE;
}


// ============================================================================
// lcddp_BlitRoi2Win
// ----------------------------------------------------------------------------
// Private function to transfer data to the LCD
// ============================================================================
void lcddp_BlitRoi2Win(const HAL_GOUDA_WINDOW_T* pRoi, const HAL_GOUDA_WINDOW_T* pActiveWin)
{
    HAL_GOUDA_LCD_CMD_T cmd[13];

    if(!((pRoi->tlPX < pRoi->brPX) && (pRoi->tlPY < pRoi->brPY)))
    {
        Trace(1,"lcddp_BlitRoi2Win: Invalid Roi x:%d < %d, y:%d < %d",pRoi->tlPX, pRoi->brPX, pRoi->tlPY, pRoi->brPY);
        lcddp_GoudaBlitHandler();
        return;
    }

    // building set roi sequence:

    if(g_lcddRotate)
    {

        LCM_WR_CMD(0x2a);    //Set Column Address
        LCM_WR_DAT(pActiveWin->tlPX>>8);
        LCM_WR_DAT(pActiveWin->tlPX&0x00ff);
        LCM_WR_DAT(pActiveWin->brPX>>8);
        LCM_WR_DAT(pActiveWin->brPX&0x00ff);


        LCM_WR_CMD(0x2b);    //Set Page Address
        LCM_WR_DAT(pActiveWin->tlPY>>8);
        LCM_WR_DAT(pActiveWin->tlPY&0x00ff);
        LCM_WR_DAT(pActiveWin->brPY>>8);
        LCM_WR_DAT(pActiveWin->brPY&0x00ff);
    }
    else
    {
        LCM_WR_CMD(0x2a);    //Set Column Address
        LCM_WR_DAT(pActiveWin->tlPX>>8);
        LCM_WR_DAT(pActiveWin->tlPX&0x00ff);
        LCM_WR_DAT(pActiveWin->brPX>>8);
        LCM_WR_DAT(pActiveWin->brPX&0x00ff);


        LCM_WR_CMD(0x2b);    //Set Page Address
        LCM_WR_DAT(pActiveWin->tlPY>>8);
        LCM_WR_DAT(pActiveWin->tlPY&0x00ff);
        LCM_WR_DAT(pActiveWin->brPY>>8);
        LCM_WR_DAT(pActiveWin->brPY&0x00ff);

        //LCM_WR_CMD(0x2c); //WRITE ram Data display
    }

    // Send command after which the data we sent
    // are recognized as pixels.
    LCM_WR_CMD(0x2c);
    while (HAL_ERR_NO != hal_GoudaBlitRoi(pRoi, 0, cmd, lcddp_GoudaBlitHandler));

}


// ============================================================================
// lcddp_Blit16
// ----------------------------------------------------------------------------
/// This function provides the basic bit-block transfer capabilities.
/// This function copies the data (such as characters/bmp) on the LCD directly
/// as a (rectangular) block. The data is drawn in the active window.
/// The buffer has to be properly aligned (@todo define properly 'properly')
///
/// @param pPixelData Pointer to the buffer holding the data to be displayed
/// as a block. The dimension of this block are the one of the #pDestRect
/// parameter
/// @return #LCD_ERROR_NONE, #LCD_ERROR_RESOURCE_BUSY or #LCD_ERROR_NONET_OPENED.
// ============================================================================
LCD_Error_t lcddp_Blit16(const LCD_FBW_t* frameBufferWin, uint16_t startX, uint16_t startY)
{
    // LCDD_ASSERT((frameBufferWin->fb.width&1) == 0, "LCDD: FBW must have an even number "
    //             "of pixels per line. Odd support is possible at the price of a huge "
    //             "performance lost");
    
    // Active window coordinates.
    HAL_GOUDA_WINDOW_T inputWin;
    HAL_GOUDA_WINDOW_T activeWin;

    if (0 == lcdd_MutexGet())
    {
        return LCD_ERROR_RESOURCE_BUSY;
    }
    else
    {
        if (g_lcddInSleep)
        {
            lcdd_MutexFree();
            return LCD_ERROR_NONE;
        }

        // Set Input window
        inputWin.tlPX = frameBufferWin->roi.x;
        inputWin.brPX = frameBufferWin->roi.x + frameBufferWin->roi.width - 1;
        inputWin.tlPY = frameBufferWin->roi.y;
        inputWin.brPY = frameBufferWin->roi.y + frameBufferWin->roi.height - 1;

        // Set Active window
        activeWin.tlPX = startX;
        activeWin.brPX = startX + frameBufferWin->roi.width - 1;
        activeWin.tlPY = startY;
        activeWin.brPY = startY + frameBufferWin->roi.height - 1;

        // Check parameters
        // ROI must be within the screen boundary
        // ROI must be within the Frame buffer
        // Color format must be 16 bits
        bool badParam = false;
        if (g_lcddRotate)
        {
            if (    (activeWin.tlPX >= LCDD_DISP_Y) ||
                    (activeWin.brPX >= LCDD_DISP_Y) ||
                    (activeWin.tlPY >= LCDD_DISP_X) ||
                    (activeWin.brPY >= LCDD_DISP_X)
               )
            {
                badParam = true;
            }
        }
        else
        {
            if (    (activeWin.tlPX >= LCDD_DISP_X) ||
                    (activeWin.brPX >= LCDD_DISP_X) ||
                    (activeWin.tlPY >= LCDD_DISP_Y) ||
                    (activeWin.brPY >= LCDD_DISP_Y)
               )
            {
                badParam = true;
            }
        }
        if (!badParam)
        {
            if (    (frameBufferWin->roi.width > frameBufferWin->fb.width) ||
                    (frameBufferWin->roi.height > frameBufferWin->fb.height) ||
                    (frameBufferWin->fb.colorFormat != LCD_COLOR_FORMAT_RGB_565)
               )
            {
                badParam = true;;
            }
        }
        if (badParam)
        {
            lcdd_MutexFree();
            return LCD_ERROR_INVALID_PARAMETER;
        }

        // this will allow to keep LCDD interface for blit while using gouda
        // directly for configuring layers
        if (frameBufferWin->fb.buffer != NULL)
        {
            HAL_GOUDA_OVL_LAYER_DEF_T def;
            // configure ovl layer 0 as buffer
            def.addr = (uint32_t*)frameBufferWin->fb.buffer; // what about aligment ?
            def.fmt = HAL_GOUDA_IMG_FORMAT_RGB565; //TODO convert from .colorFormat
            //def.stride = frameBufferWin->fb.width * 2;
            def.stride = 0; // let hal gouda decide
            def.pos.tlPX = 0;
            def.pos.tlPY = 0;
            def.pos.brPX = frameBufferWin->fb.width - 1;
            def.pos.brPY = frameBufferWin->fb.height - 1;
            def.alpha = 255;
            def.cKeyEn = false;

            // open the layer
            hal_GoudaOvlLayerClose(HAL_GOUDA_OVL_LAYER_ID0);
            hal_GoudaOvlLayerOpen(HAL_GOUDA_OVL_LAYER_ID0, &def);
            // tell the end handler not to close the layer when we are done
            g_lcddAutoCloseLayer = false;
        }

        // gouda is doing everything ;)
        lcddp_BlitRoi2Win(&inputWin, &activeWin);

        return LCD_ERROR_NONE;
    }
}


// ============================================================================
// lcddp_WakeUp
// ----------------------------------------------------------------------------
/// Wake the main LCD screen out of sleep mode
/// @return #LCD_ERROR_NONE, #LCD_ERROR_NONET_OPENED
// ============================================================================
LCD_Error_t lcddp_WakeUp(void)
{
    if (0 == lcdd_MutexGet())
    {
        OS_Sleep(LCDD_TIME_MUTEX_RETRY);
        Trace(1,"LCDD: Wakeup while another LCD operation in progress. Sleep %d ticks",
                   LCDD_TIME_MUTEX_RETRY);
    }

    if (!g_lcddInSleep)
    {
        lcdd_MutexFree();
        return LCD_ERROR_NONE;
    }

    Trace(1,"lcddp_WakeUp: calling hal_GoudaOpen");
    hal_GoudaSerialOpen(LCDD_SPI_LINE_TYPE, LCDD_SPI_FREQ, &g_tgtLcddCfg, 0);

#if 0
    LCM_WR_REG(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCM_WR_REG(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    LCM_WR_REG(0x0012, 0x0000); // VREG1OUT voltage
    LCM_WR_REG(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude

    sxr_Sleep(100 MS_WAITING); // Delay 50ms

    LCM_WR_REG(0x0010, 0x1290); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCM_WR_REG(0x0011, 0x0227); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]

    OS_Sleep(50); // Delay 50ms

    LCM_WR_REG(0x0012, 0x0091); // External reference voltage= Vci

    OS_Sleep(50); // Delay 50ms

    LCM_WR_REG(0x0013, 0x1c00); // VDV[4:0] for VCOM amplitude
    LCM_WR_REG(0x0029, 0x003b); // VCM[5:0] for VCOMH

    OS_Sleep(50); // Delay 50ms

    LCM_WR_REG(0x0007, 0x0133); // 262K color and display ON
#else
    // Init code
    OS_Sleep(50);                         // Delay 50 ms
    lcddp_Init();
#endif

    g_lcddInSleep = false;

    lcdd_MutexFree();

    // Set a comfortable background color to avoid screen flash
    LCD_FBW_t frameBufferWin;
    frameBufferWin.fb.buffer = NULL;
    frameBufferWin.fb.colorFormat = LCD_COLOR_FORMAT_RGB_565;
    frameBufferWin.roi.x=0;
    frameBufferWin.roi.y=0;

    if (g_lcddRotate)
    {
        frameBufferWin.roi.width = LCDD_DISP_Y;
        frameBufferWin.roi.height = LCDD_DISP_X;
        frameBufferWin.fb.width = LCDD_DISP_Y;
        frameBufferWin.fb.height = LCDD_DISP_X;
    }
    else
    {
        frameBufferWin.roi.width = LCDD_DISP_X;
        frameBufferWin.roi.height = LCDD_DISP_Y;
        frameBufferWin.fb.width = LCDD_DISP_X;
        frameBufferWin.fb.height = LCDD_DISP_Y;
    }
    lcddp_Blit16(&frameBufferWin,frameBufferWin.roi.x,frameBufferWin.roi.y);


    return LCD_ERROR_NONE;
}

// ============================================================================
// lcddp_SetStandbyMode
// ----------------------------------------------------------------------------
/// Set the main LCD in standby mode or exit from it
/// @param standbyMode If \c true, go in standby mode.
///                    If \c false, cancel standby mode.
/// @return #LCD_ERROR_NONE, #LCD_ERROR_NONET_OPENED or
/// #LCD_ERROR_INVALID_PARAMETER.
// ============================================================================
LCD_Error_t lcddp_SetStandbyMode(bool standbyMode)
{
    if (standbyMode)
    {
        lcddp_Sleep();
    }
    else
    {
        lcddp_WakeUp();
    }
    return LCD_ERROR_NONE;
}


// ============================================================================
// lcddp_GetScreenInfo
// ----------------------------------------------------------------------------
/// Get information about the main LCD device.
/// @param screenInfo Pointer to the structure where the information
/// obtained will be stored
/// @return #LCD_ERROR_NONE, #LCD_ERROR_NONET_OPENED or
/// #LCD_ERROR_INVALID_PARAMETER.
// ============================================================================
LCD_Error_t lcddp_GetScreenInfo(LCD_Screen_Info_t* screenInfo)
{
    screenInfo->width = LCDD_DISP_X;
    screenInfo->height = LCDD_DISP_Y;
    screenInfo->bitdepth = LCD_COLOR_FORMAT_RGB_565;
    screenInfo->nReserved = 0;
    return LCD_ERROR_NONE;
}


// ============================================================================
// lcddp_SetPixel16
// ----------------------------------------------------------------------------
/// Draw a 16-bit pixel a the specified position.
/// @param x X coordinate of the point to set.
/// @param y Y coordinate of the point to set.
/// @param pixelData 16-bit pixel data to draw.
/// @return #LCD_ERROR_NONE, #LCD_ERROR_RESOURCE_BUSY or #LCD_ERROR_NONET_OPENED.
// ============================================================================
LCD_Error_t lcddp_SetPixel16(uint16_t x, uint16_t y, uint16_t pixelData)
{
    if (0 == lcdd_MutexGet())
    {
        return LCD_ERROR_RESOURCE_BUSY;
    }
    else
    {
        if (g_lcddInSleep)
        {
            lcdd_MutexFree();
            return LCD_ERROR_NONE;
        }
        LCM_WR_CMD(0x2a);    //Set Column Address
        LCM_WR_DAT(x>>8);
        LCM_WR_DAT(x&0x00ff);
        LCM_WR_DAT(x>>8);
        LCM_WR_DAT(x&0x00ff);


        LCM_WR_CMD(0x2b);    //Set Page Address
        LCM_WR_DAT(y>>8);
        LCM_WR_DAT(y&0x00ff);
        LCM_WR_DAT(y>>8);
        LCM_WR_DAT(y&0x00ff);
        LCM_WR_REG(0x2c, pixelData);

        lcdd_MutexFree();
        return LCD_ERROR_NONE;
    }
}



// ============================================================================
// lcddp_FillRect16
// ----------------------------------------------------------------------------
/// This function performs a fill of the active window  with some color.
/// @param bgColor Color with which to fill the rectangle. It is a 16-bit
/// data, as the one of #lcddp_SetPixel16
/// @return #LCD_ERROR_NONE, #LCD_ERROR_RESOURCE_BUSY or #LCD_ERROR_NONET_OPENED.
// ============================================================================
LCD_Error_t lcddp_FillRect16(const LCD_ROI_t* regionOfInterrest, uint16_t bgColor)
{
    // Active window coordinates.
    HAL_GOUDA_WINDOW_T activeWin;

    if (0 == lcdd_MutexGet())
    {
        return LCD_ERROR_RESOURCE_BUSY;
    }
    else
    {
        if (g_lcddInSleep)
        {
            lcdd_MutexFree();
            return LCD_ERROR_NONE;
        }

        // Set Active window
        activeWin.tlPX = regionOfInterrest->x;
        activeWin.brPX = regionOfInterrest->x + regionOfInterrest->width - 1;
        activeWin.tlPY = regionOfInterrest->y;
        activeWin.brPY = regionOfInterrest->y + regionOfInterrest->height - 1;

        // Check parameters
        // ROI must be within the screen boundary
        bool badParam = false;
        if (g_lcddRotate)
        {
            if (    (activeWin.tlPX >= LCDD_DISP_Y) ||
                    (activeWin.brPX >= LCDD_DISP_Y) ||
                    (activeWin.tlPY >= LCDD_DISP_X) ||
                    (activeWin.brPY >= LCDD_DISP_X)
               )
            {
                badParam = true;
            }
        }
        else
        {
            if (    (activeWin.tlPX >= LCDD_DISP_X) ||
                    (activeWin.brPX >= LCDD_DISP_X) ||
                    (activeWin.tlPY >= LCDD_DISP_Y) ||
                    (activeWin.brPY >= LCDD_DISP_Y)
               )
            {
                badParam = true;
            }
        }
        if (badParam)
        {
            lcdd_MutexFree();
            return LCD_ERROR_INVALID_PARAMETER;
        }

        hal_GoudaSetBgColor(bgColor);
        lcddp_BlitRoi2Win(&activeWin,&activeWin);

        return LCD_ERROR_NONE;
    }
}



// ============================================================================
// lcddp_Busy
// ----------------------------------------------------------------------------
/// This function is not implemented for the ebc version of the driver
// ============================================================================
bool lcddp_Busy(void)
{
    return hal_GoudaIsActive();
}


// ============================================================================
// lcddp_SetDirRotation
// ----------------------------------------------------------------------------
///
// ============================================================================
bool lcddp_SetDirRotation(void)
{
    while (0 == lcdd_MutexGet())
    {
        OS_Sleep(LCDD_TIME_MUTEX_RETRY);
    }

    g_lcddRotate = true;

    if (g_lcddInSleep)
    {
        lcdd_MutexFree();
        return true;
    }

    LCM_WR_CMD(0x36); // Memory Access Control
    LCM_WR_DAT (0x28);

    lcdd_MutexFree();

    return true;
}

// ============================================================================
// lcddp_SetDirDefault
// ----------------------------------------------------------------------------
///
// ============================================================================
bool lcddp_SetDirDefault(void)
{
    while (0 == lcdd_MutexGet())
    {
        OS_Sleep(LCDD_TIME_MUTEX_RETRY);
    }

    g_lcddRotate = false;

    if (g_lcddInSleep)
    {
        lcdd_MutexFree();
        return true;
    }

    LCM_WR_CMD(0x36); // Memory Access Control
    LCM_WR_DAT (0x48);

    lcdd_MutexFree();

    return true;
}

char* lcddp_GetStringId(void)
{
    static char ili9341_id_str[] = "ili9341\n";
    return ili9341_id_str;
}



bool lcddp_CheckProductId()
{
#ifdef LCD_NO_PRODUCT_ID
    return true;
#else
    uint16_t productIds[4];
    uint16_t productId=0;

    hal_GoudaSerialOpen(LCDD_SPI_LINE_TYPE, LCDD_SPI_FREQ, &g_tgtLcddCfg, 0);
    OS_Sleep(20);

    hal_GoudaReadData(0xd3,productIds,4);

    productId=productIds[2]<<8;
    productId=(productId&0xff00)|(productIds[3]&0xff);
    hal_GoudaClose();
    Trace(1,"ili9341(0x%x): lcd read id is 0x%x ", LCD_ILI9341_ID, productId);

    if(productId == LCD_ILI9341_ID)
        return true;
    else
        return false;
#endif
}

// ============================================================================
// lcdd_ili9341_RegInit
// ----------------------------------------------------------------------------
/// register the right lcd driver, according to lcddp_CheckProductId
/// @return #true, #false
// ============================================================================
// PUBLIC bool  lcdd_ili9341_RegInit(LCDD_REG_T *pLcdDrv)
// {
//     if( lcddp_CheckProductId())
//     {
//         pLcdDrv->lcdd_Open=lcddp_Open;
//         pLcdDrv->lcdd_Close=lcddp_Close;
//         pLcdDrv->lcdd_SetContrast=lcddp_SetContrast;

//         pLcdDrv->lcdd_SetStandbyMode=lcddp_SetStandbyMode;
//         pLcdDrv->lcdd_PartialOn=lcddp_PartialOn;
//         pLcdDrv->lcdd_PartialOff=lcddp_PartialOff;
//         pLcdDrv->lcdd_Blit16=lcddp_Blit16;
//         pLcdDrv->lcdd_Busy=lcddp_Busy;
//         pLcdDrv->lcdd_FillRect16=lcddp_FillRect16;
//         pLcdDrv->lcdd_GetScreenInfo=lcddp_GetScreenInfo;
//         pLcdDrv->lcdd_WakeUp=lcddp_WakeUp;
//         pLcdDrv->lcdd_SetPixel16=lcddp_SetPixel16;
//         pLcdDrv->lcdd_Sleep=lcddp_Sleep;
//         pLcdDrv->lcdd_SetDirRotation=lcddp_SetDirRotation;
//         pLcdDrv->lcdd_SetDirDefault=lcddp_SetDirDefault;
//         pLcdDrv->lcdd_GetStringId=lcdd_get_id_string;
//         pLcdDrv->lcdd_GoudaBltHdl = lcddp_GoudaBlitHandler;
//         return true;
//     }

//     return false;
// }

LCD_Error_t lcddp_SetBrightness(uint8_t brightness)
{
    return LCD_ERROR_NONE;
}

uint16_t lcddp_GetLcdId()
{
    return 0x0000;
}



#include "lcd.h"

LCD_Error_t LCD_ili9341_Register(LCD_OP_t* reg,void(*onBlit)(void*))
{
    // if( lcddp_CheckProductId())
    {
        reg->Open              = lcddp_Open;
        reg->Close             = lcddp_Close;
        reg->SetContrast       = lcddp_SetContrast;
        reg->SetBrightness     = lcddp_SetBrightness;
        reg->SetStandbyMode    = lcddp_SetStandbyMode;
        reg->Sleep             = lcddp_Sleep;
        reg->PartialOn         = lcddp_PartialOn;
        reg->PartialOff        = lcddp_PartialOff;
        reg->WakeUp            = lcddp_WakeUp;
        reg->GetScreenInfo     = lcddp_GetScreenInfo;
        reg->SetPixel16        = lcddp_SetPixel16;
        reg->FillRect16        = lcddp_FillRect16;
        reg->Blit16            = lcddp_Blit16;
        reg->Busy              = lcddp_Busy;
        reg->SetDirRotation    = lcddp_SetDirRotation;
        reg->SetDirDefault     = lcddp_SetDirDefault;
        reg->GetStringId       = lcddp_GetStringId;
        reg->GetLcdId          = lcddp_GetLcdId;
        // reg->OnBlt             = pOnBlit;
        pOnBlt = onBlit;
    }
    lcdd_MutexFree();
    return LCD_ERROR_NONE;
}



