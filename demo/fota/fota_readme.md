# FOTA使用说明
## 1.整体流程
* 保留旧固件包
* 生成新的固件包
* 使用脚本生成ota包,old参数是旧包所在位置，new参数是新包所在位置,最后生成两个用于上传的ota包
```
python platform\compilation\lodCombine.py --opt otapack --old fota\fota_B1508_debug-fold.lod --new fota\fota_B1508_debug-fnew.lod
```
* 把两个ota包按照相应的位置上传，选择旧包位置，选择新包位置，点击create
```
![](./doc/assets/fota.png)
```
* 保留生成的pack，可以先通过uart进行本地升级测试，再进行fota测试，具体参考代码GPRS_C_SDK\demo\fota\fota_uart.c
* 把生成的pack包放到http文件服务器上
* 修改GPRS_C_SDK\demo\fota\fota_server.c代码中http的url，实现fota
```
        case NETWORK_STATUS_ACTIVATED:
        {
            char url[256];
            memset(url, 0, sizeof(url));
            sprintf(url, FOTA_HTTP_SERVER, SOFT_VERSION);
            Trace(1,"fota network activate success url %s", url);
            if(API_FotaByServer(url, processFotaUpgradeData) == 0)
                Trace(1,"fota wait");
            else
                Trace(1,"fota fail");
            break;
        }
```
## 2.注意事项
* fota使用的是差分包升级,制作的差分包必须要与升级前后的固件为基础,否则升级失败
* 不同版本之间的差分包不同，比如最新版本是D，那么就要用A2D升级原来的A固件,用B2D升级原来的B固件,用C2D升级原来的C固件。