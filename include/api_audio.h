#ifndef __API_AUDIO_H
#define __API_AUDIO_H

#include "sdk_init.h"


#define AUDIO_MicOpen          CSDK_FUNC(AUDIO_MicOpen)
#define AUDIO_MicClose         CSDK_FUNC(AUDIO_MicClose)
#define AUDIO_MicSetMute       CSDK_FUNC(AUDIO_MicSetMute)
#define AUDIO_SetMode          CSDK_FUNC(AUDIO_SetMode)
#define AUDIO_SpeakerOpen      CSDK_FUNC(AUDIO_SpeakerOpen)
#define AUDIO_SpeakerClose     CSDK_FUNC(AUDIO_SpeakerClose)
#define AUDIO_SpeakerSetVolume CSDK_FUNC(AUDIO_SpeakerSetVolume)
#define AUDIO_SpeakerSetMute   CSDK_FUNC(AUDIO_SpeakerSetMute)
#define AUDIO_Play             CSDK_FUNC(AUDIO_Play)
#define AUDIO_SetEQ            CSDK_FUNC(AUDIO_SetEQ)
#define AUDIO_Pause            CSDK_FUNC(AUDIO_Pause)
#define AUDIO_Resume           CSDK_FUNC(AUDIO_Resume)
#define AUDIO_Stop             CSDK_FUNC(AUDIO_Stop)
#define AUDIO_GetPlayInfo      CSDK_FUNC(AUDIO_GetPlayInfo)
#define AUDIO_LoopTestStart    CSDK_FUNC(AUDIO_LoopTestStart)
#define AUDIO_LoopTestEnd      CSDK_FUNC(AUDIO_LoopTestEnd)
#define AUDIO_RecordStart      CSDK_FUNC(AUDIO_RecordStart)
#define AUDIO_RecordStop       CSDK_FUNC(AUDIO_RecordStop)


#endif
