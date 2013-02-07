/**
   include/vpu5/OMX_VPU5Ext.h

   This component implements H.264 / MPEG-4 AVC video decoder.
   The H.264 / MPEG-4 AVC Video decoder is implemented on the
   Renesas's VPU5HG middleware library.

   Copyright (C) 2010 IGEL Co., Ltd
   Copyright (C) 2010 Renesas Solutions Corp.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
   02110-1301 USA

*/
#ifndef OMX_VPU5EXT_H
#define OMX_VPU5EXT_H
#include <OMX_Index.h>
#include <OMX_Types.h>

#define OMX_VPU5_CommandMaxOut "OMX.RE.VPU5MaxOutputSetting"
#define OMX_VPU5_CommandMaxInst "OMX.RE.VPU5MaxInstance"
#define OMX_VPU5_SoftwareRender "OMX.RE.SoftwareRender"

typedef enum OMX_REVPU5LEVEL {
	OMX_VPU5AVCLevel1,
	OMX_VPU5AVCLevel1b,
	OMX_VPU5AVCLevel11,
	OMX_VPU5AVCLevel12,
	OMX_VPU5AVCLevel13,
	OMX_VPU5AVCLevel2,
	OMX_VPU5AVCLevel21,
	OMX_VPU5AVCLevel22,
	OMX_VPU5AVCLevel3,
	OMX_VPU5AVCLevel31,
	OMX_VPU5AVCLevel32,
	OMX_VPU5AVCLevel4,
	OMX_VPU5AVCLevel41,
	OMX_VPU5AVCLevel42,
} OMX_REVPU5LEVEL;

typedef enum {
	OMX_VPU5MpegLevel0,
	OMX_VPU5MpegLevel1,
	OMX_VPU5MpegLevel2,
	OMX_VPU5MpegLevel3,
	OMX_VPU5MpegLevel3B, /* adv. simple profile only */
	OMX_VPU5MpegLevel4, /* adv. simple profile only */
	OMX_VPU5MpegLevel4A,  /* simple profile only */
	OMX_VPU5MpegLevel5,
	OMX_VPU5MpegLevel6,
	OMX_VPU5MpegLevel_Baseline,  /* H263 only */
	OMX_VPU5MpegNLevel
} OMX_REVPU5MPEGLEVEL;


typedef struct OMX_PARAM_REVPU5MAXPARAM {
	OMX_U32 nSize;
	OMX_VERSIONTYPE nVersion;
	OMX_U32 nWidth;
	OMX_U32 nHeight;
	OMX_U32 eVPU5AVCLevel;
	OMX_U32 eVPU5MpegLevel;
} OMX_PARAM_REVPU5MAXPARAM;

typedef struct OMX_PARAM_REVPU5MAXINSTANCE {
	OMX_U32 nSize;
	OMX_VERSIONTYPE nVersion;
	OMX_U32 nInstances;
} OMX_PARAM_REVPU5MAXINSTANCE;

typedef struct OMX_PARAM_REVPU5IPMMUSTATUS {
	OMX_U32 nSize;
	OMX_VERSIONTYPE nVersion;
	OMX_BOOL bIpmmuEnable;
} OMX_PARAM_REVPU5IPMMUSTATUS;
#endif
