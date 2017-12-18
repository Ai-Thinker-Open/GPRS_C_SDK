[GPRS ​​C SDK Development Environment Setup] (https://github.com/Ai-Thinker-Open/GPRS-C-SDK/blob/master/doc/compile_environment_en.md)
====

## 1. Download tool chain and download debugging tools and SDK

* [Download CSDTK3.8] (http://otge8q9ud.bkt.clouddn.com/CSDTK.7z)

* [Download SDK] (https://github.com/Ai-Thinker-Open/GPRS_C_SDK/releases), then unzip the `GPRS_C_SDK` folder into a directory

** There is a video recorded here, which can follow the video: [Click to view video] (https://www.bilibili.com/video/av16579395/) **

## 2. Decompression CSDTK file (compile link to download debugging tools)

Extract to a folder, such as `C: \ CSDTK`

## 4. Set the variable

* Edit `CSDTK / cygwin / .bashrc` file
  * Modify `PROJECT_PATH = 'C: \ projects'`, where `C: \ projects` is the project directory, modify your own project directory
  ```
  C:__
      |___ projects
                   |__ GPRS_C_SDK
                                |__ app
                                |__ demo
                                |__ doc
                                |__ include
                                |__ init
                                |__ platform
  ```
  * Change the cooltools directory in `COOLTOOLS_PATH` (eg` / cygdrive / C / CSDTK / cooltools`). Note that you need to use the slash `/` and start with `/ cygdrive`, because cygwin defaults to mounting the windows' / cygdrive` directory)
  * Save closed, re-open cygwin to take effect
* Edit `Cygwin.bat` file, modify
`` `
@echo off
D:
set CYGWIN_PATH = C: \ CSDTK \ cygwin
`` `
For your `cygwin` directory under` CSDTK`

* Modify the file permissions (modify it once):
Open `CSDTK / cygwin / Cygwin.bat` as an administrator and run` chmod 777 -R / cygdrive / c / projects / GPRS_C_SDK`, where the directory is replaced with your project directory.


## 5. Compile

** ** Compilation **
After the environment is set up, try to open cygwin, type it
```
work gprs_c_sdk
./build.sh demo gpip
```
Compile, compile will generate a `build` directory, the compiler will be completed in the` hex` directory to generate two format `lod` file, which is the target file we used to download to the development board

* ** Target Document Description **

The two files are big and small, and you need to download the large file (`* _B * .lod`) when you first download. The second and later only need to download the small file (` * _flash.lod`) Yes, updating the SDK also requires re-downloading large files

## 6. Other issues

If still wrong, please check the above steps according to the error prompt, and then no, you can ask the forum or github submitted an issue

* Way one: github [add issue] (https://github.com/Ai-Thinker-Open/GPRS-C-SDK/issues/new)

* Way 2: [Shun peace forum discussion] (http://bbs.ai-thinker.com/forum.php?mod=forumdisplay&fid=37)
