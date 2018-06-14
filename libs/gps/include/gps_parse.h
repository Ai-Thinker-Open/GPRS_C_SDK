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
#include "minmea.h"

///////////////////////////////////////////////////////////////
///////////////////////configuration///////////////////////////
#define GPS_PARSE_MAX_GSA_NUMBER 2
#define GPS_PARSE_MAX_GSV_NUMBER 20
////////////////////configuration end//////////////////////////

typedef struct{
	struct minmea_sentence_rmc rmc;
	struct minmea_sentence_gsa gsa[GPS_PARSE_MAX_GSA_NUMBER];
	struct minmea_sentence_gga gga;
	struct minmea_sentence_gll gll;
	struct minmea_sentence_gst gst;
	struct minmea_sentence_gsv gsv[GPS_PARSE_MAX_GSV_NUMBER];
	struct minmea_sentence_vtg vtg;
	struct minmea_sentence_zda zda;
}GPS_Info_t;


/**
 * Get address of global gps infomatioin variable 
 * @return GPS_Info_t*: Address of global gps infomatioin variable 
 */
GPS_Info_t* Gps_GetInfo();

/**
 * Parse a full frame gps NMEA message.
 * @param nmeas: A full GPA NMEA message frame. e.g.
 *                $GNGGA,084257.000,2234.7758,N,11354.9654,E,2,12,1.00,59.4,M,-2.8,M,,*56
 *                $GPGSA,A,3,19,28,09,03,23,193,,,,,,,1.28,1.00,0.80*32
 *                $BDGSA,A,3,04,01,07,03,06,09,,,,,,,1.28,1.00,0.80*1F
 *                $GPGSV,4,1,14,193,60,100,40,17,54,020,14,28,53,165,42,06,52,308,*43
 *                $GPGSV,4,2,14,19,46,346,13,42,46,122,33,02,23,268,,03,21,041,18*75
 *                $GPGSV,4,3,14,09,17,125,32,23,13,088,35,30,04,180,34,05,02,211,23*7B
 *                $GPGSV,4,4,14,24,01,292,,12,01,325,*74
 *                $BDGSV,3,1,12,03,65,189,37,10,55,226,,01,51,128,35,08,49,000,*67
 *                $BDGSV,3,2,12,13,49,322,,02,48,238,,17,44,136,,07,40,185,40*68
 *                $BDGSV,3,3,12,04,33,110,33,06,27,160,36,05,24,256,,09,12,183,34*6B
 *                $GNRMC,084257.000,A,2234.7758,N,11354.9654,E,0.032,306.43,140618,,,D*46
 *                $GNVTG,306.43,T,,M,0.032,N,0.059,K,D*29
 * @return bool: Parse success of not
 */
bool GPS_Parse(uint8_t* nmeas);

#ifdef __cplusplus
}
#endif

#endif

