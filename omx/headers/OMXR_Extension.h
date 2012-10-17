/**********************************************************************
 *
 * PURPOSE
 *   Media Component Library Header File
 *
 * AUTHOR
 *   Renesas Electronics Corporation
 *
 * DATE
 *   2010/06/30
 *
 **********************************************************************/
/*
 * Copyright (C) Renesas Electronics Corporation 2010
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation.
 * No part of this program may be reproduced or disclosed to
 * others, in any form, without the prior written permission
 * of Renesas Electronics Corporation.
 *
 **********************************************************************/

#ifndef OMXR_EXTENSION_H
#define OMXR_EXTENSION_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************************************************/
/*    Include Header Files                                                 */
/***************************************************************************/
#include "OMX_Types.h"
#include "OMX_Core.h"
#include "OMX_Component.h"
#include "OMX_Index.h"
#include "OMX_Audio.h"
#include "OMX_Video.h"
#include "OMX_Image.h"
#include "OMX_IVCommon.h"
#include "OMX_Other.h"

/***************************************************************************/
/*    Macro Definitions                                                    */
/***************************************************************************/
/*******************/
/* Extended Index. */
/*******************/
enum {
    OMXR_MC_IndexParamVideoReorder                   = (OMX_IndexVendorStartUnused + 0x0000),  /* OMX.RENESAS.INDEX.PARAM.VIDEO.REORDER */
    OMXR_MC_IndexParamVideoStreamStoreUnit           = (OMX_IndexVendorStartUnused + 0x0001),  /* OMX.RENESAS.INDEX.PARAM.VIDEO.STREAM.STORE.UNIT */
    OMXR_MC_IndexParamVideoPictureMemoryAlloc        = (OMX_IndexVendorStartUnused + 0x0002),  /* OMX.RENESAS.INDEX.PARAM.VIDEO.PICTURE.MEMORY.ALLOC */
    OMXR_MC_IndexParamVideoEncMode                   = (OMX_IndexVendorStartUnused + 0x0003),  /* OMX.RENESAS.INDEX.PARAM.VIDEO.ENC.MODE */
/* for_VC1D_start */
    OMX_IndexParamVideoVC1                           = (OMX_IndexVendorStartUnused + 0x0004),  /* OMX.RENESAS.INDEX.PARAM.VIDEO.VC1 */
/* for_VC1D_end */
    OMXR_MC_IndexParamVideoTimeStampMode             = (OMX_IndexVendorStartUnused + 0x0005),  /* OMX.RENESAS.INDEX.PARAM.VIDEO.TIME.STAMP.MODE */
    OMXR_MC_IndexParamVideoFirstInterframeIgnoreMode = (OMX_IndexVendorStartUnused + 0x0006),  /* OMX.RENESAS.INDEX.PARAM.VIDEO.FIRST.INTERFRAME.IGNORE.MODE */
/* for_DIVXD_start */
    OMXR_MC_IndexParamVideoDivX                      = (OMX_IndexVendorStartUnused + 0x0007),  /* OMX.RENESAS.INDEX.PARAM.VIDEO.DIVX */
/* for_DIVXD_end */
    OMXR_MC_IndexConfigVideoPlayMode                 = (OMX_IndexVendorStartUnused + 0x1000),  /* OMX.RENESAS.INDEX.CONFIG.VIDEO.PLAY.MODE */
/* for_DDD_SPU_start */
    OMXR_MC_IndexParamAudioAc3d                      = (OMX_IndexVendorStartUnused + 0x2000),  /* OMX.RENESAS.INDEX.CONFIG.AUDIO.AC3D */
/* for_DDD_SPU_end */
/* for_DDE_start */
    OMXR_MC_IndexParamAudioAc3e                      = (OMX_IndexVendorStartUnused + 0x2001),  /* OMX.RENESAS.INDEX.CONFIG.AUDIO.AC3E */
/* for_DDE_end */
    OMXR_MC_IndexConfigAudioPcm                      = (OMX_IndexVendorStartUnused + 0x3000),  /* OMX.RENESAS.INDEX.CONFIG.AUDIO.PCM */
    OMXR_MC_IndexCustomizeStartUnused                = (OMX_IndexVendorStartUnused + 0x6000),
};

enum {
    OMXR_MC_EventPortSettingChange = OMX_EventVendorStartUnused
};

/************************/
/* Extended Coding type.*/
/************************/
enum {
/* for_VC1D_start */
    OMX_VIDEO_CodingVC1      = OMX_VIDEO_CodingVendorStartUnused + 0x0000,
/* for_VC1D_end */
/* for_VP6D_start */
    OMXR_MC_VIDEO_CodingVP6  = OMX_VIDEO_CodingVendorStartUnused + 0x0001,
/* for_VP6D_end */
/* for_DIVXD_start */
    OMXR_MC_VIDEO_CodingDivX = OMX_VIDEO_CodingVendorStartUnused + 0x0002,
/* for_DIVXD_end */
/* for_DDD_SPU_start */
    OMXR_MC_AUDIO_CodingAC3  = OMX_AUDIO_CodingVendorStartUnused + 0x0000,
/* for_DDD_SPU_end */
    OMXR_MC_AUDIO_CodingCustomizeStartUnused
};

/************************/
/* Extended Error Code. */
/************************/
#define OMXR_MC_ErrorEffectuateErrorConcealment (OMX_S32)(0x90000000)
#define OMXR_MC_ErrorPcmTimeout                 (OMX_S32)(0x90000001)

#define OMXR_MGR_ErrorBusy                      (OMX_S32)(0x90000002)
#define OMXR_MGR_ErrorFileNotFound              (OMX_S32)(0x90000003)
#define OMXR_MGR_ErrorFileRead                  (OMX_S32)(0x90000004)
#define OMXR_MGR_ErrorFromSpx                   (OMX_S32)(0x90000005)
#define OMXR_MGR_ErrorIllegalConfig             (OMX_S32)(0x90000006)
#define OMXR_MGR_ErrorIllegalMEng               (OMX_S32)(0x90000007)
#define OMXR_MGR_ErrorInvalidHandle             (OMX_S32)(0x90000008)
#define OMXR_MGR_ErrorInvalidMessageBox         (OMX_S32)(0x90000009)
#define OMXR_MGR_ErrorInvalidState              (OMX_S32)(0x9000000a)
#define OMXR_MGR_ErrorLessThroughput            (OMX_S32)(0x9000000b)
#define OMXR_MGR_ErrorNoSignal                  (OMX_S32)(0x9000000c)
#define OMXR_MGR_ErrorNothingToDo               (OMX_S32)(0x9000000d)
#define OMXR_MGR_ErrorOsWrapper                 (OMX_S32)(0x9000000e)
#define OMXR_MGR_ErrorParamNotFound             (OMX_S32)(0x9000000f)
#define OMXR_MGR_ErrorSectionNotFound           (OMX_S32)(0x90000010)
#define OMXR_MGR_ErrorThreadShared              (OMX_S32)(0x90000011)
#define OMXR_MGR_ErrorMEngNotFound              (OMX_S32)(0x90000012)

/*************************/
/* Extended Buffer Flas. */
/*************************/
#define OMXR_MC_BUFFERFLAG_SEEK                 (0x00010000)
#define OMXR_MC_BUFFERFLAG_CLEARPCM             (0x00020000)
#define OMXR_MC_BUFFERFLAG_ENDOFNAL             (0x00040000)

/**************************/
/* Extended Channel Mode. */
/**************************/
typedef enum OMXR_MC_AUDIO_CHANNELMODETYPE {
    OMXR_MC_AUDIO_ChannelModeDownMixStereo = 0x00,
    OMXR_MC_AUDIO_ChannelModeMono          = 0x01,
    OMXR_MC_AUDIO_ChannelModeStereo        = 0x02,
    OMXR_MC_AUDIO_ChannelModeMonoR         = 0x12,
    OMXR_MC_AUDIO_ChannelModeMonoL         = 0x22,
    OMXR_MC_AUDIO_ChannelMode3_0           = 0x03,
    OMXR_MC_AUDIO_ChannelMode2_1           = 0x13,
    OMXR_MC_AUDIO_ChannelMode3_1           = 0x04,
    OMXR_MC_AUDIO_ChannelMode2_2           = 0x14,
    OMXR_MC_AUDIO_ChannelMode3_2           = 0x05,
    OMXR_MC_AUDIO_ChannelModeEnd           = 0x7FFFFFFF
} OMXR_MC_AUDIO_CHANNELMODETYPE;

/****************************/
/* Extended Dual/Mono Mode. */
/****************************/
typedef enum OMXR_MC_AUDIO_DUALMONOTYPE {
    OMXR_MC_AUDIO_DualMonoModeMain    = 1,
    OMXR_MC_AUDIO_DualMonoModeSub     = 2,
    OMXR_MC_AUDIO_DualMonoModeMainSub = 3,
    OMXR_MC_AUDIO_DualMonoModeEnd     = 0x7FFFFFFF
} OMXR_MC_AUDIO_DUALMONOTYPE;

/**************************/
/* Extended Object Value. */
/**************************/
typedef enum OMXR_MC_VIDEO_OBJECTTYPE {
    OMXR_MC_VIDEO_ObjectMPEG4Simple         = 0,
    OMXR_MC_VIDEO_ObjectMPEG4AdvancedSimple = 1,
    OMXR_MC_VIDEO_ObjectUnknown             = 2,
    OMXR_MC_VIDEO_ObjectNone                = 3,
    OMXR_MC_VIDEO_ObjectEnd                 = 0x7FFFFFFF
} OMXR_MC_VIDEO_OBJECTTYPE;

/* for_DDD_SPU_start *//* for_DDE_start */
/****************************/
/* Extended Kcapable Mode.  */
/****************************/
typedef enum OMXR_MC_KCAPABLE_MODETYPE {
    OMXR_MC_AC3_VOCAL_NO    = 0,
    OMXR_MC_AC3_VOCAL_LEFT  = 1,
    OMXR_MC_AC3_VOCAL_RIGHT = 2,
    OMXR_MC_AC3_VOCAL_BOTH  = 3
} OMXR_MC_KCAPABLE_MODETYPE;

/****************************/
/*Extended Compression Mode */
/****************************/
typedef enum OMXR_MC_COMPRESSION_MODETYPE {
    OMXR_MC_AC3_NO_DIGITAL_DIALOG_NORMALIZATION = 0,
    OMXR_MC_AC3_DIGITAL_DIALOG_NORMALIZATION    = 1,
    OMXR_MC_AC3_LINE_OUT_MODE                   = 2,
    OMXR_MC_AC3_RF_MODE                         = 3
} OMXR_MC_COMPRESSION_MODETYPE;

/****************************/
/* Extended AC3 Stereo Mode */
/****************************/
typedef enum OMXR_MC_STEREO_MODETYPE {
    OMXR_MC_AC3_AUTO_DETECT               = 0,
    OMXR_MC_AC3_DOLBY_SURROUND_COMPATIBLE = 1,
    OMXR_MC_AC3_STEREO                    = 2
} OMXR_MC_STEREO_MODETYPE;

/****************************/
/* Extended AC3 Stereo Mode */
/****************************/
typedef enum OMXR_MC_DUALMONO_MODETYPE {
    OMXR_MC_AC3_DUAL_MONO_MODE_BOTH_CHANNEL  = 0,
    OMXR_MC_AC3_DUAL_MONO_MODE_1CHANNEL      = 1,
    OMXR_MC_AC3_DUAL_MONO_MODE_2CHANNEL      = 2,
    OMXR_MC_AC3_DUAL_MONO_MODE_MIX_CHANNEL   = 3
} OMXR_MC_DUALMONO_MODETYPE;

/****************************/
/* Extended AC3 Stereo Mode */
/****************************/
typedef enum tagOMXR_MC_AC3_STEREO_MODETYPE {
	OMXR_MC_AC3_STEREO_MODE_STEREO,
	OMXR_MC_AC3_STEREO_MODE_DUAL
} OMXR_MC_AC3_STEREO_MODETYPE;
/* for_DDD_SPU_end */

/****************************/
/* Extended AC3 Encode Mode */
/****************************/
typedef enum tagOMXR_MC_AC3_ENCODE_MODETYPE {
	OMXR_MC_AC3_ENCODE_MODE_AC3E,
	OMXR_MC_AC3_ENCODE_MODE_AC3E_TEST
} OMXR_MC_AC3_ENCODE_MODETYPE;
/* for_DDE_end */

/*********************************/
/* Extended Profile/Level Value. */
/*********************************/
/* for_H264D_start *//* for_H264E_start */
enum {
    OMXR_MC_VIDEO_AVCProfileMultiviewHigh = OMX_VIDEO_AVCProfileVendorStartUnused,
    OMXR_MC_VIDEO_AVCProfileStereoHigh,
    OMXR_MC_VIDEO_AVCProfileConstrainedBaseline,
    OMXR_MC_VIDEO_AVCProfileUnknown,
    OMXR_MC_VIDEO_AVCProfileNone,
    OMXR_MC_VIDEO_AVCProfileCustomizeStartUnused
};
enum {
    OMXR_MC_VIDEO_AVCLevelUnknown = OMX_VIDEO_AVCLevelVendorStartUnused,
    OMXR_MC_VIDEO_AVCLevelNone,
    OMXR_MC_VIDEO_AVCLevelCustomizeStartUnused
};
/* for_H264D_end *//* for_H264E_endt */
/* for_M4VD_start *//* for_M4VE_start *//* for_DIVXD_start */
enum {
    OMXR_MC_VIDEO_MPEG4ProfileUnknown = OMX_VIDEO_MPEG4ProfileVendorStartUnused,
	OMXR_MC_VIDEO_MPEG4ProfileNone,
    OMXR_MC_VIDEO_MPEG4ProfileCustomizeStartUnused
};
enum {
    OMXR_MC_VIDEO_MPEG4Level3b = OMX_VIDEO_MPEG4LevelVendorStartUnused,
	OMXR_MC_VIDEO_MPEG4Level6,
	OMXR_MC_VIDEO_MPEG4LevelUnknown,
	OMXR_MC_VIDEO_MPEG4LevelNone,
    OMXR_MC_VIDEO_MPEG4LevelCustomizeStartUnused
};
/* for_M4VD_end *//* for_M4VE_end *//* for_DIVXD_end */
/* for_H263D_start *//* for_H263E_start */
enum {
	OMXR_MC_VIDEO_H263ProfileUnknown = OMX_VIDEO_H263ProfileVendorStartUnused,
	OMXR_MC_VIDEO_H263ProfileNone,
    OMXR_MC_VIDEO_H263ProfileCustomizeStartUnused
};
enum {
	OMXR_MC_VIDEO_H263LevelUnknown = OMX_VIDEO_H263LevelVendorStartUnused,
	OMXR_MC_VIDEO_H263LevelNone,
    OMXR_MC_VIDEO_H263LevelCustomizeStartUnused
};
/* for_H263D_end *//* for_H263E_end */
/* for_M2VD_start */
enum {
	OMXR_MC_VIDEO_MPEG2ProfileMPEG1 = OMX_VIDEO_MPEG2ProfileVendorStartUnused,
	OMXR_MC_VIDEO_MPEG2ProfileUnknown,
	OMXR_MC_VIDEO_MPEG2ProfileNone,
    OMXR_MC_VIDEO_MPEG2ProfileCustomizeStartUnused
};
enum {
	OMXR_MC_VIDEO_MPEG2LevelMPEG1 = OMX_VIDEO_MPEG2LevelVendorStartUnused,
	OMXR_MC_VIDEO_MPEG2LevelHPL,
	OMXR_MC_VIDEO_MPEG2LevelUnknown,
	OMXR_MC_VIDEO_MPEG2LevelNone,
    OMXR_MC_VIDEO_MPEG2LevelCustomizeStartUnused
};
/* for_M2VD_end */
/* for_DIVXD_start */
typedef enum tagOMXR_MC_VIDEO_DIVXPROFILETYPE{
	OMXR_MC_VIDEO_DIVXProfileNone = OMXR_MC_VIDEO_MPEG4ProfileCustomizeStartUnused,
	OMXR_MC_VIDEO_DIVXProfileUnknown,
	OMXR_MC_VIDEO_DIVXProfileCustomizeStartUnused
} OMXR_MC_VIDEO_DIVXPROFILETYPE;

typedef enum tagOMXR_MC_VIDEO_DIVXLEVELTYPE{
	OMXR_MC_VIDEO_DIVXLevelNone = OMXR_MC_VIDEO_MPEG4LevelCustomizeStartUnused,
	OMXR_MC_VIDEO_DIVXLevelUnknown,
	OMXR_MC_VIDEO_DIVXLevelCustomizeStartUnused
} OMXR_MC_VIDEO_DIVXLEVELTYPE;
/* for_DIVXD_end */
/* for_VC1D_start */
typedef enum OMX_VIDEO_VC1PROFILETYPE {
	OMX_VIDEO_VC1ProfileSimple    = 0x01,       /**< baseline profile */
	OMX_VIDEO_VC1ProfileMain      = 0x02,       /**< main     profile */
	OMX_VIDEO_VC1ProfileAdvanced  = 0x04,       /**< advanced profile */
	OMX_VIDEO_VC1ProfileUnknown   = 0x7F000000, /**< Unknown  profile */
	OMX_VIDEO_VC1ProfileNone      = 0x7F000001, /**< None     profile */
	OMX_VIDEO_VC1ProfileUnused    = 0x7F000002,
	OMX_VIDEO_VC1ProfileEnd       = 0x7FFFFFFF
} OMX_VIDEO_VC1PROFILETYPE;

typedef enum OMX_VIDEO_VC1LEVELTYPE {
	OMX_VIDEO_VC1LevelLow     = 0x01,       /**< Level low     */
	OMX_VIDEO_VC1LevelMedium  = 0x02,       /**< Level medium  */
	OMX_VIDEO_VC1LevelHigh    = 0x04,       /**< Level high    */
	OMX_VIDEO_VC1Level0       = 0x08,       /**< Level 0       */
	OMX_VIDEO_VC1Level1       = 0x10,       /**< Level 1       */
	OMX_VIDEO_VC1Level2       = 0x20,       /**< Level 2       */
	OMX_VIDEO_VC1Level3       = 0x40,       /**< Level 3       */
	OMX_VIDEO_VC1Level4       = 0x80,       /**< Level 4       */
	OMX_VIDEO_VC1LevelUnknown = 0x7F000000, /**< Level Unknown */
	OMX_VIDEO_VC1LevelNone    = 0x7F000001, /**< Level None    */
	OMX_VIDEO_VC1LevelUnused  = 0x7F000002,
	OMX_VIDEO_VC1LevelEnd     = 0x7FFFFFFF
} OMX_VIDEO_VC1LEVELTYPE;
/* for_VC1D_end */
/* for_VP6D_start */
typedef enum OMXR_MC_VIDEO_VP6PROFILETYPE {
	OMXR_MC_VIDEO_VP6ProfileVP60    = 0x1,        /**< Profile VP60R Simple */
	OMXR_MC_VIDEO_VP6ProfileVP61    = 0x2,        /**< Profile VP61R Advanced */
	OMXR_MC_VIDEO_VP6ProfileVP62    = 0x4,        /**< Profile VP62R Heightened Sharpness */
	OMXR_MC_VIDEO_VP6ProfileUnknown = 0x6F000000, /**< Profile Unknown */
	OMXR_MC_VIDEO_VP6ProfileNone    = 0x6F000001, /**< Profile None  */
	OMXR_MC_VIDEO_VP6ProfileUnused  = 0x6F000002,
	OMXR_MC_VIDEO_VP6ProfileCustomizeStartUnused = 0x7F000000,
	OMXR_MC_VIDEO_VP6ProfileEnd     = 0x7FFFFFFF
} OMXR_MC_VIDEO_VP6PROFILETYPE;

typedef enum OMXR_MC_VIDEO_VP6LEVELTYPE{
	OMXR_MC_VIDEO_VP6LevelUnknown = 0x6F000000, /**< Level Unknown */
	OMXR_MC_VIDEO_VP6LevelNone    = 0x6F000001, /**< Level None  */
	OMXR_MC_VIDEO_VP6LevelUnused  = 0x6F000002,
	OMXR_MC_VIDEO_VP6LevelCustomizeStartUnused = 0x7F000000,
	OMXR_MC_VIDEO_VP6LevelEnd     = 0x7FFFFFFF
} OMXR_MC_VIDEO_VP6LEVELTYPE;
/* for_VP6D_end */

/* for_DIVXD_start */
/******************************/
/* Extended DivX Version Type */
/******************************/
typedef enum tagOMXR_MC_VIDEO_DIVXVERSIONTYPE{
    OMXR_MC_VIDEO_DIVXVersion311  = 0x00000000,
    OMXR_MC_VIDEO_DIVXVersion4    = 0x00000001,
    OMXR_MC_VIDEO_DIVXVersion5    = 0x00000002,
    OMXR_MC_VIDEO_DIVXVersion6    = 0x00000004,
    OMXR_MC_VIDEO_DIVXVersionEnd  = 0x7FFFFFFF
} OMXR_MC_VIDEO_DIVXVERSIONTYPE;
/* for_DIVXD_end */
/***************************/
/* Extended PictureStruct. */
/***************************/
typedef enum OMXR_MC_VIDEO_PICSTRUCTTYPE {
    OMXR_MC_VIDEO_PicStructFrame          = 0,
    OMXR_MC_VIDEO_PicStructFieldTopBottom = 1,
    OMXR_MC_VIDEO_PicStructFieldBottomTop = 2,
    OMXR_MC_VIDEO_PicStructTopBottom      = 3,
    OMXR_MC_VIDEO_PicStructBottomTop      = 4,
    OMXR_MC_VIDEO_PicStructTBRepeatedT    = 5,
    OMXR_MC_VIDEO_PicStructBTRepeatedB    = 6,
    OMXR_MC_VIDEO_PicStructFrameDoubling  = 7,
    OMXR_MC_VIDEO_PicStructFrameTripling  = 8,
    OMXR_MC_VIDEO_PicStructEnd            = 0x7FFFFFFF
} OMXR_MC_VIDEO_PICSTRUCTTYPE;

/***************************/
/* Extended PictureStatus. */
/***************************/
typedef enum OMXR_MC_VIDEO_PICSTATUSTYPE {
    OMXR_MC_VIDEO_PicStatusOk        = 0,
    OMXR_MC_VIDEO_PicStatusConcealed = 1,
    OMXR_MC_VIDEO_PicStatusEnd       = 0x7FFFFFFF
} OMXR_MC_VIDEO_PICSTATUSTYPE;

/*************************/
/* Extended PictureType. */
/*************************/
enum {
    OMXR_MC_VIDEO_PictureTypeIDR = OMX_VIDEO_PictureTypeVendorStartUnused,
    OMXR_MC_VIDEO_PictureTypeD,
    OMXR_MC_VIDEO_PictureTypeBI,
    OMXR_MC_VIDEO_PictureTypeSKIP,
    OMXR_MC_VIDEO_PictureTypeUnknown,
    OMXR_MC_VIDEO_PictureTypeCustomizeStartUnused
};

/****************************/
/* Extended StoreUnit Mode. */
/****************************/
typedef enum OMXR_MC_VIDEO_STOREUNITTYPE {
    OMXR_MC_VIDEO_StoreUnitFrame           = 0,
    OMXR_MC_VIDEO_StoreUnitPicture         = 1,
    OMXR_MC_VIDEO_StoreUnitNal             = 2,
    OMXR_MC_VIDEO_StoreUnitMultiFrame      = 3,
    OMXR_MC_VIDEO_StoreUnitOpenCore        = 4,
    OMXR_MC_VIDEO_StoreUnitStageFright     = 5,
    OMXR_MC_VIDEO_StoreUnitPackedBitstream = 6,
    OMXR_MC_VIDEO_StoreUnitEnd             = 0x7FFFFFFF
} OMXR_MC_VIDEO_STOREUNITTYPE;

/******************************/
/* Extended MemoryAlloc Mode. */
/******************************/
typedef enum OMXR_MC_VIDEO_MEMORYALLOCTYPE {
    OMXR_MC_VIDEO_MemAllocFrame        = 0,
    OMXR_MC_VIDEO_MemAllocFrameTff     = 1,
    OMXR_MC_VIDEO_MemAllocFrameBff     = 2,
    OMXR_MC_VIDEO_MemAllocField        = 3,
    OMXR_MC_VIDEO_MemAllocFieldTff     = 4,
    OMXR_MC_VIDEO_MemAllocFieldBff     = 5,
    OMXR_MC_VIDEO_MemAllocFrameOrField = 6,
    OMXR_MC_VIDEO_MemAllocEnd          = 0x7FFFFFFF
} OMXR_MC_VIDEO_MEMORYALLOCTYPE;

/***************************/
/* Extended PlayMode Mode. */
/***************************/
typedef enum OMXR_MC_VIDEO_PLAYMODETYPE {
    OMXR_MC_VIDEO_PlayModeForward       = 0,
    OMXR_MC_VIDEO_PlayModeIFrame        = 1,
    OMXR_MC_VIDEO_PlayModeBPSkipForward = 2,
    OMXR_MC_VIDEO_PlayModeEnd           = 0x7FFFFFFF
} OMXR_MC_VIDEO_PLAYMODETYPE;

/**************************/
/* Extended EncMode Mode. */
/**************************/
typedef enum OMXR_MC_VIDEO_ENCMODETYPE {
    OMXR_MC_VIDEO_EncModeProgressive = 0,
    OMXR_MC_VIDEO_EncModeInterlace   = 1,
    OMXR_MC_VIDEO_EncModePicaff      = 2,
    OMXR_MC_VIDEO_EncModeEnd         = 0x7FFFFFFF
} OMXR_MC_VIDEO_ENCMODETYPE;

/****************************/
/* Extended TimeStamp Mode. */
/****************************/
typedef enum OMXR_MC_VIDEO_TIMESTAMPMODETYPE {
    OMXR_MC_VIDEO_TimeStampModeDisplayOrder = 0,
    OMXR_MC_VIDEO_TimeStampModeDecodeOrder  = 1,
    OMXR_MC_VIDEO_TimeStampModeEnd          = 0x7FFFFFFF
} OMXR_MC_VIDEO_TIMESTAMPMODETYPE;

/*********************************/
/* Log Message Level             */
/*********************************/
#define OMXR_LOG_LEVEL_NONE     0x0
#define OMXR_LOG_LEVEL_ERROR    0x1
#define OMXR_LOG_LEVEL_HIGH     0x2
#define OMXR_LOG_LEVEL_LOW      0x4
#define OMXR_LOG_LEVEL_DEBUG    0x8
#define OMXR_LOG_LEVEL_COMM     0x10

#define OMXR_LOG_LEVEL_ALL      0xFFFFFFFF

/*********************************/
/* Operation Mode                */
/*********************************/
#define OMXR_CHECK_THROUGHPUT   0x1


/***************************************************************************/
/*    Type  Definitions                                                    */
/***************************************************************************/
/***************************/
/* Extended PCM Mode Type. */
/***************************/
typedef struct tagOMXR_MC_AUDIO_CONFIG_PCMMODETYPE {
    OMX_U32                       nSize;
    OMX_VERSIONTYPE               nVersion;
    OMX_U32                       nPortIndex;
    OMX_U32                       nChannels;
    OMXR_MC_AUDIO_CHANNELMODETYPE eChannelMode;
    OMXR_MC_AUDIO_DUALMONOTYPE    eDualMonoMode;
    OMX_S32                       nPlaySpeed;
    OMX_U32                       nBitPerSample;
    OMX_U32                       nSamplingRate;
    OMX_ENDIANTYPE                eEndian;
} OMXR_MC_AUDIO_CONFIG_PCMMODETYPE;

/* for_VC1D_start */
/**************************/
/* Extended Vc1 Type.     */
/**************************/
typedef struct tagOMX_VIDEO_PARAM_VC1TYPE {
    OMX_U32                       nSize;
    OMX_VERSIONTYPE               nVersion;
    OMX_U32                       nPortIndex;
    OMX_VIDEO_VC1PROFILETYPE      eProfile;
    OMX_VIDEO_VC1LEVELTYPE        eLevel;
} OMX_VIDEO_PARAM_VC1TYPE;
/* for_VC1D_end */
/* for_DIVXD_start */
/**************************/
/* Extended DivX Type.     */
/**************************/
typedef struct OMXR_MC_VIDEO_PARAM_DIVXTYPE {
	OMX_U32							nSize;
	OMX_VERSIONTYPE					nVersion;
	OMX_U32							nPortIndex;
	OMXR_MC_VIDEO_DIVXVERSIONTYPE	eDivXVersion;
	OMXR_MC_VIDEO_DIVXPROFILETYPE	eProfile;
	OMXR_MC_VIDEO_DIVXLEVELTYPE		eLevel;
} OMXR_MC_VIDEO_PARAM_DIVXTYPE;
/* for_DIVXD_end */

/**************************/
/* Extended Reorder Type. */
/**************************/
typedef struct tagOMXR_MC_VIDEO_PARAM_REORDERTYPE {
    OMX_U32                       nSize;
    OMX_VERSIONTYPE               nVersion;
    OMX_U32                       nPortIndex;
    OMX_BOOL                      bReorder;
} OMXR_MC_VIDEO_PARAM_REORDERTYPE;

/****************************/
/* Extended StoreUnit Type. */
/****************************/
typedef struct tagOMXR_MC_VIDEO_PARAM_STREAM_STORE_UNITTYPE {
    OMX_U32                       nSize;
    OMX_VERSIONTYPE               nVersion;
    OMX_U32                       nPortIndex;
    OMXR_MC_VIDEO_STOREUNITTYPE   eStoreUnit;
} OMXR_MC_VIDEO_PARAM_STREAM_STORE_UNITTYPE;

/******************************/
/* Extended MemoryAlloc Type. */
/******************************/
typedef struct tagOMXR_MC_VIDEO_PARAM_PICTURE_MEMORY_ALLOCTYPE {
    OMX_U32                       nSize;
    OMX_VERSIONTYPE               nVersion;
    OMX_U32                       nPortIndex;
    OMXR_MC_VIDEO_MEMORYALLOCTYPE eMemoryAlloc;
    OMX_BOOL                      bIPConvert;
    OMX_BOOL                      bTLConvert;
} OMXR_MC_VIDEO_PARAM_PICTURE_MEMORY_ALLOCTYPE;

/***************************/
/* Extended PlayMode Type. */
/***************************/
typedef struct tagOMXR_MC_VIDEO_CONFIG_PLAY_MODETYPE {
    OMX_U32                       nSize;
    OMX_VERSIONTYPE               nVersion;
    OMX_U32                       nPortIndex;
    OMXR_MC_VIDEO_PLAYMODETYPE    ePlayMode;
} OMXR_MC_VIDEO_CONFIG_PLAY_MODETYPE;

/**************************/
/* Extended EncMode Type. */
/**************************/
typedef struct tagOMXR_MC_VIDEO_PARAM_ENC_MODETYPE {
    OMX_U32                       nSize;
    OMX_VERSIONTYPE               nVersion;
    OMX_U32                       nPortIndex;
    OMXR_MC_VIDEO_ENCMODETYPE     eEncMode;
} OMXR_MC_VIDEO_PARAM_ENC_MODETYPE;

/************************************/
/* Audio Decoder Output Delay Type. */
/************************************/
typedef struct tagOMXR_MC_AUDIO_OUTPUT_DELAYTYPE {
    OMX_U32                       nSize;
    OMX_VERSIONTYPE               nVersion;
    OMX_U32                       sec;
    OMX_U32                       usec;
} OMXR_MC_AUDIO_OUTPUT_DELAYTYPE;

/******************************/
/* Video Decoder Result Type. */
/******************************/
typedef struct tagOMXR_MC_VIDEO_DECODERESULTTYPE {
    OMX_U32                       nSize;
    OMX_VERSIONTYPE               nVersion;
    OMX_PTR                       pvPhysImageAddressY;
    OMX_PTR                       pvVirtImageAddressY;
    OMX_U32                       u32ImageAddressOffsetC;
    OMX_U32                       u32ImageAddressOffsetBottomY;
    OMX_U32                       u32ImageAddressOffsetBottomC;
    OMX_U32                       u32ImageAddressOffsetCbToCr;
    OMX_U32                       u32CropTop;
    OMX_U32                       u32CropBottom;
    OMX_U32                       u32CropLeft;
    OMX_U32                       u32CropRight;
    OMX_U32                       u32DecodedImageWidth;
    OMX_U32                       u32DecodedImageHeight;
    OMXR_MC_VIDEO_OBJECTTYPE      eObjectType;
    OMX_U32                       eProfile;
    OMX_U32                       eLevel;
    OMXR_MC_VIDEO_PICSTRUCTTYPE   ePictureStruct;
    OMXR_MC_VIDEO_PICSTATUSTYPE   ePictureStatus;
    OMX_VIDEO_PICTURETYPE         ePictureType;
    OMX_U32                       u32FrameNumber;
    OMX_U32                       u32AspectRatioPresentFlag;
    OMX_U32                       u32AspectRatioInfo;
    OMX_U32                       u32AspectRatioWidth;
    OMX_U32                       u32AspectRatioHeight;
    OMX_U32                       u32VideoRange;
    OMX_BOOL                      bColourDescription;
    OMX_U32                       u32ColourPrimaries;
    OMX_U32                       u32TransferCharacteristics;
    OMX_U32                       u32MatrixCoefficients;
    OMX_PTR                       pVenderPrivate;
} OMXR_MC_VIDEO_DECODERESULTTYPE;

/* for_VC1D_start */
typedef struct tagOMXR_MC_VC1D_DECODERESULTTYPE {
	OMX_U32						nSize;
	OMX_VERSIONTYPE				nVersion;
	OMX_U32						u32MultiResolution;
} OMXR_MC_VC1D_DECODERESULTTYPE;
/* for_VC1D_end */

/* for_DDD_SPU_start */
/******************************/
/* AC3 Decoder Param Type.    */
/******************************/
typedef struct tagOMXR_MC_AUDIO_PARAM_AC3DTYPE {
    OMX_U32                       nSize;
    OMX_VERSIONTYPE               nVersion;
    OMX_U32                       nPortIndex;
    OMXR_MC_KCAPABLE_MODETYPE     eKcapableMode;
    OMXR_MC_COMPRESSION_MODETYPE  eCompressionMode;
    OMXR_MC_AUDIO_CHANNELMODETYPE eOutputMode;
    OMXR_MC_STEREO_MODETYPE       eStereoMode;
    OMX_BOOL                      bLfeMode;
    OMXR_MC_DUALMONO_MODETYPE     eDualMonoMode;
    OMX_U32                       nPcmScaleFactor;
    OMX_U32                       nDynamicRangeScaleFactorHi;
    OMX_U32                       nDynamicRangeScaleFactorLow;
} OMXR_MC_AUDIO_PARAM_AC3DTYPE;
/* for_DDD_SPU_end */
/* for_DDE_start */
/******************************/
/* AC3 Encoder Param Type.    */
/******************************/
typedef struct tagOMXR_MC_AUDIO_PARAM_AC3ETYPE {
	OMX_U32							nSize;
	OMX_VERSIONTYPE					nVersion;
	OMX_U32							nPortIndex;
	OMXR_MC_AC3_ENCODE_MODETYPE		eAc3EncodeMode;
	OMX_U32							nBitPerSample;
	OMX_U32							nBitRate;
	OMXR_MC_AUDIO_CHANNELMODETYPE	eChannelMode;
	OMXR_MC_AC3_STEREO_MODETYPE		eStereoMode;
	OMX_BOOL						bCompressionAGCMode;
	OMX_BOOL						bCompressionAGCMode2;
	OMX_BOOL						bLFEPresentMode;
	OMX_BOOL						bLFELowpassFilterMode;
} OMXR_MC_AUDIO_PARAM_AC3ETYPE;
/* for_DDE_end */

/***********************************************/
/* Extended First Interframe Ignore Mode Type. */
/***********************************************/
typedef struct tagOMXR_MC_VIDEO_PARAM_TIME_STAMP_MODETYPE {
    OMX_U32                         nSize;
    OMX_VERSIONTYPE                 nVersion;
    OMX_U32                         nPortIndex;
    OMXR_MC_VIDEO_TIMESTAMPMODETYPE eTimeStampMode;
} OMXR_MC_VIDEO_PARAM_TIME_STAMP_MODETYPE;

/*********************************/
/* Extended TimeStamp Mode Type. */
/*********************************/
typedef struct tagOMXR_MC_VIDEO_PARAM_FIRST_INTERFRAME_IGNORE_MODETYPE {
    OMX_U32             nSize;
    OMX_VERSIONTYPE     nVersion;
    OMX_U32             nPortIndex;
    OMX_BOOL            bFirstInterframeIgnoreMode;
} OMXR_MC_VIDEO_PARAM_FIRST_INTERFRAME_IGNORE_MODETYPE;

/******************************/
/* OMX Version Type.          */
/******************************/
typedef struct OMXR_MGR_VERSIONTYPE {
    OMX_U32             nSize;
    OMX_VERSIONTYPE     nManagerVersion;
    OMX_VERSIONTYPE     nConfigurationVersion;
} OMXR_MGR_VERSIONTYPE;

/****************************************************************************/
/*    Function prototypes (private)                                         */
/****************************************************************************/
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMXR_SetLogMode(OMX_IN OMX_U32 u32LogMode);
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMXR_GetConfiguration(OMX_OUT OMX_STRING cConfigName, OMX_U32* pu32Length);
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMXR_SetConfiguration(OMX_IN OMX_STRING cConfigName);
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMXR_GetVersion(OMX_OUT OMXR_MGR_VERSIONTYPE* psVersion);
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMXR_SetOperationMode(OMX_IN OMX_U32 u32OperationMode);
OMX_API OMX_ERRORTYPE OMX_APIENTRY OMXR_GetOperationMode(OMX_OUT OMX_U32 *pu32OperationMode);


/***************************************************************************/
/*    Global Variables                                                     */
/***************************************************************************/


/***************************************************************************/
/*    Local Variables                                                      */
/***************************************************************************/


/***************************************************************************/
/*    Global Routines                                                      */
/***************************************************************************/


/***************************************************************************/
/*    Local Routines                                                       */
/***************************************************************************/


/***************************************************************************/
/* End of module                                                           */
/***************************************************************************/
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMXR_EXTENSION_H */
