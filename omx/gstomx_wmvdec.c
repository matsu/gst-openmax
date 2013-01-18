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

#include "gstomx_wmvdec.h"
#include "gstomx.h"

GSTOMX_BOILERPLATE (GstOmxWmvDec, gst_omx_wmvdec, GstOmxBaseVideoDec,
    GST_OMX_BASE_VIDEODEC_TYPE);

static void
type_base_init (gpointer g_class)
{
  GstElementClass *element_class;

  element_class = GST_ELEMENT_CLASS (g_class);

  gst_element_class_set_details_simple (element_class,
      "OpenMAX IL WMV video decoder",
      "Codec/Decoder/Video",
      "Decodes video in WMV format with OpenMAX IL", "Felipe Contreras");

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
          gstomx_template_caps (G_TYPE_FROM_CLASS (g_class), "sink")));

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
          gstomx_template_caps (G_TYPE_FROM_CLASS (g_class), "src")));
}

static gboolean
gst_omx_wmvdec_sink_setcaps (GstPad * pad, GstCaps * caps)
{
  GstOmxWmvDec *omx_wmvdec;
  GstOmxBaseFilter *omx_base;
  GstStructure *structure;
  const GValue *value;
  GstCaps *sink_caps;
  guint32 fourcc;
  gboolean result;

  omx_base = GST_OMX_BASE_FILTER (GST_PAD_PARENT (pad));
  omx_wmvdec = GST_OMX_WMVDEC (gst_pad_get_parent (pad));

  GST_INFO_OBJECT (omx_wmvdec, "Enter");

  /* get codec_data */
  structure = gst_caps_get_structure (caps, 0);
  if (omx_wmvdec->codec_data != NULL) {
    gst_buffer_unref (omx_wmvdec->codec_data);
    omx_wmvdec->codec_data = NULL;
  }

  sink_caps = gst_caps_copy (caps);

  if ((value = gst_structure_get_value (structure, "codec_data"))) {
    GstStructure *st;

    omx_wmvdec->codec_data = gst_buffer_ref (gst_value_get_buffer (value));
    GST_INFO_OBJECT (omx_wmvdec,
        "codec_data_length=%d", GST_BUFFER_SIZE (omx_wmvdec->codec_data));

    sink_caps = gst_caps_make_writable (sink_caps);
    st = gst_caps_get_structure (sink_caps, 0);
    /* To prevent duplication of pushing codec_data in base_filter. */
    gst_structure_remove_field (st, "codec_data");
  }

  gst_structure_get_int (structure, "width", (gint *) & omx_wmvdec->width);
  gst_structure_get_int (structure, "height", (gint *) & omx_wmvdec->height);

  GST_INFO_OBJECT (omx_wmvdec,
      "picture size = %d x %d", omx_wmvdec->width, omx_wmvdec->height);

  if (gst_structure_get_fourcc (structure, "format", &fourcc)) {
    if (fourcc == GST_MAKE_FOURCC ('W', 'V', 'C', '1')) {
      GST_INFO_OBJECT (omx_wmvdec, "stream type is Advanced Profile");
      omx_wmvdec->is_ap = TRUE;
    } else {
      GST_INFO_OBJECT (omx_wmvdec, "stream type is Simple/Main Profile");
      omx_wmvdec->is_ap = FALSE;
    }
  }

  result = omx_wmvdec->base_setcapsfunc (pad, sink_caps);

  GST_INFO_OBJECT (omx_wmvdec, "setcaps (sink): %" GST_PTR_FORMAT, sink_caps);

  gst_caps_unref (sink_caps);
  g_object_unref (omx_wmvdec);

  return result;
}

static GstFlowReturn
gst_omx_wmvdec_split_and_push_field_picture (GstPad * pad, GstCaps * caps,
    GstClockTime timestamp, GstClockTime duration)
{
  GstOmxWmvDec *omx_wmvdec;
  GstBuffer *buf;
  const guint8 *adapter_buf;
  guint field_pic_size;
  GstFlowReturn result;
  guint avail_size = 0;
  guint i;
  guint val1, val2, val3, val4;

  omx_wmvdec = GST_OMX_WMVDEC (gst_pad_get_parent (pad));

  val1 = GUINT32_FROM_BE (0x0000010c);
  val2 = GUINT32_FROM_BE (0x0c000000);
  val3 = GUINT32_FROM_BE (0x010c0000);
  val4 = GUINT32_FROM_BE (0x00010c00);

  /* Try to search field picture start code */
  while (avail_size != gst_adapter_available (omx_wmvdec->adapter)) {
    avail_size = gst_adapter_available (omx_wmvdec->adapter);
    adapter_buf = gst_adapter_peek (omx_wmvdec->adapter, avail_size);

    field_pic_size = 0;
    for (i = 1; i < avail_size - 4; i += 4) {
      guint *val;

      val = (guint32 *) & adapter_buf[i];
      if (*val == val1) {
        field_pic_size = i;
        break;
      }
#if (G_BYTE_ORDER == G_LITTLE_ENDIAN)
      if (((*val & 0xffffff00) == 0x01000000) &&
          ((*(val + 1) & 0x000000ff) == val2)) {
        field_pic_size = i + 1;
        break;
      }

      if (((*val & 0xffff0000) == 0x00000000) &&
          ((*(val + 1) & 0x0000ffff) == val3)) {
        field_pic_size = i + 2;
        break;
      }

      if (((*val & 0xff000000) == 0x00000000) &&
          ((*(val + 1) & 0x00ffffff) == val4)) {
        field_pic_size = i + 3;
        break;
      }
#else
      if (((*val & 0x00ffffff) == 0x00000001) &&
          ((*(val + 1) & 0xff000000) == val2)) {
        field_pic_size = i + 1;
        break;
      }

      if (((*val & 0x0000ffff) == 0x00000000) &&
          ((*(val + 1) & 0xffff0000) == val3)) {
        field_pic_size = i + 2;
        break;
      }

      if (((*val & 0x000000ff) == 0x00000000) &&
          ((*(val + 1) & 0xffffff00) == val4)) {
        field_pic_size = i + 3;
        break;
      }
#endif
    }

    if (field_pic_size) {
      /* A field picture start code was found.
         Take the field picture from the adapter. */
      buf = gst_adapter_take_buffer (omx_wmvdec->adapter, field_pic_size);
      gst_buffer_set_caps (buf, caps);
      GST_BUFFER_TIMESTAMP (buf) = timestamp;
      GST_BUFFER_DURATION (buf) = duration;

      result = omx_wmvdec->base_chain_func (pad, buf);
      if (result != GST_FLOW_OK) {
        GST_ERROR_OBJECT (omx_wmvdec, "failed to push field picture data");
        g_object_unref (omx_wmvdec);
        return result;
      }
    }
  }

  g_object_unref (omx_wmvdec);

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_omx_wmvdec_pad_chain (GstPad * pad, GstBuffer * buf)
{
  GstOmxBaseFilter *omx_base;
  GstOmxWmvDec *omx_wmvdec;
  guint size;
  guint8 *data;
  GstFlowReturn result;
  GstCaps *caps;
  GstClockTime timestamp, duration;

  omx_base = GST_OMX_BASE_FILTER (GST_PAD_PARENT (pad));
  omx_wmvdec = GST_OMX_WMVDEC (gst_pad_get_parent (pad));

  GST_INFO_OBJECT (omx_wmvdec, "Enter");

  caps = GST_BUFFER_CAPS (buf);
  timestamp = GST_BUFFER_TIMESTAMP (buf);
  duration = GST_BUFFER_DURATION (buf);

  if (omx_wmvdec->codec_data == NULL)
    goto no_codec_data;

  if (omx_wmvdec->is_ap) {
    buf = gst_buffer_join (omx_wmvdec->codec_data, buf);
    gst_buffer_set_caps (buf, caps);

    omx_wmvdec->codec_data = NULL;
  } else {
    GstBuffer *SeqParabuf = NULL;
    guint32 *u32ptr;
    guint8 *u8ptr;

    /* split sequence parameter set and picture parameter set and other NALU */
    /* and parse AVCDecoderConfigurationRecord */

    size = GST_BUFFER_SIZE (omx_wmvdec->codec_data);
    data = GST_BUFFER_DATA (omx_wmvdec->codec_data);

    /* create a new buffer */
    SeqParabuf = gst_buffer_new_and_alloc (SEQ_PARAM_BUF_SIZE);
    u32ptr = (guint32 *) GST_BUFFER_DATA (SeqParabuf);

    /* create sequence header */
    u32ptr[0] = 0xC5000000;
    u32ptr[1] = 0x00000004;
    u8ptr = (guint8 *) & u32ptr[2];
    u8ptr[0] = data[0];
    u8ptr[1] = data[1];
    u8ptr[2] = data[2];
    u8ptr[3] = data[3];
    u32ptr[3] = omx_wmvdec->height;
    u32ptr[4] = omx_wmvdec->width;
    u32ptr[5] = 0x0000000C;

    GST_BUFFER_FLAG_SET (SeqParabuf, GST_BUFFER_FLAG_PREROLL);

    gst_buffer_unref (omx_wmvdec->codec_data);
    omx_wmvdec->codec_data = NULL;

    result = omx_wmvdec->base_chain_func (pad, SeqParabuf);
    if (result != GST_FLOW_OK) {
      GST_ERROR_OBJECT (omx_wmvdec, "failed to push sequence header");
      g_object_unref (omx_wmvdec);
      return result;
    }
  }

no_codec_data:

  gst_adapter_push (omx_wmvdec->adapter, buf);

  /*
   * To parse field structure images, this funtion searches a start code present
   * before a field picture data and it passes the field picture into
   * base_chain_func(). The field interlaced mode is only supported in the
   * advanced profile.
   */
  if (omx_wmvdec->is_ap) {
    result = gst_omx_wmvdec_split_and_push_field_picture (pad, caps, timestamp,
        duration);
    if (result != GST_FLOW_OK) {
      GST_ERROR_OBJECT (omx_wmvdec, "failed to push field picture data");
      gst_adapter_flush (omx_wmvdec->adapter,
          gst_adapter_available (omx_wmvdec->adapter));
      g_object_unref (omx_wmvdec);
      return result;
    }
  }

  /* Take the last frame */
  buf = gst_adapter_take_buffer (omx_wmvdec->adapter,
      gst_adapter_available (omx_wmvdec->adapter));
  gst_buffer_set_caps (buf, caps);
  GST_BUFFER_TIMESTAMP (buf) = timestamp;
  GST_BUFFER_DURATION (buf) = duration;

  result = omx_wmvdec->base_chain_func (pad, buf);

  g_object_unref (omx_wmvdec);

  return result;
}

static void
finalize (GObject * obj)
{
  GstOmxWmvDec *omx_wmvdec;

  omx_wmvdec = GST_OMX_WMVDEC (obj);

  g_object_unref (omx_wmvdec->adapter);

  G_OBJECT_CLASS (parent_class)->finalize (obj);
}

static void
type_class_init (gpointer g_class, gpointer class_data)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (g_class);

  gobject_class->finalize = finalize;
}

static void
type_instance_init (GTypeInstance * instance, gpointer g_class)
{
  GstOmxBaseVideoDec *omx_base;
  GstOmxBaseFilter *omx_base_filter;
  GstOmxWmvDec *omx_wmvdec;

  omx_base = GST_OMX_BASE_VIDEODEC (instance);
  omx_base_filter = GST_OMX_BASE_FILTER (instance);
  omx_wmvdec = GST_OMX_WMVDEC (instance);

  omx_base->compression_format = OMX_VIDEO_CodingWMV;

  omx_wmvdec->base_setcapsfunc = GST_PAD_SETCAPSFUNC (omx_base_filter->sinkpad);
  gst_pad_set_setcaps_function (omx_base_filter->sinkpad,
      gst_omx_wmvdec_sink_setcaps);

  omx_wmvdec->base_chain_func = GST_PAD_CHAINFUNC (omx_base_filter->sinkpad);
  gst_pad_set_chain_function (omx_base_filter->sinkpad,
      gst_omx_wmvdec_pad_chain);

  omx_wmvdec->codec_data = NULL;
  omx_wmvdec->width = 0;
  omx_wmvdec->height = 0;
  omx_wmvdec->is_ap = FALSE;

  omx_wmvdec->adapter = gst_adapter_new ();
}
