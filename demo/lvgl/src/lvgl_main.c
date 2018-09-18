#include "stdint.h"
#include "stdbool.h"
#include "api_os.h"
#include "api_event.h"
#include "api_debug.h"
#include "api_hal_spi.h"
#include "api_hal_gpio.h"
#include "api_hal_pm.h"
#include "ssd1306.h"
#include "stdio.h"

#include "lvgl.h"


#define SSD1306_PIN_RST GPIO_PIN6
#define SSD1306_PIN_DC  GPIO_PIN7
#define SSD1306_SPI          SPI2


#define SSD1306_WIDTH   128  // x
#define SSD1306_HEIGHT  64   // y

///////////////////////////////////////////////////////////////////////////


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

static uint8_t oled_buffer[SSD1306_WIDTH*SSD1306_HEIGHT/8];
// static uint8_t oled_buffer2[SSD1306_WIDTH*SSD1306_HEIGHT];

void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_POWER_ON:
            break;
        case API_EVENT_ID_SYSTEM_READY:
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
    for(uint16_t i=0;i<SSD1306_WIDTH*SSD1306_HEIGHT/8;++i)
            oled_buffer[i] = 0x00;
    SSD1306_Init(SSD1306_WIDTH,SSD1306_HEIGHT,SSD1306_SPI,&spi_cfg,SSD1306_PIN_RST,SSD1306_PIN_DC);

    // for(uint16_t i=0;i<SSD1306_WIDTH*SSD1306_HEIGHT;++i)
    //     oled_buffer2[i] = 0x00;	
}

void lvgl_task_tick()
{
    lv_tick_inc(1);
    OS_StartCallbackTimer(mainTaskHandle,1,lvgl_task_tick,NULL);
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

    // OS_StartCallbackTimer(mainTaskHandle,1,lvgl_task_tick,NULL);

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
bool lvgl_init_flag = false;
void LVGL_Task(void* param)
{
    Init_Interface();
    Init_LVGL();
    lvgl_init_flag = true;
    Trace(3,"init complete");
    // //show hello world and height 16 bits

    /*************************************
     * Run the task handler of LittlevGL
     *************************************/
    while(1) {
        /* Periodically call this function.
         * The timing is not critical but should be between 1..10 ms */
        lv_tick_inc(5);
        lv_task_handler();
        OS_Sleep(5);
    }
}

void Display_Task(void* param)
{
    while(!lvgl_init_flag)
        OS_Sleep(200);
    
    Trace(3,"start display");
    
    /*Create a Label on the currently active screen*/
    lv_obj_t * label1 =  lv_label_create(lv_scr_act(), NULL);

    /*Modify the Label's text*/
    lv_label_set_text(label1, "Hello World!"SYMBOL_AUDIO);

    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

    while(1)
    {
        OS_Sleep(1000);
    }
}

void AppMainTask(void *pData)
{
    API_Event_t* event=NULL;
    OS_Sleep(3000);
            
    OS_CreateTask(LVGL_Task ,
        NULL, NULL, AppMain_TASK_STACK_SIZE, AppMain_TASK_PRIORITY+1, 0, 0, "lvgl Task");
    OS_CreateTask(Display_Task ,
        NULL, NULL, AppMain_TASK_STACK_SIZE, AppMain_TASK_PRIORITY+2, 0, 0, "display Task");
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
    uint8_t data ;
    uint16_t page;
    uint8_t  raw_in_page;
    Trace(3,"flush %d,%d %d,%d",x1,y1,x2,y2);
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
            /* put_px(x, y, *color_p)*/
            // oled_buffer2[y*SSD1306_WIDTH+x] = color_p->full;
            data = 1;
            page = y/8;
            raw_in_page = y%8;
            data <<= raw_in_page;
            oled_buffer[page*SSD1306_WIDTH+x] &= ~data;//clear this bit
            if(!color_p->full)
                data = 0; //not fill this bit
            oled_buffer[page*SSD1306_WIDTH+x] |= data;
            // SSD1306_SetPos(page,y);
            // SSD1306_WriteByte(data,SSD1306_DATA);
            color_p++;
        }
    }
    SSD1306_Show(oled_buffer);

    // uint8_t tmp[300];
    // for(uint16_t i=0;i<SSD1306_HEIGHT;++i)
    // {
    //     memset(tmp,0,sizeof(tmp));
    //     snprintf(tmp,sizeof(tmp),"%d: ",i);
    //     for(uint16_t j=0;j<SSD1306_WIDTH;++j)
    //     {
    //         snprintf(tmp+strlen(tmp),sizeof(tmp)-strlen(tmp),"%s ",oled_buffer2[i*SSD1306_WIDTH+j]?"$":"^");
            
    //     }
    //     Trace(5,tmp);
    // }
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
    uint8_t data ;
    uint16_t page;
    uint8_t  raw_in_page;
    Trace(3,"fill %d,%d %d,%d",x1,y1,x2,y2);
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
            /* put_px(x, y, *color)*/
            data = 1;
            page = y/8;
            raw_in_page = y%8;
            data <<= raw_in_page;
            oled_buffer[page*SSD1306_WIDTH+x] &= ~data;//clear this bit
            if(!color.full)
                data = 0; //not fill this bit
            oled_buffer[page*SSD1306_WIDTH+x] |= data;
        }
    }
    SSD1306_Show(oled_buffer);
}

/* Write a pixel array (called 'map') to the a specific area on the display
 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h*/
static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    uint8_t data ;
    uint16_t page;
    uint8_t  raw_in_page;
    Trace(3,"map %d,%d %d,%d",x1,y1,x2,y2);
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
            /* put_px(x, y, *color_p)*/
            data = 1;
            page = y/8;
            raw_in_page = y%8;
            data <<= raw_in_page;
            oled_buffer[page*SSD1306_WIDTH+x] &= ~data;//clear this bit
            if(!color_p->full)
                data = 0; //not fill this bit
            oled_buffer[page*SSD1306_WIDTH+x] |= data;
            color_p++;
        }
    }
    SSD1306_Show(oled_buffer);
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

