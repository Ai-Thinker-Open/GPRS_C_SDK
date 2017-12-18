[GPRS ​​and GPRS C SDK from scratch] (https://github.com/Ai-Thinker-Open/GPRS-C-SDK/blob/master/doc/gprs_start_from_scratch_en.md)
===========


Before you begin, make sure that the development environment and download software are installed and working properly,
Reference [GPRS ​​C SDK development environment to build] (./ compile_environment_zh-cn.md)
And [GPRS ​​download debug instructions] (./ download_debug_tool_zh-cn.md)

## (A) SDK directory structure

| Contents | Description |
| ---     | --- |
|app      |home directory, application code here
|build    | compiled directory, intermediate file |
|demo     | some routines |
| doc     | Some SDK-related documents, Markdown format, can be read online at github, can also be opened using the relevant software to read, the official website Wiki archive will provide `html` format document, you can open the browser directly to read
| hex     | last generated burnable file |
| include | SDK file directory |
| init    | system initialization directory, you can ignore, do not recommend changes |

## (B) routine

There are various routines in the `demo` directory, starting with a simple GPIO

### GPIO pin used

* **Compile a look at the phenomenon of**: `./build.sh demo gpio`, and then downloaded to the development board, the first time burn the large file in ` hex` `** _ B **. Lod`, the second time burn the small file` ** _ flash.lod`.
After programming, the LED of the development board will blink

* **Code**: The amount of demo code is very small, as follows:
```
#include <api_os.h>
#include <api_hal_gpio.h>
#include "demo_gpio.h"

#define GPIO_PIN_LED_BLUE GPIO_PIN27

#define GPIO_Main_TASK_STACK_SIZE (1024 * 2)
#define GPIO_Main_TASK_PRIORITY 1
HANDLE gpioTaskHandle = NULL;

void Gpio_MainTask (VOID * pData)
{
    static GPIO_LEVEL ledBlueLevel = GPIO_LEVEL_LOW;
    GPIO_config_t gpioLedBlue = {
        .mode = GPIO_MODE_OUTPUT,
        .pin = GPIO_PIN_LED_BLUE
    };

    GPIO_Init (gpioLedBlue);

    while (1)
    {
        ledBlueLevel = (ledBlueLevel == GPIO_LEVEL_HIGH)? GPIO_LEVEL_LOW: GPIO_LEVEL_HIGH;
        Trace (1, "gpio_main toggle:% d", ledBlueLevel);
        GPIO_SetLevel (gpioLedBlue, ledBlueLevel); // Set level
        OS_Sleep (1000); // Sleep 1 s
    }
}
void gpio_Main (void)
{
    gpioTaskHandle = OS_CreateTask (Gpio_MainTask,
        NULL, NULL, GPIO_Main_TASK_STACK_SIZE, GPIO_Main_TASK_PRIORITY, 0, 0, "main Task");
    OS_SetUserMainHandle (& gpioTaskHandle);
}
```

* **Entry Function**: The entry function is `gpio_Main`,
  * Which gpio_Main is an entry function, the name must be `demo folder name _Main ()`
  In `gpio_Main ()` must create a main task, not used too much thread and operating system does not matter, here is the simplest function, `OS_CreateTask` create a task, the first parameter is a function of the task Name, `GPIO_Main_TASK_STACK_SIZE` and` GPIO_Main_TASK_PRIORITY` are the task stack size and task priority (the lower the value the higher the priority)
  * Then `OS_SetUserMainHandle`, this function must be called once, it is very important. It is mainly to tell the SDK application which is the main task, so that the underlying SDK to know who sent the event (for example, SDK found no card, you need to send the API_EVENT_ID_NO_SIMCARD event to the main task

* **GPIO Usage**: GPO was created in the main task after the main task was created. The task function created here is: `Gpio_MainTask ()`
  * Define a structure `GPIO_config_t gpioLedBlue`, set the pin and mode
    ```
    gpioLedBlue mode = GPIO_MODE_OUTPUT; // Output mode
    gpioLedBlue pin = GPIO_PIN_LED_BLUE; // Pin is GPIO27 pin
    ```
  * Then initialize GPIO using the `GPIO_Init ()` function
  * Then you can call `GPIO_SetLevel ()` in `while (1) {}` to set the GPIO output level
  * It should be noted that in the current version of the SDK, it is best to add an `OS_Sleep (1)` in while (1), otherwise it may be troublesome to download the program! How to solve this problem in the download and debugging documents said

Of course, there are other GPIO functions, such as as input, as an interrupt pin, is commented in the routine, you can try it yourself



### minimum system, operating system (OS) use

Compared to GPIO, the main task of the OS routine is no longer directly in the `while (1) {} ` operation, but in the main task to listen for events from the SDK, the definition of the event and the meaning of the parameters in` api_event.h` as defined and explained. There is something else to do to create one more task

* **The most basic structure of the application**

The main task should be written as follows:
```
void EventDispatch (API_Event_t * pEvent);

void socket_MainTask (void * pData)
{
    API_Event_t * event = NULL;

    while (1)
    {
        if (OS_WaitEvent (socketTaskHandle, & event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch (event);
            OS_Free (event-> pParam1);
            OS_Free (event);
        }
    }
}
```
Which `OS_WaitEvent` is blocking waiting for the arrival of the event, does not support non-blocking temporarily (the third parameter can only be `OS_TIME_OUT_WAIT_FOREVER` )

Then in the EventDispatch function to handle the event, release the memory after processing, OS_Free will automatically determine whether the law

```
void EventDispatch (API_Event_t * pEvent)
{
    switch (pEvent-> id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace (10, "!! NO SIM CARD% d !!!!", pEvent-> param1);
            break;

        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            Trace (2, "network register success");
            break;
            
        default:
            break;
    }
}
```

Here mainly dealt with two events,
One is not read the SIM card event, `param1` parameter represents the card number, there is a description in` api_event.h`
The other is the successful registration of network events, respectively, are successfully registered to the local network and roaming network, registration will be successful after receiving this event, and print debugging information, debug information need to use coolwatcher tracer function to see how to debug see the download debug document

* Other OS features
Including sending events, semaphores, etc. can see the header file


### socket connection server

To use socket, need to do the following steps:

* 1. **Register to base station network**
    Open automatically registered, the registration will be completed parameter `API_EVENT_ID_NETWORK_REGISTERED_HOME` or` API_EVENT_ID_NETWORK_REGISTERED_ROAMING` event

* 2. **Attached to the base station network**
    After successful registration, use `Network_StartAttach ()` to attach, trigger `API_EVENT_ID_NETWORK_ATTACHED` after successful attachment

* 3. **Activate GPRS Context**
    After the attachment is successful, use the following statement for context activation
    ```
    Network_PDP_Context_t context = {
                .apn = "cmnet",
                .userName = "",
                .userPasswd = ""
            };
            Network_StartActive (context);
    ```
    Among them, the context is defined context, set the correct apn and other information, the use of mobile Internet card, just set apn to `cmnet`
    Activation will trigger the `API_EVENT_ID_NETWORK_ACTIVATED` event, this module will have the ability to GPRS networking

* 4. **Create socket connection and communication**

    See routine

    Call socket related API to establish communication:
    ```
    int fd = Socket_TcpipConnect (TCP, SERVER_IP, SERVER_PORT);
    Socket_TcpipWrite (fd, buffer, length);
    length = Socket_TcpipRead (fd, buffer, length);
    ```
    At present, these APIs are event-based APIs, and API_EVENT_ID_SOCKET_CONNECTED will be generated when the connection is established successfully. After the data is received, an API_EVENT_ID_SOCKET_RECEIVED event will be generated, which can be viewed in the socket routine.
    Follow-up version will update the blocking method, easier to use, easy to use in some simple applications

### Please refer to the routine for the use of other modules
