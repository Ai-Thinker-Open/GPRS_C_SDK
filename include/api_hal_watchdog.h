

#ifndef __API_HAL_WATCHDOS_H
#define __API_HAL_WATCHDOS_H

#include "sdk_init.h"


#define WATCHDOG_SECOND_TO_TICK(s) (s*16384)

// =============================================================================
// WatchDog_Open
// -----------------------------------------------------------------------------
/// This function starts a watchdog timer which will start with an initial value
/// of \c delay and decrement by one every 2 periods of a 32 khz clock. When
/// the watchdog timer reaches 0, a reset is generated.
///
/// @param delay Number of 16384 Hz clock cycles (e.g. 30*16384 means 30 seconds)
/// the WatchDog timer counts down before reaching 0. 
///
/// @attention in release software(build by `./build.bat app release`),
/// watchdog was actived with 30 seconds delay by default,
/// and auto keep alive in idle task, if you open watchdog by this function,
/// the idle task will not feed(call WatchDog_KeepAlive) any more, you must 
/// call `WatchDog_KeepAlive` to prevent the reset of system
// =============================================================================
// void WatchDog_Open(uint32_t delay);
#define WatchDog_Open   CSDK_FUNC(WatchDog_Open)


// =============================================================================
// WatchDogSetPeriod
// -----------------------------------------------------------------------------
/// Setup the watchdog timer which will start at \c delay and decrement by one
/// every 2 clocks 32kHz, after #. Will generate a reset when zero is reached
/// If the WD timer is already started, the timer will be reseted to the new
/// value.
/// If the WD timer is not started yet, this will only setup the configuration.
/// The WD timer will be started the next time #WatchDogKeepAlive is
/// called
/// @param delay Number of 16384 Hz clock cycles (e.g. 30*16384 means 30 seconds)
/// the WatchDog timer counts down before reaching 0. 
// =============================================================================
// void WatchDogSetPeriod(uint32_t delay);
#define WatchDogSetPeriod   CSDK_FUNC(WatchDogSetPeriod)



// =============================================================================
// WatchDog_KeepAlive
// -----------------------------------------------------------------------------
/// Reloads the watchdog timer
/// This function restarts the watchdog timer with the \c delay used to start
/// it (#WatchDog_Open) or to set it up (#WatchDogSetPeriod),
/// preventing the reset from occurring.
// =============================================================================
void WatchDog_KeepAlive(void);
#define WatchDog_KeepAlive   CSDK_FUNC(WatchDog_KeepAlive)


// =============================================================================
// WatchDog_Close
// -----------------------------------------------------------------------------
/// Disables the watchdog timer
/// This function disables the watchdog timer. A reset will not occur because
/// of the watchdog, even if #WatchDogKeepAlive is not called. The
/// watchdog can be re-enabled by #WatchDog_Open.
// =============================================================================
void WatchDog_Close(void);
#define WatchDog_Close   CSDK_FUNC(WatchDog_Close)


#endif

