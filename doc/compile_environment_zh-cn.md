[GPRS C SDK 开发环境搭建](https://github.com/Ai-Thinker-Open/GPRS-C-SDK/blob/master/doc/compile_environment_zh-cn.md)
====

## 1. 下载工具链以及下载调试工具及SDK

* [下载CSDTK3.8](https://pan.baidu.com/s/1rYLb_JpJJjn2ZCiblzAsMA)

* [下载SDK](https://github.com/Ai-Thinker-Open/GPRS_C_SDK/releases), 然后解压为`GPRS_C_SDK`文件夹放到某个目录下

**这里录制了一个视频，可以照着视频做:[点击查看视频](https://www.bilibili.com/video/av16579395/)**

## 2. 解压CSDTK文件（编译链接下载调试工具）

解压到某个文件夹,比如`C:\CSDTK`

## 4. 设置变量

* 编辑`CSDTK/cygwin/.bashrc`文件
  * 修改`PROJECT_PATH="C:\projects"`，这里`C:\projects`是放工程的目录，修改成自己的放工程文件夹的目录
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
  * 改`COOLTOOLS_PATH`中的cooltools目录(比如`/cygdrive/C/CSDTK/cooltools`,注意这里需要使用斜杠`/`而且以`/cygdrive`开头，因为cygwin默认把windows的盘挂载在`/cygdrive`目录下)
  * 保存关闭，重新打开cygwin即生效
* 编辑`Cygwin.bat`文件，修改
```
@echo off
D:
set CYGWIN_PATH=C:\CSDTK\cygwin
```
为你的`CSDTK`下的`cygwin`目录

* 修改文件权限（修改一次就可以了）：
用管理员身份打开`CSDTK/cygwin/Cygwin.bat`，运行 `chmod 777 -R /cygdrive/c/projects/GPRS_C_SDK`，这里目录替换成你的工程目录。


## 5. 编译

* **编译**
环境搭建完成后，尝试打开cygwin，输入
```
work gprs_c_sdk
./build.sh demo gpio
```
进行编译，编译会生成一个`build`目录，编译完成会在`hex`目录下生成两个格式为`lod`的文件，这就是我们用来下载到开发板的目标文件

* **目标文件说明** 

两个文件一大一小，在第一次下载时需要下载大文件(`*_B*.lod`)即可，第二次和以后都只需要下载小文件(`*_flash.lod`)即可，更新SDK后也需要重新下载大文件

## 6. 其它问题

如果仍然有错，请根据错误提示仔细检查上面的步奏，再不行，可以到安信可论坛提问或者github提交issue

* 方式一：github[添加issue](https://github.com/Ai-Thinker-Open/GPRS-C-SDK/issues/new)

* 方式二：[安信可论坛讨论](http://bbs.ai-thinker.com/forum.php?mod=forumdisplay&fid=37)
