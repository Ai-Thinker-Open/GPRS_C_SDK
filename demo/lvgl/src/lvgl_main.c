#include "stdint.h"
#include "stdbool.h"
#include "api_os.h"
#include "api_event.h"
#include "api_debug.h"
#include "api_hal_spi.h"
#include "api_hal_gpio.h"
#include "api_hal_pm.h"
#include "ssd1306.h"

#include "lvgl.h"


#define SSD1306_PIN_RST GPIO_PIN6
#define SSD1306_PIN_DC  GPIO_PIN7
#define SSD1306_SPI          SPI2


#define SSD1306_WIDTH   128  // x
#define SSD1306_HEIGHT  64   // y

///////////////////////////////////////////////////////////////////////////

const unsigned char bmp_rocket[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xE0,0xE0,0xF0,0xF0,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF0,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xF0,0xF8,0xF8,0xFC,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE7,0x83,0x81,0x81,0x81,0xC3,0xFF,0xFF,0xFF,0xFF,0x7F,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x1F,0x07,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x07,0x87,0x87,0x87,0x87,0x87,0x87,0x0F,0x1F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x1F,0x0F,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xF0,0xFC,0xFE,0xFF,0xFF,0xFF,0xE7,0xF3,0xF1,0xF0,0xF0,0xF0,0xF8,0xFC,0x38,0x00,0x01,0x73,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x1E,0x1F,0x0F,0x07,0x07,0x03,0x0F,0x0F,0x07,0x03,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x07,0x1F,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};/* (48 X 48 )*/
//列行式



#define AppMain_TASK_STACK_SIZE    (1024 * 2)
#define AppMain_TASK_PRIORITY      0
HANDLE mainTaskHandle  = NULL;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
static void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color);
#if USE_LV_GPU
static void ex_mem_blend(lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa);
static void ex_mem_fill(lv_color_t * dest, uint32_t length, lv_color_t color);
#endif
static bool ex_tp_read(lv_indev_data_t * data);


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_POWER_ON:
            break;
        case API_EVENT_ID_NO_SIMCARD:
            break;
        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            break;
        default:
            break;
    }
}



void Init_Interface()
{
    // init spi
	SPI_Config_t spi_cfg =
    {
	    .cs = SPI_CS_0,
        .txMode = SPI_MODE_DIRECT_POLLING,
        .rxMode = SPI_MODE_DIRECT_POLLING,
        .freq = 40000000,//40M here
        .line = SPI_LINE_3,
        .txOnly = false,
        .cpol = 0,
        .cpha = 0,
        .csActiveLow = true,
        .dataBits = SPI_DATA_BITS_8,
        .irqHandler = NULL,
        .irqMask = {0,0,0,0,0}
    };
    SSD1306_Init(SSD1306_WIDTH,SSD1306_HEIGHT,SSD1306_SPI,&spi_cfg,SSD1306_PIN_RST,SSD1306_PIN_DC);
	
}

void Init_LVGL()
{
    lv_init();

    lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/
    disp_drv.disp_flush = ex_disp_flush;            /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/

    disp_drv.disp_fill = ex_disp_fill;              /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
    disp_drv.disp_map = ex_disp_map;                /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
#if USE_LV_GPU
    /*Optionally add functions to access the GPU. (Only in buffered mode, LV_VDB_SIZE != 0)*/
    disp_drv.mem_blend = ex_mem_blend;              /*Blend two color array using opacity*/
    disp_drv.mem_fill = ex_mem_fill;                /*Fill a memory array with a color*/
#endif

   /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);

    /*************************
     * Input device interface
     *************************/
    /*Add a touchpad in the example*/
    /*touchpad_init();*/                            /*Initialize your touchpad*/
    lv_indev_drv_t indev_drv;                       /*Descriptor of an input device driver*/
    lv_indev_drv_init(&indev_drv);                  /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;         /*The touchpad is pointer type device*/
    indev_drv.read = ex_tp_read;                 /*Library ready your touchpad via this function*/
    lv_indev_drv_register(&indev_drv);              /*Finally register the driver*/

}

void SSD1306_Task(void* param)
{
    Init_Interface();
    Init_LVGL();

    // //show hello world and height 16 bits
    // SSD1306_ShowString(8,0,"==hello oled==",16);

    /*************************************
     * Run the task handler of LittlevGL
     *************************************/
    while(1) {
        /* Periodically call this function.
         * The timing is not critical but should be between 1..10 ms */
        lv_task_handler();
        OS_Sleep(5);
    }

    // while(1)
    // {
    //     for(int8_t i=0;i<(128-48);++i)
    //     {
    //         SSD1306_DrawBMP(i,47+i,2,7,(unsigned char*)bmp_rocket);
    //         OS_Sleep(10);
    //     }
    //     for(int8_t i=(128-48);i>=0;--i)
    //     {
    //         SSD1306_DrawBMP(i,47+i,2,7,(unsigned char*)bmp_rocket);
    //         OS_Sleep(10);
    //     }
    // }
}

void AppMainTask(void *pData)
{
    API_Event_t* event=NULL;
            
    OS_CreateTask(SSD1306_Task ,
        NULL, NULL, AppMain_TASK_STACK_SIZE, AppMain_TASK_PRIORITY+1, 0, 0, "lvgl Task");
    while(1)
    {
        if(OS_WaitEvent(mainTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}
void lvgl_Main(void)
{
    mainTaskHandle = OS_CreateTask(AppMainTask ,
        NULL, NULL, AppMain_TASK_STACK_SIZE, AppMain_TASK_PRIORITY, 0, 0, "init Task");
    OS_SetUserMainHandle(&mainTaskHandle);
}



/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_flush_ready()' has to be called when finished
 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h*/
static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
            /* put_px(x, y, *color_p)*/
            // SSD1306_SetPos(x/8,y);
            Trace(3,"flush %d,%d %d,%d",x1,y1,x2,y2);
            color_p++;
        }
    }

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_flush_ready();
}


/* Write a pixel array (called 'map') to the a specific area on the display
 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h*/
static void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
            /* put_px(x, y, *color)*/
            Trace(3,"fill %d,%d %d,%d",x1,y1,x2,y2);
        }
    }
}

/* Write a pixel array (called 'map') to the a specific area on the display
 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h*/
static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
            /* put_px(x, y, *color_p)*/
            Trace(3,"map %d,%d %d,%d",x1,y1,x2,y2);
            color_p++;
        }
    }
}


#if USE_LV_GPU

/* If your MCU has hardware accelerator (GPU) then you can use it to blend to memories using opacity
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void ex_mem_blend(lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa)
{
    /*It's an example code which should be done by your GPU*/

    int32_t i;
    for(i = 0; i < length; i++) {
        dest[i] = lv_color_mix(dest[i], src[i], opa);
    }
}

/* If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void ex_mem_fill(lv_color_t * dest, uint32_t length, lv_color_t color)
{
    /*It's an example code which should be done by your GPU*/

    int32_t i;
    for(i = 0; i < length; i++) {
        dest[i] = color;
    }
}

#endif

/* Read the touchpad and store it in 'data'
 * Return false if no more data read; true for ready again */
static bool ex_tp_read(lv_indev_data_t *data)
{
    /* Read your touchpad */
    /* data->state = LV_INDEV_STATE_REL or LV_INDEV_STATE_PR */
    /* data->point.x = tp_x; */
    /* data->point.y = tp_y; */

    return false;   /*false: no more data to read because we are no buffering*/
}
