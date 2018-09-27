#include "stdint.h"
#include "stdbool.h"
#include "api_os.h"
#include "api_event.h"
#include "api_debug.h"
#include "api_hal_spi.h"
#include "api_hal_gpio.h"
#include "api_hal_pm.h"
#include "stdio.h"

#include "lcd.h"
#include "lvgl.h"

///////////////////////////////////////////////////////////////////////////
static uint16_t lcd_width  = 0;
static uint16_t lcd_height = 0;

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
        case API_EVENT_ID_SYSTEM_READY:

            break;
        default:
            break;
    }
}


LCD_OP_t lcd = {
    .Open              = NULL,
    .Close             = NULL,
    .SetContrast       = NULL,
    .SetBrightness     = NULL,
    .SetStandbyMode    = NULL,
    .Sleep             = NULL,
    .PartialOn         = NULL,
    .PartialOff        = NULL,
    .WakeUp            = NULL,
    .GetScreenInfo     = NULL,
    .SetPixel16        = NULL,
    .FillRect16        = NULL,
    .Blit16            = NULL,
    .Busy              = NULL,
    .SetDirRotation    = NULL,
    .SetDirDefault     = NULL,
    .GetStringId       = NULL,
    .GetLcdId          = NULL,
};


void OnBlit(void* param);

void Init_Interface()
{
    LCD_Screen_Info_t info;

    //register functions by lcd driver
    LCD_ili9341_Register(&lcd,OnBlit);
    lcd.Open();
    lcd.GetScreenInfo(&info);
    lcd_width = info.width;
    lcd_height= info.height;

    LCD_ROI_t roi = {
        .x = 0,
        .y = 0,
        .width = info.width,
        .height = info.height
    };
    lcd.FillRect16(&roi,0xffff);
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

    OS_StartCallbackTimer(mainTaskHandle,1,lvgl_task_tick,NULL);

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
        // lv_tick_inc(1);
        lv_task_handler();
        OS_Sleep(1);
    }
}


/**********************
 *  STATIC VARIABLES
 **********************/
lv_style_t btn3_style;

/**
 * Crate some objects an animate them
 */
void lv_tutorial_animations(void)
{
    lv_obj_t * label;


    /*Create a button the demonstrate built-in animations*/
    lv_obj_t * btn1;
    btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_pos(btn1, 10, 10);       /*Set a position. It will be the animation's destination*/
    lv_obj_set_size(btn1, 80, 50);

    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Float");

    /* Float in the button using a built-in function
     * Delay the animation with 2000 ms and float in 300 ms. NULL means no end callback*/
    lv_obj_animate(btn1, LV_ANIM_FLOAT_TOP | LV_ANIM_IN, 300, 2000, NULL);

    /*Create a button to demonstrate user defined animations*/
    lv_obj_t * btn2;
    btn2 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_pos(btn2, 10, 80);       /*Set a position. It will be the animation's destination*/
    lv_obj_set_size(btn2, 80, 50);

    label = lv_label_create(btn2, NULL);
    lv_label_set_text(label, "Move");

    /*Create an animation to move the button continuously left to right*/
    lv_anim_t a;
    a.var = btn2;
    a.start = lv_obj_get_x(btn2);
    a.end = a.start + (100);
    a.fp = (lv_anim_fp_t)lv_obj_set_x;
    a.path = lv_anim_path_linear;
    a.end_cb = NULL;
    a.act_time = -1000;                         /*Negative number to set a delay*/
    a.time = 400;                               /*Animate in 400 ms*/
    a.playback = 1;                             /*Make the animation backward too when it's ready*/
    a.playback_pause = 0;                       /*Wait before playback*/
    a.repeat = 1;                               /*Repeat the animation*/
    a.repeat_pause = 500;                       /*Wait before repeat*/
    lv_anim_create(&a);

    /*Create a button to demonstrate the style animations*/
    lv_obj_t * btn3;
    btn3 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_pos(btn3, 10, 150);       /*Set a position. It will be the animation's destination*/
    lv_obj_set_size(btn3, 80, 50);

    label = lv_label_create(btn3, NULL);
    lv_label_set_text(label, "Style");

    /*Create a unique style for the button*/
    lv_style_copy(&btn3_style, lv_btn_get_style(btn3, LV_BTN_STYLE_REL));
    lv_btn_set_style(btn3, LV_BTN_STATE_REL, &btn3_style);

    /*Animate the new style*/
    lv_style_anim_t sa;
    sa.style_anim = &btn3_style;            /*This style will be animated*/
    sa.style_start = &lv_style_btn_rel;     /*Style in the beginning (can be 'style_anim' as well)*/
    sa.style_end = &lv_style_pretty;        /*Style at the and (can be 'style_anim' as well)*/
    sa.act_time = -500;                     /*These parameters are the same as with the normal animation*/
    sa.time = 1000;
    sa.playback = 1;
    sa.playback_pause = 500;
    sa.repeat = 1;
    sa.repeat_pause = 500;
    sa.end_cb = NULL;
    lv_style_anim_create(&sa);
}


void lvgl_text_show()
{
    /*Create a Label on the currently active screen*/
    lv_obj_t * label1 =  lv_label_create(lv_scr_act(), NULL);

    /*Modify the Label's text*/
    lv_label_set_text(label1, "Hello LCD !\n"SYMBOL_AUDIO);

    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
}


void Display_Task(void* param)
{
    while(!lvgl_init_flag)
        OS_Sleep(200);
    
    Trace(3,"start display");
    // lvgl_text_show();
    lv_tutorial_animations();

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
void lvgl2_Main(void)
{
    mainTaskHandle = OS_CreateTask(AppMainTask ,
        NULL, NULL, AppMain_TASK_STACK_SIZE, AppMain_TASK_PRIORITY, 0, 0, "init Task");
    OS_SetUserMainHandle(&mainTaskHandle);
}

static uint16_t* flush_pixels = NULL;
void OnBlit(void* param)
{
    Trace(3,"blit ok");
    OS_Free(flush_pixels);
    flush_pixels = NULL;
    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_flush_ready();
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
    Trace(3,"ex_disp_flush %d,%d %d,%d",x1,y1,x2,y2);

    uint32_t S = (x2-x1+1)*(y2-y1+1);
    if(flush_pixels)
    {
        Trace(2,"not flush complete yet!!!should wait for complete here");
        OS_Free(flush_pixels);
        flush_pixels = NULL;
    }
    flush_pixels = (uint16_t*)OS_Malloc(S*sizeof(uint16_t));
    if(!flush_pixels)
    {
        Trace(3,"malloc");
        return;
    }
    // get all pixels to a buffer
    uint32_t i = 0;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            flush_pixels[i++] = color_p->full;
            color_p++;
        }
    }
    //flush to LCD
    LCD_FBW_t window;
    window.fb.buffer = flush_pixels;
    // window.fb.buffer = (uint16_t*)color_p;
    window.fb.width = x2-x1+1;
    window.fb.height = y2-y1+1;
    window.fb.colorFormat = LCD_COLOR_FORMAT_RGB_565;
    window.roi.x = 0;
    window.roi.y = 0;
    window.roi.width = window.fb.width;
    window.roi.height = window.fb.height;
    lcd.Blit16(&window,x1,y1);
    // OS_Free(pixels);
    // /* IMPORTANT!!!
    //  * Inform the graphics library that you are ready with the flushing*/
    // lv_flush_ready();
}


/* Write a pixel array (called 'map') to the a specific area on the display
 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h*/
static void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    Trace(3,"ex_disp_fill %d,%d %d,%d",x1,y1,x2,y2);

    LCD_ROI_t roi = {
        .x = x1,
        .y = y1,
        .width = x2-x1+1,
        .height = y2-y1+1 
    };

    lcd.FillRect16(&roi,color.full);
}

/* Write a pixel array (called 'map') to the a specific area on the display
 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h*/
static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    
    Trace(3,"ex_disp_map %d,%d %d,%d",x1,y1,x2,y2);
    uint32_t S = (x2-x1+1)*(y2-y1+1);
    if(flush_pixels)
    {
        Trace(2,"not flush complete yet!!!should wait for complete here");
        OS_Free(flush_pixels);
        flush_pixels = NULL;
    }
    flush_pixels = (uint16_t*)OS_Malloc(S*sizeof(uint16_t));
    if(!flush_pixels)
    {
        Trace(3,"malloc");
        return;
    }
    //get all pixels to a buffer
    uint32_t i = 0;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            flush_pixels[i++] = color_p->full;
            color_p++;
        }
    }
    //flush to LCD
    LCD_FBW_t window;
    window.fb.buffer = flush_pixels;
    // window.fb.buffer = (uint16_t*)(color_p);
    window.fb.width = x2-x1+1;
    window.fb.height = y2-y1+1;
    window.fb.colorFormat = LCD_COLOR_FORMAT_RGB_565;
    window.roi.x = 0;
    window.roi.y = 0;
    window.roi.width = window.fb.width;
    window.roi.height = window.fb.height;
    lcd.Blit16(&window,x1,y1);
    // OS_Free(pixels);
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

