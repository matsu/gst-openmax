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

static gboolean
gst_omx_mpeg4dec_sink_setcaps (GstPad * pad, GstCaps * caps)
{
  GstOmxMpeg4Dec *omx_mpeg4dec;
  GstStructure *structure;
  GstCaps *sink_caps;
  const GValue *value;
  gboolean result;

  omx_mpeg4dec = GST_OMX_MPEG4DEC (gst_pad_get_parent (pad));

  GST_INFO_OBJECT (omx_mpeg4dec, "Enter");

  /* get codec_data */
  structure = gst_caps_get_structure (caps, 0);
  if (omx_mpeg4dec->codec_data != NULL) {
    gst_buffer_unref (omx_mpeg4dec->codec_data);
    omx_mpeg4dec->codec_data = NULL;
  }

  sink_caps = gst_caps_copy (caps);

  value = gst_structure_get_value (structure, "codec_data");
  if (value != NULL) {
    GstStructure *st;

    omx_mpeg4dec->codec_data = gst_buffer_ref (gst_value_get_buffer (value));
    GST_INFO_OBJECT (omx_mpeg4dec,
        "codec_data_length=%d", GST_BUFFER_SIZE (omx_mpeg4dec->codec_data));

    sink_caps = gst_caps_make_writable (sink_caps);
    st = gst_caps_get_structure (sink_caps, 0);
    /* To prevent duplication of pushing codec_data in base_filter. */
    gst_structure_remove_field (st, "codec_data");
  }

  GST_INFO_OBJECT (omx_mpeg4dec, "setcaps (sink): %" GST_PTR_FORMAT, sink_caps);

  result = omx_mpeg4dec->base_setcapsfunc (pad, sink_caps);

  gst_caps_unref (sink_caps);
  g_object_unref (omx_mpeg4dec);

  return result;
}

static GstFlowReturn
gst_omx_mpeg4dec_pad_chain (GstPad * pad, GstBuffer * buf)
{
  GstOmxBaseFilter *omx_base_filter;
  GstOmxMpeg4Dec *omx_mpeg4dec;
  GstCaps *caps;
  GstClockTime timestamp, duration;
  GstFlowReturn result;

  omx_base_filter = GST_OMX_BASE_FILTER (GST_PAD_PARENT (pad));
  omx_mpeg4dec = GST_OMX_MPEG4DEC (gst_pad_get_parent (pad));

  GST_INFO_OBJECT (omx_mpeg4dec, "Enter");

  caps = GST_BUFFER_CAPS (buf);
  timestamp = GST_BUFFER_TIMESTAMP (buf);
  duration = GST_BUFFER_DURATION (buf);

  if (omx_mpeg4dec->codec_data) {
    buf = gst_buffer_join (omx_mpeg4dec->codec_data, buf);
    gst_buffer_set_caps (buf, caps);
    GST_BUFFER_TIMESTAMP (buf) = timestamp;
    GST_BUFFER_DURATION (buf) = duration;

    omx_mpeg4dec->codec_data = NULL;
  }

  result = omx_mpeg4dec->base_chain_func (pad, buf);

  g_object_unref (omx_mpeg4dec);

  return result;
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

  omx_mpeg4dec->base_setcapsfunc =
      GST_PAD_SETCAPSFUNC (omx_base_filter->sinkpad);
  gst_pad_set_setcaps_function (omx_base_filter->sinkpad,
      gst_omx_mpeg4dec_sink_setcaps);

  omx_mpeg4dec->base_chain_func = GST_PAD_CHAINFUNC (omx_base_filter->sinkpad);
  gst_pad_set_chain_function (omx_base_filter->sinkpad,
      gst_omx_mpeg4dec_pad_chain);

  omx_mpeg4dec->codec_data = NULL;
}
