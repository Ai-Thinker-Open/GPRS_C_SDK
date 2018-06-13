/*
 * @File  gps_parse.h
 * @Brief 
 * 
 * @Author: Neucrack 
 * @Date: 2017-12-13 10:57:17 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2018-06-13 16:08:07
 */

#ifndef __GPS_PARSE_H
#define __GPS_PARSE_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"
#include "stdbool.h"

typedef enum{
    GPS_FIX_NO = 1  ,  //no fix
    GPS_FIX_2D      ,
    GPS_FIX_3D      ,
    GPS_FIX_MAX
}GPS_Fix_t;


//TODO: optimize, not elegent
typedef struct _GPS_Information{
	unsigned char Real_Locate;  	    //Real Locate Valid
	unsigned char Located_Status;       //Locate Valid 有效定位，无效定位
	unsigned char fixModeGPS;           //Locate Mode A=自动手动2D/3D，M=手动2D/3D
    unsigned char fixModeBDS;           //Locate Mode A=自动手动2D/3D，M=手动2D/3D
	GPS_Fix_t fixGPS;	          	    //1=未定位，2=2D定位，3=3D定位
    GPS_Fix_t fixBDS;                   //BDS fix status
	char UTC_Time[7];                   //UTC TIme  hhmmss.sss(时分秒.毫秒)格式
	char UTC_Date[7];                   //UTC Date
	char latitude[10];                  //Latitude  ddmm.mmmm(度分)格式(前面的0也将被传输)
	char NS_Indicator;                  //NS
	char longitude[11];                 //Longtitude
	char EW_Indicator;                  //EW
	double Knot_Speed;                  //Knot_Speed 地面速率(000.0~999.9节，前面的0也将被传输)、
	double Speed;						//地面速率  (0000.0~1851.8公里/小时，前面的0也将被传输)
	double Course;                      //Course  以真北为参考基准，沿顺时针方向至航向的角度。
	double Magnetic_Course;				//Magnetic Course 以磁北为参考基准，沿顺时针方向至航向的角度。
	
	double PDOP;                        //PDOP综合位置精度因子（0.5 - 99.9）
	double HDOP;                        //HDOP水平精度因子（0.5 - 99.9）
	double VDOP;                        //VDOP垂直精度因子（0.5 - 99.9）
	
	double MSL_Altitude;                //High 海平面高度
	double Geoid_Separation;            //Geoid Separation大地水准面分离
	unsigned char Use_EPH_Sum;       	//Use_EPH_Sum
	unsigned char User_EPH[12];         //User_EPH   contain:Satellite Used1
	unsigned short EPH_State[12][4]; 	//EPH_State  contain:Satellite ID , Elevation , Azimuth , SNR (C/No)
}GPS_Information_t;


bool GPS_ParseOneNMEA(uint8_t* oneNmeaData);
GPS_Information_t* Gps_GetInfo();
bool GPS_Parse(uint8_t* nmeas);

#ifdef __cplusplus
}
#endif

#endif

