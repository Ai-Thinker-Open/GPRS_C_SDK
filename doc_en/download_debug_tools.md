[GPRS ​​download debug instructions]
======


## hardware connection

* Module Debugging The download interface is the `HST` interface. The pins` HST_RX` and `HST_TX` are respectively connected to the` TX` and `RX` pins of the USB to serial module (cross connection,` HST_RX` then `TX `)

* The power can be connected to the lithium battery voltage (ie 3.8v ~ 4.2v) (VBAT pin), can also be connected to 5v power supply (5v input pin or USB input), it should be noted that, if using lithium battery power supply You need to press <kbd> POWER KEY </kbd> for about 3 seconds to power on the module, USB power will automatically power on the power. In the download or debugging to ensure that has been switched on.

## Download steps
Update `coolwatcher to the latest version`
* Open `CSDTK / cooltools / coolwatcher.exe`

* Select the 8955 for the options on the left and the `lastcomport` for the right to download the port for debugging the serial port (` HST`) for the connection module. </br>
![coolwatcher startup options](./assets/coolwatcher_open.png) </br>
Click Ok

* Wait for the connection to succeed, the following screen appears </br>
![coolwatcher main interface and download button](./assets/coolwatcher_download.png) </br>
> If there is a connection failure, please refer to [coolwatcher can not connect reason] (# coolwatcher can not connect) part

* Click the <kbd> lod </kbd> icon in the image below to select the firmware you want to download

* If you are using it for the first time, you need to click the <kbd> DRY </kbd> icon and select the file whose name contains `8955 ramrun spi32m .lod` (under project` platform / ramrun` or coolwatcher` cooltools \ chipgen \ Modem2G \ toolpool \ plugins \ fastpf \ flash_programmers` directory).
Only need to set once, will remember later

* Click the <kbd> download </kbd> icon to download, the normal effect as shown below, the bottom right corner of the Command column will have a progress mark </br>
![coolwatcher Downloading](./assets/coolwatcher_downloading.png) </br>
> If the download failed, please refer to [coolwatcher download failed reason] (# coolwatcher can not download) part


## coolwatcher can not connect

![Unable to connect module](./assets/coolwatcher_connect_fail.png)

Solution:

* Ensure that the serial port and the module are connected (note that the USB port on the development board is not a serial port, and the serial port is connected to the `HST_RX` and` HST_TX` pins through the USB to serial port module)

* Ensure that the serial port is not occupied by other software

* Check the serial number is wrong, set the serial number when restarting, or do not restart click the lower right corner <kbd> CoolHost </kbd> icon to set </br>
![CoolHost settings](./assets/coolwatcher_connect_settings.png)

Then click <kbd> connect </kbd> to reconnect the serial port

## coolwatcher can not download

![Can not download](./assets/coolwatcher_download_fail.png)

*[ramrun file error](https://github.com/Ai-Thinker-Open/GPRS_C_SDK/issues/1), try using project directory `platform / ramrun` and coolwatcher` CSDTK \ cooltools \ chipgen \ Modem2G \ toolpool \ plugins \ fastpf \ flash_programmers` directory `* _8955 _ * _ spi32m_ramrun.lod`

* It is possible that the program has entered hibernation mode and is reset by the module reset button before downloading

* Serial port may be lost, the lower right corner of the taskbar double-click the <Cool> <Cool> </kbd> icon to reconnect the serial port, and then download

* Serial port other problems, unplug the USB to serial port module re-insert the computer, and then re-connect the serial port

* Boot into the program into the sleep mode or thread using the `while (1)` dead loop and completely take up resources, such as:
```
void LoopTask (VOID * pData)
{
    uint64_t count = 0;
    while (1)
    {
        ++ count;
        if (count == 3000)
        {
            count = 0;
            Trace (1, "Test Test");
        }
    }
}
```
**Solution:** Press the module reset button, the USB transfer serial port module data transmission indicator flashing (that is, the module has been started and send data to the serial port) press the download button for a second, not to try on how many times
In order to completely prevent this problem, it is recommended not to write such a program, it is best to use the time in the loop trigger or delay at least `OS_Sleep ()`)

```
void LoopTask (VOID * pData)
{
    uint64_t count = 0;
    while (1)
    {
        ++ count;
        if (count == 3000)
        {
            count = 0;
            Trace (1, "Test Test");
            OS_Sleep (100);
        }
    }
}
```
or
```
void LoopTask (VOID * pData)
{
    uint64_t count = 0;
    while (1)
    {
        ++ count;
        if (count == 3000)
        {
            count = 0;
            Trace (1, "Test Test");
            
        }
    }
    OS_Sleep (1);
}
```
Or event handling
```
void AppMainTask (VOID * pData)
{
    API_Event_t * event = NULL;
        
    while (1)
    {
        if (OS_WaitEvent (mainTaskHandle, & event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch (event);
            OS_Free (event-> pParam1);
            OS_Free (event);
        }
    }
}
```


## debug

#### Tracer basic introduction and use



* View debug information: Open `Plugins`-> `Active Tracer`, the interface and function as shown below

![tracer](./assets/coolwatcher_trace.png)

Need to set up Tracer before use, set as follows 1 ~ 6 steps:

![Tracer Settings](./assets/coolwatcher_trace_settings.png)

* The program needs to output the debugging information You can call `Trace (uint16_t nIndex, PCSTR fmt, ...)` in `api_debug.h`, where` nIndex` corresponds to `MMI 01` ~` MMI 16 `

#### Tracer use easy to encounter problems

* Sometimes I expect the debug data is not displayed, it may be the upper right corner of the pause to display the latest button, forgot to turn off! !


#### Crash handling

Here simulated an exception, null pointer assignment exception, the general emergence of such problems are the problem of indiscriminate flight!
Open GBD, and set the select elf file

![Pointer exception](./assets/coolwatcher_gdb_launch.png)

![Select elf file](./assets/coolwatcher_gdb_settings.png)

Open the GBD terminal and enter `bt` or` bt f` to view the message

![Abnormal information](./assets/coolwatcher_gdb_error_info.png)

It should be noted that this GDB is not in the manual does not support the function of the break point, break point need to write breakpoint statements in the code Caixing, so only to see the crash information on the line, and then use the crash it! !
