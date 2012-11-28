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

#include <string.h>             /* for memset */
#include "gstomx_mpeg4dec.h"
#include "gstomx.h"
#include "OMXR_Extension.h"

GSTOMX_BOILERPLATE (GstOmxMpeg4Dec, gst_omx_mpeg4dec, GstOmxBaseVideoDec,
    GST_OMX_BASE_VIDEODEC_TYPE);

static void
type_base_init (gpointer g_class)
{
  GstElementClass *element_class;

  element_class = GST_ELEMENT_CLASS (g_class);

  gst_element_class_set_details_simple (element_class,
      "OpenMAX IL MPEG-4 video decoder",
      "Codec/Decoder/Video",
      "Decodes video in MPEG-4 format with OpenMAX IL", "Felipe Contreras");

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
          gstomx_template_caps (G_TYPE_FROM_CLASS (g_class), "sink")));

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
          gstomx_template_caps (G_TYPE_FROM_CLASS (g_class), "src")));
}

static void
omx_setup (GstOmxBaseFilter * omx_base)
{
  GstOmxMpeg4Dec *omx_mpeg4dec;
  GOmxCore *gomx;
  OMXR_MC_VIDEO_PARAM_STREAM_STORE_UNITTYPE store_type;

  omx_mpeg4dec = GST_OMX_MPEG4DEC (omx_base);
  gomx = (GOmxCore *) omx_base->gomx;

  memset (&store_type, 0, sizeof (store_type));
  store_type.nSize = sizeof (store_type);

  OMX_GetParameter (gomx->omx_handle, OMXR_MC_IndexParamVideoStreamStoreUnit,
      &store_type);
  store_type.eStoreUnit = OMXR_MC_VIDEO_StoreUnitPackedBitstream;
  OMX_SetParameter (gomx->omx_handle, OMXR_MC_IndexParamVideoStreamStoreUnit,
      &store_type);

  omx_mpeg4dec->base_omx_setup (omx_base);
}

static void
type_class_init (gpointer g_class, gpointer class_data)
{
}

static void
type_instance_init (GTypeInstance * instance, gpointer g_class)
{
  GstOmxBaseVideoDec *omx_base;
  GstOmxBaseFilter *omx_base_filter;
  GstOmxMpeg4Dec *omx_mpeg4dec;

  omx_base = GST_OMX_BASE_VIDEODEC (instance);
  omx_base_filter = GST_OMX_BASE_FILTER (instance);
  omx_mpeg4dec = GST_OMX_MPEG4DEC (instance);

  omx_base->compression_format = OMX_VIDEO_CodingMPEG4;

  omx_mpeg4dec->base_omx_setup = omx_base_filter->omx_setup;
  omx_base_filter->omx_setup = omx_setup;
}
