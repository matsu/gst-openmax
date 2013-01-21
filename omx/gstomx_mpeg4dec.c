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

static gint
gst_omx_mpeg4dec_search_32bit_single_sc (guint8 * data)
{
  guint *val;
  guint startcode;
  guint code1, code2, code3, code4;
  const guint8 sc_val[] = {
    SC_VISUAL_OBJECT_SEQUENCE,
    SC_VISUAL_OBJECT,
    SC_STILL_TEXTURE_OBJECT,
    SC_MESH_OBJECT,
    SC_FACE_OBJECT
  };
  guint i;
  gint pos = -1;

  val = (guint *) data;

  for (i = 0; i < sizeof (sc_val); i++) {
    startcode = 0x00000100 | sc_val[i];
    code1 = GUINT32_FROM_BE (startcode);
    code2 = GUINT32_FROM_BE (startcode << 24);
    code3 = GUINT32_FROM_BE (startcode << 16);
    code4 = GUINT32_FROM_BE (startcode << 8);

    if (*val == code1) {
      pos = 0;
      break;
    }
#if (G_BYTE_ORDER == G_LITTLE_ENDIAN)
    if (((*val & 0xffffff00) == 0x01000000) &&
        ((*(val + 1) & 0x000000ff) == code2)) {
      pos = 1;
      break;
    }

    if (((*val & 0xffff0000) == 0x00000000) &&
        ((*(val + 1) & 0x0000ffff) == code3)) {
      pos = 2;
      break;
    }

    if (((*val & 0xff000000) == 0x00000000) &&
        ((*(val + 1) & 0x00ffffff) == code4)) {
      pos = 3;
      break;
    }
#else
    if (((*val & 0x00ffffff) == 0x00000001) &&
        ((*(val + 1) & 0xff000000) == code2)) {
      pos = 1;
      break;
    }

    if (((*val & 0x0000ffff) == 0x00000000) &&
        ((*(val + 1) & 0xffff0000) == code3)) {
      pos = 2;
      break;
    }

    if (((*val & 0x000000ff) == 0x00000000) &&
        ((*(val + 1) & 0xffffff00) == code4)) {
      pos = 3;
      break;
    }
#endif
  }

  return pos;
}

static gint
gst_omx_mpeg4dec_search_32bit_range_sc (guint8 * data)
{
  guint *val;
  const struct range_sc range_sc_val[] = {
    {SC_VIDEO_OBJECT_MIN, SC_VIDEO_OBJECT_MAX},
    {SC_VIDEO_OBJECT_LAYER_MIN, SC_VIDEO_OBJECT_LAYER_MAX}
  };
  guint i;
  gint pos = -1;

  val = (guint *) data;

  for (i = 0; i < sizeof (range_sc_val) / sizeof (struct range_sc); i++) {
    guint8 sc_val = 0;

#if (G_BYTE_ORDER == G_LITTLE_ENDIAN)
    if ((*val & 0x00ffffff) == 0x00010000) {
      sc_val = (*val & 0xff000000) >> 24;
      pos = 0;
      goto check_sc_val;
    }

    if (((*val & 0xffffff00) == 0x01000000)) {
      sc_val = *(val + 1) & 0x000000ff;
      pos = 1;
      goto check_sc_val;
    }

    if (((*val & 0xffff0000) == 0x00000000) &&
        ((*(val + 1) & 0x000000ff) == 0x00000001)) {
      sc_val = (*(val + 1) & 0x0000ff00) >> 8;
      pos = 2;
      goto check_sc_val;
    }

    if (((*val & 0xff000000) == 0x00000000) &&
        ((*(val + 1) & 0x0000ffff) == 0x00000100)) {
      sc_val = (*(val + 1) & 0x00ff0000) >> 16;
      pos = 3;
      goto check_sc_val;
    }
#else
    if ((*val & 0xffffff00) == 0x00000100) {
      sc_val = *val & 0x000000ff;
      pos = 0;
      goto check_sc_val;
    }

    if (((*val & 0x00ffffff) == 0x00000001)) {
      sc_val = (*(val + 1) & 0xff000000) >> 24;
      pos = 1;
      goto check_sc_val;
    }

    if (((*val & 0x0000ffff) == 0x00000000) &&
        ((*(val + 1) & 0xff000000) == 0x01000000)) {
      sc_val = (*(val + 1) & 0x00ff0000) >> 16;
      pos = 2;
      goto check_sc_val;
    }

    if (((*val & 0x000000ff) == 0x00000000) &&
        ((*(val + 1) & 0xffff0000) == 0x00010000)) {
      sc_val = (*(val + 1) & 0x0000ff00) >> 8;
      pos = 3;
      goto check_sc_val;
    }
#endif

  check_sc_val:
    if (pos >= 0) {
      if ((sc_val >= range_sc_val[i].min) && (sc_val <= range_sc_val[i].max))
        break;

      sc_val = 0;
      pos = -1;
    }
  }

  return pos;
}

static gint
gst_omx_mpeg4dec_search_32bit_sc (guint8 * data)
{
  gint pos;

  pos = gst_omx_mpeg4dec_search_32bit_single_sc (data);
  if (pos < 0)
    pos = gst_omx_mpeg4dec_search_32bit_range_sc (data);

  return pos;
}

static gint
gst_omx_mpeg4dec_search_16bit_sc (guint8 * data)
{
  guint *val;
  guint startcode;
  guint code1, code2, code3, code4;

  val = (guint *) data;

  startcode = SC_3D_MESH_OBJECT;
  code1 = GUINT32_FROM_BE (startcode << 16);
  code2 = GUINT32_FROM_BE (startcode << 8);
  code3 = GUINT32_FROM_BE (startcode);
  code4 = GUINT32_FROM_BE (startcode << 24);

#if (G_BYTE_ORDER == G_LITTLE_ENDIAN)
  if ((*val & 0x0000ffff) == code1)
    return 0;

  if ((*val & 0x00ffff00) == code2)
    return 1;

  if ((*val & 0xffff0000) == code3)
    return 2;

  if (((*val & 0xff000000) == 0x00000000) &&
      ((*(val + 1) & 0x000000ff) == code4))
    return 3;
#else
  if ((*val & 0xffff0000) == code1)
    return 0;

  if ((*val & 0x00ffff00) == code2)
    return 1;

  if ((*val & 0x0000ffff) == code3)
    return 2;

  if (((*val & 0x000000ff) == 0x00000000) &&
      ((*(val + 1) & 0xff000000) == code4))
    return 3;
#endif

  return -1;
}

static GstBuffer *
gst_omx_mpeg4dec_retrieve_dci (GstOmxMpeg4Dec * omx_mpeg4dec,
    const GValue * value)
{
  GstBuffer *codec_data, *dci = NULL;
  guint8 *data;
  guint size;
  guint i;
  guint dci_offs, dci_size = 0;

  codec_data = gst_buffer_ref (gst_value_get_buffer (value));
  if (codec_data == NULL) {
    GST_ERROR_OBJECT (omx_mpeg4dec, "couldn't get codec_data from GValue");
    return NULL;
  }

  data = GST_BUFFER_DATA (codec_data);
  size = GST_BUFFER_SIZE (codec_data);
  for (i = 0; i < size - 4; i += 4) {
    gint pos;

    pos = gst_omx_mpeg4dec_search_32bit_sc (&data[i]);
    if (pos < 0)
      pos = gst_omx_mpeg4dec_search_16bit_sc (&data[i]);

    if (pos >= 0) {
      /* found */
      dci_offs = i + pos;
      dci_size = size - dci_offs;
      break;
    }
  }

  if (dci_size > 0) {
    dci = gst_buffer_create_sub (codec_data, dci_offs, dci_size);
    if (dci == NULL) {
      GST_ERROR_OBJECT (omx_mpeg4dec, "failed to create subbuffer");
      return NULL;
    }
  } else
    GST_INFO_OBJECT (omx_mpeg4dec, "DCI wasn't found in codec_data");

  gst_buffer_unref (codec_data);

  return dci;
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

    omx_mpeg4dec->codec_data =
        gst_omx_mpeg4dec_retrieve_dci (omx_mpeg4dec, value);
    if (omx_mpeg4dec->codec_data)
      GST_INFO_OBJECT (omx_mpeg4dec,
          "DCI data length=%d", GST_BUFFER_SIZE (omx_mpeg4dec->codec_data));

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
