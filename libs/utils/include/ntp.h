#ifndef __NTP_H
#define __NTP_H

#include <time.h>

/**
  * Get UTC time from NTP server
  * 
  * @attention Just get UTC time from server, no time zone!!
  *            get time zone from base station or other server
  * 
  * @param server: ntp server ip or domain
  * @param timeoutS: ntp request timeout, unit:s
  * @param timeNow: time sync from NTP server if NTP update success
  * @param isSetRTC: if set RTC time once get time success
  * 
  * @return time_t: return 0 if success, or return error code( < 0)
  * 
  */
int NTP_Update(const char* server, time_t timeoutS, time_t* utcTime, bool isSetRTC);


#endif
