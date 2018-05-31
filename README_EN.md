[Ai-Thinker GPRS C SDK](https://github.com/Ai-Thinker-Open/GPRS-C-SDK)
=====

Ai-Thinker GPRS onchip development SDK written by C.

[中文版本](./README.md)



## ( A ) Hardware

* **A9 GPRS Module** </br>
![](./doc/assets/A9.png) </br>
**Features**:
  * RDA 32 bit RISC core, frequency up to 312MHz, with 4k instruction cache, 4k data cache
  * Up to 29 GPIOs (with two download pins)
  * Calendar (Real Time Clock) with alarm
  * 1 USB1.1 device interface
  * 2 UART interface with flow control (+1 download/debug serial port)
  * 2 SPI interface
  * 3 I <sup>2</sup> C interface
  * 1 SDMMC controller (interface)
  * 2 ADC interface, 10 bits
  * 32Mb (4MB) SPI NOR Flash
  * 32Mb (4MB) DDR PSRAM
  * 8kHz, 13Bits/sample ADC mic
  * 48kHz, 16bits/sample DAC Audio
  * Power Management Unit: Lithium battery charge management, integrated DC-DC and LDOs, variable IO voltage
  * 18.8 x 19.2 mm SMD package
  * Quad-band GSM/GPRS (800/900/1800 / 1900MHz)
  * calls
  * SMS service

* **A9G GPRS + GPS + BDS Module**</br>
![](./doc/assets/A9G.png) </br>
**Features**:
  * All the features of A9 above
  * Integrated GPS+BDS (connected to UART2 internal of module)

* **A9/A9G GPRS (+GPS+BDS) Development Board( pudding)** </br>
![](./doc/assets/A9G_dev.png) </br>
A9/A9G development board for easy development and debugging
**Features**:
  * 1 A9G module (the board be the same that with A9 and A9G, for the A9 and A9G in the same package, the same pin )
  * 29 GPIOs with 2.45mm spacing (with 2 download debug pins (`HST_TX`,` HST_RX`)
  * One SIM card slot (Nano Card <Micro Card <Standard Card)
  * 1 TF card slot
  * 1 GPRS interface with IPEX 1 generation package
  * 1 GPS interface with IPEX 1 generation package
  * 1 micro USB interface
  * 5v-4.2V DC-DC, it can be 5v power supply or 3.8 ~ 4.2V power supply
  * ~~1 accelerometer chip(LIS3DHx)~~
  * 1 power key, and a reset button
  * 2 LED
  * 1 microphone </br>
![](./doc/assets/pudding_pin.png) </br>


* **USB to Serial Module** </br>
![](./doc/assets/USB-UART.png) </br>
It should be noted that the USB interface on the development board is not a USB serial port function, but USB1.1 function,
Therefore, in order to download or debug, you need at least one USB to serial port module to connect board's `HST_TX` and` HST_RX` pin

* **Lithium Battery** </br>
board can be light up with 5V or 4.2V power, and ensure supply enough current

## ( B ) SDK

### SDK Features

* Provide easy-to-use API, as long as you have `C` language knowledge. 
Integrates a wide range of features including basic GPIO, UART, SPI, IIC, ADC, OS, FS, SMS, CALL and GPRS, socket, DNS, MQTT, SSL/TLS etc.


### Get the SDK

**Attention**: Please **DO NOT** Download using the green <kbd> Clone or download </kbd> button in the top right corner of the page will result in tarballs without `SDK-LIB`

there's two way to download SDK:

* Download the latest released archive from [github release page](https://github.com/Ai-Thinker-Open/GPRS_C_SDK/releases).
  > This release gets the released version of the code

* Or clone directly(need to install `git`), and **must with parameter` --recursive`**, or get incomplete SDK files
```
git clone https://github.com/Ai-Thinker-Open/GPRS_C_SDK.git --recursive
```

### SDK directory structure:

| Contents | Description |
| --- | --- |
|app|  application code here|
build | build files |
demo | examples |
| doc | SDK-related documentation, in `Markdown` format, available online at [github](https://github.com/Ai-Thinker-Open/GPRS_C_SDK/blob/master/README.md)|
| hex | last generated burnable file(burn files) |
| include | SDK api include directory |
| init | system initialization directory, you can ignore, do not recommend edit |




### Develop applications using the SDK

Examples refer to the `demo` folder

**Doumentation** here:  [GPRS C SDK DOC](https://ai-thinker-open.github.io/GPRS_C_SDK_DOC/en)

Include [environment installation](https://ai-thinker-open.github.io/GPRS_C_SDK_DOC/en/c-sdk/installation.html) or [burn & debug](https://ai-thinker-open.github.io/GPRS_C_SDK_DOC/en/c-sdk/burn-debug.html) docs etc.


## ( E ) Feedback

* [creat issue on github](https://github.com/Ai-Thinker-Open/GPRS-C-SDK/issues/new)

* Or [discuss on forum ](http://bbs.ai-thinker.com/forum.php?mod=forumdisplay&fid=37)

* Click ★ <kbd> Star </kbd> on the right conner of this page if this repo helps you



## ( F ) Participate in the development

fork -> modify -> submit PR
