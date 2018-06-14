/*
 * @File  gps_parse.c
 * @Brief 
 * 
 * @Author: Neucrack 
 * @Date: 2017-12-13 11:00:11 
 * @Last Modified by: Neucrack
 * @Last Modified time: 2018-06-13 18:54:36
 */

#include "gps_parse.h"
#include "api_os.h"
#include "string.h"
#include "stdlib.h"
#include "api_debug.h"
#include "minmea.h"
#include "stdint.h"
#include "stdbool.h"

#define GPS_MALLOC OS_Malloc
#define GPS_FREE   OS_Free

static int is_digit(char ch)  
{  
    if(ch>='0'&&ch<='9')  
        return 1;  
    else  
        return 0;  
}  
static int is_space(char ch)  
{  
    if(ch==' ')  
        return 1;  
    else  
        return 0;  
} 

static double GpsAtof(char *s)  
{  
    double power,value;  
    int i,sign;  
    if(s == NULL)
        return 0;
    for(i=0;is_space(s[i]);i++);//除去字符串前的空格  
    sign = (true)?-1:1;
    if(s[i]=='-'||s[i]=='+')//要是有符号位就前进一位  
        i++;  
    for(value=0.0;is_digit(s[i]);i++)//计算小数点钱的数字  
        value=value*10.0+(s[i]-'0');  
    if(s[i]=='.')  
        i++;  
    for(power=1.0;is_digit(s[i]);i++)//计算小数点后的数字  
    {  
        value=value*10.0+(s[i]-'0');  
        power*=10.0;  
    }  
    return sign*value/power;  
} 



GPS_Information_t gpsInfo__;
static uint8_t commaIndexArray[32];

/**
  *@brief  save every comma(',') and replace ',' with '\0'
  *@param  Svae DH_id_sep[]
  *@return void
  */
static void CommaIndex( char* buffer ){
    
    unsigned short i, len;
    unsigned char commaCount;

    memset( commaIndexArray, 0, sizeof( commaIndexArray ) );
    len = strlen( buffer );
    for ( i = 0, commaCount = 0; i < len; i++ ){
        if ( buffer[i] == ',' ){
            commaIndexArray[commaCount] = i;
            commaCount++;
            buffer[i] = '\0';
        }
    }
}

/**	
  *@brief  Return content after comma index 
  *@param  (1~n)
  *@return address after the `commaindex`
  */
static uint8_t* GetDataIndexOfComma( uint8_t* buffer, uint8_t commaIndex ){
    if (commaIndex < 1 )
        return  &buffer[0];
    return  &buffer[ commaIndexArray[commaIndex - 1] + 1];
}


bool GPS_ParseOneNMEA(uint8_t* oneNmeaData)
{
    char* temp;
    unsigned char i, j, msgID;
    bool ret = false;
    uint8_t* nmeaData = GPS_MALLOC(strlen(oneNmeaData)+1);
    if(!nmeaData)
    {
        return false;
    }
    memset(nmeaData,0,strlen(oneNmeaData));
    strcpy(nmeaData,oneNmeaData);
    // Trace(1,"%s",nmeaData);
    CommaIndex(nmeaData);
    if ( strstr(nmeaData, "GGA" ) ){//$GPGGA,112118.000,3743.5044,N,11540.5393,E,1,06,1.6,15.3,M,-9.1,M,,0000*7E
        //************************UTC_Time***********************//
        temp = GetDataIndexOfComma(nmeaData, 1 );
        if ( *temp != 0 )
            memcpy( gpsInfo__.UTC_Time, temp, 6 );
        //****************latitude***********************//
        temp = GetDataIndexOfComma(nmeaData, 2 );
        if ( ( *temp >= 0x31 ) && ( *temp <= 0x39 ) ){
            memcpy( gpsInfo__.latitude, temp, 9 );
            gpsInfo__.latitude[9] = 0;
        } else {
            gpsInfo__.latitude[0] = '0';
            gpsInfo__.latitude[1] = 0;
        }
        //*****************N/S Indicator*********************************//
        gpsInfo__.NS_Indicator = *( GetDataIndexOfComma(nmeaData, 3 ) );
        //******************longitude************************************//
        temp = GetDataIndexOfComma(nmeaData, 4 );
        if ( ( *temp >= 0x30 ) && ( *temp <= 0x39 ) ){
            memcpy( gpsInfo__.longitude, temp, 10 );
            gpsInfo__.longitude[10] = 0;
        } else {
            gpsInfo__.longitude[0] = '0';
            gpsInfo__.longitude[1] = 0;
        }
        //*****************E/W Indicator********************************************//
        gpsInfo__.EW_Indicator = *( GetDataIndexOfComma(nmeaData, 5 ) );
        gpsInfo__.Use_EPH_Sum = GpsAtof( GetDataIndexOfComma(nmeaData, 7 ) );
        gpsInfo__.HDOP = GpsAtof( GetDataIndexOfComma(nmeaData, 8 ) );
        gpsInfo__.MSL_Altitude = GpsAtof( GetDataIndexOfComma(nmeaData, 9 ) );      //海平面高度 
        gpsInfo__.Geoid_Separation = GpsAtof( GetDataIndexOfComma(nmeaData, 11 ) ); //Geoid Separation 大地水准面分离
        ret = true;
    }
    else if ( strstr(nmeaData, "GPGSA" ) ) { //$GPGSA,A,3,28,17,11,09,08,07,,,,,,,3.4,1.6,3.0*3B
        gpsInfo__.fixModeGPS = * GetDataIndexOfComma(nmeaData, 1 ); 
        temp = GetDataIndexOfComma(nmeaData, 2 ); 
        gpsInfo__.fixGPS = *temp - '0';
        for ( i = 0; i < 12; i++ ){
            gpsInfo__.User_EPH[i] = GpsAtof( GetDataIndexOfComma(nmeaData, i + 3 ) ); 
        }
        gpsInfo__.PDOP = GpsAtof( GetDataIndexOfComma(nmeaData, 15 ) );
        gpsInfo__.HDOP = GpsAtof( GetDataIndexOfComma(nmeaData, 16 ) );
        gpsInfo__.VDOP = GpsAtof( GetDataIndexOfComma(nmeaData, 17 ) );
        ret = true;
    }
    else if ( strstr(nmeaData, "BDGSA" ) ) { //$GPGSA,A,3,28,17,11,09,08,07,,,,,,,3.4,1.6,3.0*3B
        gpsInfo__.fixModeBDS = * GetDataIndexOfComma(nmeaData, 1 ); 
        temp = GetDataIndexOfComma(nmeaData, 2 ); 
        gpsInfo__.fixBDS = *temp - '0';
        for ( i = 0; i < 12; i++ ){
            gpsInfo__.User_EPH[i] = GpsAtof( GetDataIndexOfComma(nmeaData, i + 3 ) ); 
        }
        gpsInfo__.PDOP = GpsAtof( GetDataIndexOfComma(nmeaData, 15 ) );
        gpsInfo__.HDOP = GpsAtof( GetDataIndexOfComma(nmeaData, 16 ) );
        gpsInfo__.VDOP = GpsAtof( GetDataIndexOfComma(nmeaData, 17 ) );
        ret = true;
    }
    else if ( strstr(nmeaData, "RMC" ) ){//$GPRMC,112118.000,A,3743.5044,N,11540.5393,E,0.25,198.81,130613,,,A*67
                                         //$GNRMC,000202.029,V,2236.3455,N,11350.3942,E,0.000,0.00,060180,,,N*54
        //*************************************UTC_Time********************************//
        temp = GetDataIndexOfComma(nmeaData, 1 );
        if ( *temp != 0 )
            memcpy( gpsInfo__.UTC_Time, temp, 6 );
        //***********************************Status**************///
        if ( *( GetDataIndexOfComma(nmeaData, 2 ) ) == 'A' ) {
            gpsInfo__.Real_Locate = TRUE;
            gpsInfo__.Located_Status = TRUE;
        } else {
            gpsInfo__.Real_Locate = FALSE;
        }
        //**********************************latitude*************************//
        temp = GetDataIndexOfComma(nmeaData, 3 );
        if ( ( *temp >= 0x31 ) && ( *temp <= 0x39 ) ){
            memcpy( gpsInfo__.latitude, temp, 9 );
            gpsInfo__.latitude[9] = 0;
        } else {
            gpsInfo__.latitude[0] = '0';
            gpsInfo__.latitude[1] = 0;
        }
        gpsInfo__.NS_Indicator = *( GetDataIndexOfComma(nmeaData, 4 ) );
        //********************************longitude********************//
        temp = GetDataIndexOfComma(nmeaData, 5 );
        if ( ( *temp >= 0x30 ) && ( *temp <= 0x39 ) ){
            memcpy( gpsInfo__.longitude, temp, 10 );
            gpsInfo__.longitude[10] = 0;
        } else {
            gpsInfo__.longitude[0] = '0';
            gpsInfo__.longitude[1] = 0;
        }
        gpsInfo__.EW_Indicator = *( GetDataIndexOfComma(nmeaData, 6 ) );
        gpsInfo__.Speed = GpsAtof( GetDataIndexOfComma(nmeaData, 7 ) );
        gpsInfo__.Course = GpsAtof( GetDataIndexOfComma(nmeaData, 8 ) );
        //**************************UTC_Date***************//
        temp = GetDataIndexOfComma(nmeaData, 9 );
        if ( *temp != 0 ){
            memcpy( gpsInfo__.UTC_Date, temp, 6 );
        }
        //Mode A=Autonomous, D=DGPS, E=DR ( GetDataIndexOfComma(nmeaData, 6 ) );              //Only apply to NMEA version 2.3 (and later) in this NMEA message description.
        ret = true;
    }
    else if ( strstr(nmeaData, "GSV" ) ){//$GPGSV,3,1,11,28,73,321,32,17,39,289,43,11,38,053,17,09,37,250,41*78
        // 1,Number of Messages 2 , Message Number
        msgID = atoi( GetDataIndexOfComma(nmeaData, 2 ) );
        if ( ( msgID <= 3 ) && ( msgID != 0 ) ){
            for ( i = ( msgID - 1 ) * 4, j = 4; i < msgID * 4; i++ ){
                gpsInfo__.EPH_State[i][0] = GpsAtof( GetDataIndexOfComma(nmeaData, j++ ) );//Satellite ID
                gpsInfo__.EPH_State[i][1] = GpsAtof( GetDataIndexOfComma(nmeaData, j++ ) );//Elevation
                gpsInfo__.EPH_State[i][2] = GpsAtof( GetDataIndexOfComma(nmeaData, j++ ) );//Azimuth
                gpsInfo__.EPH_State[i][3] = GpsAtof( GetDataIndexOfComma(nmeaData, j++ ) );//SNR (C/No) dBHz
            }
        }
        ret = true;
    }
    else if(strstr(nmeaData, "VTG" ) ){
        gpsInfo__.Course = GpsAtof( GetDataIndexOfComma(nmeaData, 1 ) );//True Course
        gpsInfo__.Magnetic_Course = GpsAtof( GetDataIndexOfComma(nmeaData, 3 ) );//Magnetic Course
        gpsInfo__.Knot_Speed = GpsAtof( GetDataIndexOfComma(nmeaData, 5 ) );//knots
        gpsInfo__.Speed = GpsAtof( GetDataIndexOfComma(nmeaData, 6 ) );//km/hr
        ret = true;
    }
    else
    {
        ret = false;
    }
    GPS_FREE(nmeaData);
    return ret;
}

bool ParseOneNmea(uint8_t* nmea)
{
    char* line = nmea;
    switch (minmea_sentence_id(line, false)) {
        case MINMEA_SENTENCE_RMC: {
            struct minmea_sentence_rmc frame;
            if (minmea_parse_rmc(&frame, line)) {
                Trace(9,"$xxRMC: raw coordinates and speed: (%d/%d,%d/%d) %d/%d\n",
                        frame.latitude.value, frame.latitude.scale,
                        frame.longitude.value, frame.longitude.scale,
                        frame.speed.value, frame.speed.scale);
                Trace(9,"$xxRMC fixed-point coordinates and speed scaled to three decimal places: (%d,%d) %d\n",
                        minmea_rescale(&frame.latitude, 1000),
                        minmea_rescale(&frame.longitude, 1000),
                        minmea_rescale(&frame.speed, 1000));
                // Trace(9,"$xxRMC floating point degree coordinates and speed: (%f,%f) %f\n",
                //         minmea_tocoord(&frame.latitude),
                //         minmea_tocoord(&frame.longitude),
                //         minmea_tofloat(&frame.speed));
            }
            else {
                Trace(9,"$xxRMC sentence is not parsed\n");
            }
        } break;

        case MINMEA_SENTENCE_GGA: {
            struct minmea_sentence_gga frame;
            if (minmea_parse_gga(&frame, line)) {
                Trace(9,"$xxGGA: fix quality: %d\n", frame.fix_quality);
            }
            else {
                Trace(9,"$xxGGA sentence is not parsed\n");
            }
        } break;

        case MINMEA_SENTENCE_GST: {
            struct minmea_sentence_gst frame;
            if (minmea_parse_gst(&frame, line)) {
                Trace(9,"$xxGST: raw latitude,longitude and altitude error deviation: (%d/%d,%d/%d,%d/%d)\n",
                        frame.latitude_error_deviation.value, frame.latitude_error_deviation.scale,
                        frame.longitude_error_deviation.value, frame.longitude_error_deviation.scale,
                        frame.altitude_error_deviation.value, frame.altitude_error_deviation.scale);
                Trace(9,"$xxGST fixed point latitude,longitude and altitude error deviation"
                        " scaled to one decimal place: (%d,%d,%d)\n",
                        minmea_rescale(&frame.latitude_error_deviation, 10),
                        minmea_rescale(&frame.longitude_error_deviation, 10),
                        minmea_rescale(&frame.altitude_error_deviation, 10));
                // Trace(9,"$xxGST floating point degree latitude, longitude and altitude error deviation: (%f,%f,%f)",
                //         minmea_tofloat(&frame.latitude_error_deviation),
                //         minmea_tofloat(&frame.longitude_error_deviation),
                //         minmea_tofloat(&frame.altitude_error_deviation));
            }
            else {
                Trace(9,"$xxGST sentence is not parsed\n");
            }
        } break;

        case MINMEA_SENTENCE_GSV: {
            struct minmea_sentence_gsv frame;
            if (minmea_parse_gsv(&frame, line)) {
                Trace(9,"$xxGSV: message %d of %d\n", frame.msg_nr, frame.total_msgs);
                Trace(9,"$xxGSV: sattelites in view: %d\n", frame.total_sats);
                for (int i = 0; i < 4; i++)
                    Trace(9,"$xxGSV: sat nr %d, elevation: %d, azimuth: %d, snr: %d dbm\n",
                        frame.sats[i].nr,
                        frame.sats[i].elevation,
                        frame.sats[i].azimuth,
                        frame.sats[i].snr);
            }
            else {
                Trace(9,"$xxGSV sentence is not parsed\n");
            }
        } break;

        case MINMEA_SENTENCE_VTG: {
            struct minmea_sentence_vtg frame;
            if (minmea_parse_vtg(&frame, line)) {
                Trace(9,"$xxVTG: true track degrees = %d\n",
                        minmea_tofloat(&frame.true_track_degrees));
                Trace(9,"        magnetic track degrees = %d\n",
                        minmea_tofloat(&frame.magnetic_track_degrees));
                Trace(9,"        speed knots = %d\n",
                        minmea_tofloat(&frame.speed_knots));
                Trace(9,"        speed kph = %d\n",
                        minmea_tofloat(&frame.speed_kph));
            }
            else {
                Trace(9,"$xxVTG sentence is not parsed\n");
            }
        } break;

        case MINMEA_SENTENCE_ZDA: {
            struct minmea_sentence_zda frame;
            if (minmea_parse_zda(&frame, line)) {
                Trace(9,"$xxZDA: %d:%d:%d %02d.%02d.%d UTC%+03d:%02d\n",
                        frame.time.hours,
                        frame.time.minutes,
                        frame.time.seconds,
                        frame.date.day,
                        frame.date.month,
                        frame.date.year,
                        frame.hour_offset,
                        frame.minute_offset);
            }
            else {
                Trace(9,"$xxZDA sentence is not parsed\n");
            }
        } break;

        case MINMEA_INVALID:
        default:
            Trace(9,"$xxxxx sentence is not parsed\n");
            return false;
    }
    return true;
}


bool GPS_Parse(uint8_t* nmeas)
{
    bool retFlag = false;
    uint8_t tmpStore;
    while(nmeas)
    {
        uint8_t* index1 = strstr(nmeas,"$");
        if(index1)
        {
            retFlag = true;
            uint8_t* index2 = strstr(index1,"\r\n");
            if(index2)
            {
                tmpStore = index2[2];
                index2[2] = '\0';
                nmeas += index2-index1+2;
                // GPS_ParseOneNMEA(index1);
                retFlag = ParseOneNmea(index1);
                index2[2] = tmpStore;
            }
        }
        else
            break;
    }
    return retFlag;
}



GPS_Information_t* Gps_GetInfo()
{
    return &gpsInfo__;
}

