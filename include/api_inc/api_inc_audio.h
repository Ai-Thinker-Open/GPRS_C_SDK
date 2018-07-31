#ifndef __API_INC_AUDIO_H
#define __API_INC_AUDIO_H




typedef enum
{
    AUDIO_MODE_EARPIECE     = 0,
    AUDIO_MODE_HANDSET      = 1,
    AUDIO_MODE_LOUDSPEAKER  = 2,
    AUDIO_MODE_MAX
}AUDIO_Mode_t;

typedef enum
{
    AUDIO_TYPE_NONE=-1,
    AUDIO_TYPE_GSM_FR,              /* 0 */
    AUDIO_TYPE_GSM_HR,              /* 1 */
    AUDIO_TYPE_GSM_EFR,             /* 2 */
    AUDIO_TYPE_AMR,                 /* 3 */
    AUDIO_TYPE_AMR_WB,              /* 4 */
    AUDIO_TYPE_MP3,                 /* 5 */
    AUDIO_TYPE_AAC,                 /* 6 */
    AUDIO_TYPE_PCM_8K,              /* 7 */
    AUDIO_TYPE_PCM_16K,             /* 8 */
    AUDIO_TYPE_G711_ALAW,           /* 9 */
    AUDIO_TYPE_G711_ULAW,           /* 10 */
    AUDIO_TYPE_DVI_ADPCM,           /* 11 */
    AUDIO_TYPE_VR,                  /* 12 */
    AUDIO_TYPE_WAV,                 /* 13 */
    AUDIO_TYPE_WAV_ALAW,            /* 14 */
    AUDIO_TYPE_WAV_ULAW,            /* 15 */
    AUDIO_TYPE_WAV_DVI_ADPCM,       /* 16 */
    AUDIO_TYPE_SMF,                    /* 17 */
    AUDIO_TYPE_IMELODY,                /* 18 */
    AUDIO_TYPE_SMF_SND,             /* 19 */
    AUDIO_TYPE_MMF,                /* 20 */
    AUDIO_TYPE_AU,                 /*  21 */
    AUDIO_TYPE_AIFF,                 /*  22 */
    AUDIO_TYPE_M4A,                  /* 23  */
    AUDIO_TYPE_3GP,                  /*  24 */
    AUDIO_TYPE_MP4,                      /* 25 */
    AUDIO_TYPE_JPG,                      /* 26 */
    AUDIO_TYPE_GIF,                      /* 27 */
    AUDIO_TYPE_MJPG,                     /* 28 */
    AUDIO_TYPE_WMA,                  /* 29 */
    AUDIO_TYPE_MIDI,                  /* 30 */
    AUDIO_TYPE_RM,              /*31*/
    AUDIO_TYPE_AVSTRM,         /*32*/
    AUDIO_TYPE_SCO,                 /*33*/

    AUDIO_TYPE_MAX
} AUDIO_Type_t;


typedef enum
{
    AUDIO_EQ_NORMAL,   /*EQ=0*/
    AUDIO_EQ_BASS,     /*EQ=1*/
    AUDIO_EQ_DANCE,    /*EQ=2*/
    AUDIO_EQ_CLASSICAL,/*EQ=3*/
    AUDIO_EQ_TREBLE,   /*EQ=4*/
    AUDIO_EQ_PARTY,    /*EQ=5*/
    AUDIO_EQ_POP,      /*EQ=6*/
    AUDIO_EQ_ROCK,     /*EQ=7*/
    AUDIO_EQ_MAX
 } AUDIO_EQ_t;

typedef enum
{
    AUDIO_ERROR_NO              = 0,
    AUDIO_ERROR_UNKNOWN_FORMAT,
    AUDIO_ERROR_BUSY,
    AUDIO_ERROR_INVALID_PARAMETER,
    AUDIO_ERROR_ACTION_NOT_ALLOWED,
    AUDIO_ERROR_OUT_OF_MEMORY,
    AUDIO_ERROR_CANNOT_OPEN_FILE,
    AUDIO_ERROR_END_OF_FILE,
    AUDIO_ERROR_TERMINATED,
    AUDIO_ERROR_BAD_FORMAT,
    AUDIO_ERROR_INVALID_FORMAT,
    AUDIO_ERROR_ERROR,
    AUDIO_ERROR_DISK_FULL,
    AUDIO_ERROR_BUSY_CALLING ,
    AUDIO_ERROR_FILE_OPERATE,

    AUDIO_ERROR_MAX
} AUDIO_Error_t;

typedef struct
{
    int32_t playProgress;
    int32_t reserve;
} AUDIO_Play_Info_t;

typedef enum
{
    AUDIO_RECORD_MODE_AMR475,
    AUDIO_RECORD_MODE_AMR515,
    AUDIO_RECORD_MODE_AMR59,
    AUDIO_RECORD_MODE_AMR67,
    AUDIO_RECORD_MODE_AMR74,
    AUDIO_RECORD_MODE_AMR795,
    AUDIO_RECORD_MODE_AMR102,
    AUDIO_RECORD_MODE_AMR122,
    AUDIO_RECORD_MODE_FR,
    AUDIO_RECORD_MODE_HR,
    AUDIO_RECORD_MODE_EFR,
    AUDIO_RECORD_MODE_AMR_RING,
    AUDIO_RECORD_MODE_PCM,
    AUDIO_RECORD_MODE_MP3,
    AUDIO_RECORD_MODE_INVALID,

    AUDIO_RECORD_MODE_QTY
} AUDIO_Record_Mode_t;

typedef void (*AUDIO_PLAY_CALLBACK_t)(AUDIO_Error_t);
typedef void (*AUDIO_FILE_RECORD_CALLBACK_t)(AUDIO_Error_t result);
/**
 * 
 * @param record_len: the data saved to file successfully, value can be changed in this function,
 *                    if not, will keep the value of saved in file
 * 
 */
typedef void (*AUDIO_BUFFER_DATA_CALLBACK_t)(unsigned char* buf_p,unsigned int len, unsigned int *record_len) ;


#endif
