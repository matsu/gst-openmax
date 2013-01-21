/*
 * Copyright (C) 2007-2009 Nokia Corporation.
 *
 * Author: Felipe Contreras <felipe.contreras@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef GSTOMX_MPEG4DEC_H
#define GSTOMX_MPEG4DEC_H

#include <gst/gst.h>

G_BEGIN_DECLS
#define GST_OMX_MPEG4DEC(obj) (GstOmxMpeg4Dec *) (obj)
#define GST_OMX_MPEG4DEC_TYPE (gst_omx_mpeg4dec_get_type ())
typedef struct GstOmxMpeg4Dec GstOmxMpeg4Dec;
typedef struct GstOmxMpeg4DecClass GstOmxMpeg4DecClass;

#include "gstomx_base_videodec.h"

/* start code value */
#define SC_VISUAL_OBJECT_SEQUENCE       0xb0
#define SC_VISUAL_OBJECT                0xb5
#define SC_STILL_TEXTURE_OBJECT         0xbe
#define SC_MESH_OBJECT                  0xbc
#define SC_FACE_OBJECT                  0xba
#define SC_VIDEO_OBJECT_MIN             0x00
#define SC_VIDEO_OBJECT_MAX             0x1f
#define SC_VIDEO_OBJECT_LAYER_MIN       0x20
#define SC_VIDEO_OBJECT_LAYER_MAX       0x2f
#define SC_3D_MESH_OBJECT               0x20

struct GstOmxMpeg4Dec
{
  GstOmxBaseVideoDec omx_base;

  GstOmxBaseFilterCb base_omx_setup;
  GstPadSetCapsFunction base_setcapsfunc;
  GstPadChainFunction base_chain_func;
  GstBuffer *codec_data;
};

struct GstOmxMpeg4DecClass
{
  GstOmxBaseVideoDecClass parent_class;
};

struct range_sc
{
  guint8 min;
  guint8 max;
};

GType gst_omx_mpeg4dec_get_type (void);

G_END_DECLS
#endif /* GSTOMX_MPEG4DEC_H */
