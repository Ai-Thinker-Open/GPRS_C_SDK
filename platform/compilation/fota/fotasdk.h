/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/

#ifndef _FOTA_SDK_H_
#define _FOTA_SDK_H_

#ifdef _WIN32
#ifdef FOTASDK_EXPORTS
#define FOTASDK_CAPI extern "C" __declspec(dllexport)
#else
#define FOTASDK_CAPI extern "C" __declspec(dllimport)
#endif
#else
#ifdef FOTASDK_EXPORTS
#define FOTASDK_CAPI extern "C" __attribute__((__visibility__("default")))
#else
#define FOTASDK_CAPI extern "C"
#endif
#endif

enum
{
    ERR_INVALID_SECTOR_SIZE = 1, // must be 2^n
    ERR_CODE_BASE_UNALIGNED,
    ERR_CODE_SIZE_UNALIGNED,
    ERR_FOTA_END_UNAIGNED,       // FOTA area end must be block aligned
    ERR_FOTA_START_UNALIGNED,    // FOTA area start must be sector aligned
    ERR_FOTA_INVALID_AREA,       // FOTA area not in FLASH range
    ERR_FAIL_READ_OLD,
    ERR_FAIL_READ_NEW,
    ERR_OLD_TOO_MANY_PACKET,   // must be contiguous
    ERR_OLD_SIZE_UNALIGNED,    // must be sector aligned
    ERR_OLD_BASE_MISMATCH,
    ERR_OLD_SIZE_OVERFLOW,
    ERR_NEW_TOO_MANY_PACKET,   // must be contiguous
    ERR_NEW_SIZE_UNALIGNED,    // must be sector aligned
    ERR_NEW_BASE_MISMATCH,
    ERR_NEW_SIZE_OVERFLOW,
    ERR_FAIL_WRITE_PACK,
    ERR_FAIL_READ_PACK,
    ERR_VERIFY_PACK_INVALID,
    ERR_VERIFY_UPGRADE_FAIL,
    ERR_VERIFY_UPGRADE_CLEAN_FAIL,
    ERR_VERIFY_CMP_FAIL
};

struct FotaFlashParam
{
    unsigned pysicalBase;   // FLASH physical address, such as 0x08000000
    unsigned codeFlashBase; // FLASH base for code (must be sector aligned)
    unsigned codeFlashSize; // FLASH size for code (must be sector aligned)
};

// File name strings are encoded as UTF-8

FOTASDK_CAPI int FotaCreatePack(const char *olodfname,  // input old LOD file name
                                const char *nlodfname,  // input new LOD file name
                                const char *patchfname, // output patch file name
                                unsigned *oldstart,     // return old LOD start
                                unsigned *oldsize,      // return old LOD size
                                unsigned *oldcrc,       // return old data CRC
                                struct FotaFlashParam flash);

FOTASDK_CAPI const char *FotaErrorMsg(int result);

#endif
