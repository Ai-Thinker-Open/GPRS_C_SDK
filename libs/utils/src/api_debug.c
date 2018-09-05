

#include "api_debug.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"

void Trace_MemBlock(uint16_t nIndex, uint8_t *buffer, uint16_t len, uint8_t radix)
{
    int32_t i            = 0;
    char s[4]          = { 0 };
    char line_buff[64] = { 0 }; // Temporary buffer for print trace information.

    Trace(nIndex, "==================TRACE MEMORY BLOCK=====================>>");
    Trace(nIndex, "   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15");

    // Just print part of the data if the data length overrun the temporary buffer size.

    // ///////////////////////////////////////////////////////////////////////////
    // the buffer of vsprintf is not more than 256(the max number chars of each line), so
    // the size of the buffer to need to print is not limited, line_buffer size can be less
    // than 256 each line and it can be used circularly, modify by wangqunyang 2008.06.02
    // ///////////////////////////////////////////////////////////////////////////
    /*
       if (len > sizeof(line_buff))
       len = sizeof(line_buff);
     */

    for (i = 0; i < len; i++)
    {
        if (radix == 10)
            sprintf(s, "%3d ", buffer[i]);
        else if (radix == 16)
            sprintf(s, "%02x ", buffer[i]);
        else  // default case
            sprintf(s, "%02x ", buffer[i]);

        /* we can print 10 chars each line or any others being less than
           ** line_buff size and in this function max size of line_buff is 64 */
        if (i != 0 && i % radix == 0)
        {
            Trace(nIndex, "%p| %s |", buffer+i, line_buff);
            memset(line_buff, 0, sizeof(line_buff));
        }

        strcat(line_buff, s);
    }

    if (strlen(line_buff) > 0)
        Trace(nIndex, "%p| %s ", buffer+i, line_buff);

    Trace(nIndex, "==================TRACE MEMORY BLOCK=====================<<");
}

