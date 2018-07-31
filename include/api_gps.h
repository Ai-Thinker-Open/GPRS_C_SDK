#ifndef __API_GPS_H_
#define __API_GPS_H_


#include <sdk_init.h>


/// \brief  bool GPS_Open(UART_Callback_t gpsReceivedCallback);
/// \param  UART_Callback_t gpsReceivedCallback, gps uart callback func
/// \return bool
#define GPS_Open                                        CSDK_FUNC(GPS_Open)

/// \brief  bool GPS_Close(void);
/// \param  void
/// \return bool
#define GPS_Close                                       CSDK_FUNC(GPS_Close)

#define GPS_IsOpen                                      CSDK_FUNC(GPS_IsOpen)



#endif

