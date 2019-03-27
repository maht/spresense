/****************************************************************************
 * modules/include/audio/audio_frontend_api.h
 *
 *   Copyright 2019 Sony Semiconductor Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __MODULES_INCLUDE_AUDIO_AUDIO_FRONTEND_API_H
#define __MODULES_INCLUDE_AUDIO_AUDIO_FRONTEND_API_H

/**
 * @defgroup audioutils Audio Utility
 * @{
 */

/**
 * @defgroup audioutils_audio_frontend_api Audio Frontend API
 * @{
 *
 * @file       audio_frontend_api.h
 * @brief      CXD5602 Audio Frontend API
 * @author     CXD5602 Audio SW Team
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "audio/audio_common_defs.h"
#include "audio/audio_object_common_api.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define AS_FEATURE_FRONTEND_ENABLE

/** @name Packet length of command*/
/** @{ */

/*! \brief InitMFE command (#AUDCMD_INITMFE) packet length */

#define  LENGTH_INITMFE             4

/*! \brief InitMFE command (#AUDCMD_SETMFE) packet length */

#define  LENGTH_SETMFE             4

/** @} */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/** Event type of FrontEnd */

typedef enum
{
  /*! \brief Activate */

  AsFrontendEventAct = 0,

  /*! \brief Deactivate */

  AsFrontendEventDeact,

  /*! \brief Init */

  AsFrontendEventInit,

  /*! \brief Start */

  AsFrontendEventStart,

  /*! \brief Stop */

  AsFrontendEventStop,

  /*! \brief Init PreProc */

  AsFrontendEventInitPreProc,

  /*! \brief Set PreProc */

  AsFrontendEventSetPreProc,

  /*! \brief Set MicGain */

  AsFrontendEventSetMicGain,

} AsFrontendEvent;

typedef enum
{
  AsFrontendDeviceMic = 0,

  AsFrontendDeviceI2s,

} AsFrontendInputDevice;

/** Select Frontend PreProcess Enable */

typedef enum
{
  /*! \brief Pre Process Disable */

  AsFrontendPreProcDisable = 0,

  /*! \brief Pre Process Enable */

  AsFrontendPreProcEnable,

} AsFrontendPreProc;

typedef enum
{
  /*! \brief PCM data is send by callback */

  AsDataPathCallback = 0,

  /*! \brief PCM data is send by message */

  AsDataPathMessage,

} AsFrontendDataPath;

/** Activate FrontEnd Command */

typedef struct
{
  /*! \brief [in] Select Frontend input device
   *
   * Use #AsSetRecorderStsInputDevice enum type
   */

  uint8_t  input_device;

  /*! \brief [in] Select pre process enable 
   *
   * Use #AsFrontendPreProc enum type
   */

  uint32_t  pre_enable;

} AsActivateFrontendParam;

typedef bool (*FrontendCallback)(AsFrontendEvent evtype, uint32_t result, uint32_t sub_result);

typedef struct
{
  AsActivateFrontendParam param;
   
  FrontendCallback cb;

} AsActivateFrontend;

/** Deactivate FrontEnd Command */

typedef struct
{
  uint32_t reserve;

} AsDeactivateFrontendParam;

/** InitFrontend Command parameter */

typedef void (*FrontendDoneCallback)(AsPcmDataParam param);

union AsDataDest
{
  FrontendDoneCallback cb;

  struct __st_tunnel
  {
    uint8_t msgqid;
    uint32_t msgtype;
  } msg;
};

typedef struct
{
  /*! \brief [in] Select InitFrontend input channels
   */

  uint8_t  channel_number;

  /*! \brief [in] Select InitFrontend input bit length
   */

  uint8_t  bit_length;

  /*! \brief [in] Samples per a frame
   */

  uint32_t samples_per_frame;

  /*! \brief [in] Select Data path from Frontend 
   *
   * Use #AsFrontendDataPath enum type
   */

  uint8_t  data_path;

  /*! \brief [in] Data destination, callback or message 
   *
   * Follow "data_path" parameter to select callback or message
   */

  AsDataDest dest;

} AsInitFrontendParam;

/** StartFrontend Command parameter */

typedef struct
{
  uint32_t reserve;

} AsStartFrontendParam;

/** StopFrontend Command parameter */

typedef struct
{
  uint32_t stop_mode;

} AsStopFrontendParam;

/** InitPreProc, SetPreProc Command parameter */

typedef struct
{
  /*! \brief [in] Command packet address */

  uint8_t  *packet_addr;

  /*! \brief [in] Command packet size */

  uint32_t packet_size;

} AsInitPreProcParam, AsSetPreProcParam;

/** Set Mic Gain Command parameter */

typedef struct
{
  /*! \brief [in] Mic gain
   * 
   *  Analog microphone can set every 0.5 dB between 0 dB and 21 dB.
   *  In this parameter, a value from 0 to 210 is set for every 5.
   *
   *  Digital microphone can set every 0.01 dB between 78.50 dB and 0.00 dB
   *  In this parameter, a value from -7850 to 0 is set for every 1.
   */

  int16_t mic_gain[AS_MIC_CHANNEL_MAX];
} AsFrontendMicGainParam;

/** FrontendCommand definition */

typedef union
{
  /*! \brief [in] for ActivateFrontend
   * (Object Interface==AS_ActivateFrontend)
   */
 
  AsActivateFrontend act_param;

  /*! \brief [in] for DeactivateFrontend
   * (Object Interface==AS_DeactivateFrontend)
   */
 
  AsDeactivateFrontendParam deact_param;

  /*! \brief [in] for InitFrontend
   * (Object Interface==AS_InitFrontend)
   */

  AsInitFrontendParam init_param;

  /*! \brief [in] for StartFrontend
   * (Object Interface==AS_StartFrontend)
   */

  AsStartFrontendParam start_param;

  /*! \brief [in] for StopFrontend
   * (Object Interface==AS_StopFrontend)
   */

  AsStopFrontendParam stop_param;

  /*! \brief [in] for InitPreProc
   * (Object Interface==AS_InitPreProcFrontend)
   */

  AsInitPreProcParam initpreproc_param;

  /*! \brief [in] for SetPreProc
   * (Object Interface==AS_SetPreProcFrontend)
   */

  AsSetPreProcParam setpreproc_param;

  /*! \brief [in] for SetMicGain
   * (Object Interface==AS_SetMicGainFrontend)
   */

  AsFrontendMicGainParam mic_gain_param;

} FrontendCommand;

/** Message queue ID parameter of activate function */

typedef struct
{
  /*! \brief [in] Message queue id of frontend */

  uint8_t frontend;

  /*! \brief [in] Message queue id of audio_manager */

  uint8_t mng;

  /*! \brief [in] Message queue id of DSP */

  uint8_t dsp;

} AsFrontendMsgQueId_t;

/** Pool ID parameter of activate function */

typedef struct
{
  /*! \brief [in] Memory pool id of input data */

  uint8_t capin;

  /*! \brief [in] Memory pool id of PreProc */

  uint8_t output;

  /*! \brief [in] Memory pool id of dsp command data */

  uint8_t dspcmd;

} AsFrontendPoolId_t;

/** Activate function parameter */

typedef struct
{
  /*! \brief [in] ID for sending messages to each function */

  AsFrontendMsgQueId_t msgq_id;

  /*! \brief [in] ID of memory pool for processing data */

  AsFrontendPoolId_t   pool_id;

} AsCreateFrontendParam_t;

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief Create audio frontend
 *
 * @param[in] param: Parameters of resources used by audio frontend
 * @param[in] attcb: Attention callback of Frontend. NULL means no callback.
 *
 * @retval     true  : success
 * @retval     false : failure
 */

bool AS_CreateFrontend(FAR AsCreateFrontendParam_t *param,
                       AudioAttentionCb attcb);

/**
 * @brief Activate audio frontend
 *
 * @param[in] param: Activation parameters
 *
 * @retval     true  : success
 * @retval     false : failure
 */

bool AS_ActivateFrontend(FAR AsActivateFrontend *actparam);

/**
 * @brief Init audio frontend
 *
 * @param[in] param: Initialization parameters
 *
 * @retval     true  : success
 * @retval     false : failure
 */

bool AS_InitFrontend(FAR AsInitFrontendParam *initparam);

/**
 * @brief Start audio frontend
 *
 * @retval     true  : success
 * @retval     false : failure
 */

bool AS_StartFrontend(FAR AsStartFrontendParam *startparam);

/**
 * @brief Stop audio frontend
 *
 * @retval     true  : success
 * @retval     false : failure
 */

bool AS_StopFrontend(FAR AsStopFrontendParam *stopparam);

/**
 * @brief Init pre process 
 *
 * @retval     true  : success
 * @retval     false : failure
 */

bool AS_InitPreprocFrontend(FAR AsInitPreProcParam *param);

/**
 * @brief Set pre process 
 *
 * @retval     true  : success
 * @retval     false : failure
 */

bool AS_SetPreprocFrontend(FAR AsSetPreProcParam *param);

/**
 * @brief Set Mic gain 
 *
 * @retval     true  : success
 * @retval     false : failure
 */

bool AS_SetMicGainFrontend(FAR AsFrontendMicGainParam *micgain_param);

/**
 * @brief Deactivate audio frontend
 *
 * @retval     true  : success
 * @retval     false : failure
 */

bool AS_DeactivateFrontend(FAR AsDeactivateFrontendParam *deactparam);

/**
 * @brief Deactivate audio frontend
 *
 * @retval     true  : success
 * @retval     false : failure
 */

bool AS_DeleteFrontend(void);

#endif  /* __MODULES_INCLUDE_AUDIO_AUDIO_FRONTEND_API_H */
/**
 * @}
 */

/**
 * @}
 */
