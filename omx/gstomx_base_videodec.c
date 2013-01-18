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

#include "gstomx_base_videodec.h"
#include "gstomx.h"
#include "uiomux/uiomux.h"

GSTOMX_BOILERPLATE (GstOmxBaseVideoDec, gst_omx_base_videodec, GstOmxBaseFilter,
    GST_OMX_BASE_FILTER_TYPE);

static void
type_base_init (gpointer g_class)
{
}

static GstStateChangeReturn
change_state (GstElement * element, GstStateChange transition)
{
  GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;
  GstOmxBaseFilter *self;
  GOmxCore *core;

  self = GST_OMX_BASE_FILTER (element);
  core = self->gomx;

  GST_LOG_OBJECT (self, "begin");

  GST_INFO_OBJECT (self, "changing state %s - %s",
      gst_element_state_get_name (GST_STATE_TRANSITION_CURRENT (transition)),
      gst_element_state_get_name (GST_STATE_TRANSITION_NEXT (transition)));

  switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      if (!core->postproc)
        uiomux_unregister ((void *) 0x80000000);
      break;

    default:
      break;
  }

  ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

  if (ret == GST_STATE_CHANGE_FAILURE)
    goto leave;

  switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
      if (!core->postproc)
        uiomux_register ((void *) 0x80000000, 0x80000000, 0x20000000);
      break;

    default:
      break;
  }

leave:
  GST_LOG_OBJECT (self, "end");

  return ret;
}

static void
type_class_init (gpointer g_class, gpointer class_data)
{
  GstElementClass *gstelement_class;

  gstelement_class = GST_ELEMENT_CLASS (g_class);
  gstelement_class->change_state = change_state;
}

static gboolean
query_tladdressing_supported (GstOmxBaseFilter * omx_base)
{
  GstOmxBaseVideoDec *self;
  gboolean result = FALSE;
  GstPad *peer;
  GstStructure *structure;
  GstQuery *query;

  self = GST_OMX_BASE_VIDEODEC (omx_base);
  peer = gst_pad_get_peer (omx_base->srcpad);
  structure = gst_structure_empty_new ("GstQueryTLAddressingSupported");
  gst_structure_set (structure, "tladdressing-supported", G_TYPE_BOOLEAN, FALSE,
      NULL);
  self->query_type_tladdr = gst_query_type_register ("tladdressing-supported",
      "whether dealing with T/L addressing as a capability or not");
  query = gst_query_new_application (self->query_type_tladdr, structure);
  if (gst_pad_query (peer, query)) {
    gst_structure_get_boolean (structure, "tladdressing-supported", &result);
  }

  gst_query_unref (query);
  gst_object_unref (peer);

  GST_INFO_OBJECT (self, "A downstream sink %s T/L addressing.",
      result ? "supports" : "does not support");
  return result;
}

static gboolean
settings_changed_cb (GOmxCore * core)
{
  GstOmxBaseFilter *omx_base;
  GstOmxBaseVideoDec *self;
  guint width;
  guint height;
  guint sliceheight;
  guint32 format = 0;
  gint32 stride, chroma_byte_offset;

  omx_base = core->object;
  self = GST_OMX_BASE_VIDEODEC (omx_base);

  GST_DEBUG_OBJECT (omx_base, "settings changed");

  {
    OMX_PARAM_PORTDEFINITIONTYPE param;

    G_OMX_INIT_PARAM (param);

    param.nPortIndex = omx_base->out_port->port_index;
    OMX_GetParameter (omx_base->gomx->omx_handle, OMX_IndexParamPortDefinition,
        &param);

    width = param.format.video.nFrameWidth;
    height = param.format.video.nFrameHeight;
    sliceheight = param.format.video.nSliceHeight;
    switch (param.format.video.eColorFormat) {
      case OMX_COLOR_FormatYUV420Planar:
      case OMX_COLOR_FormatYUV420PackedPlanar:
        format = GST_MAKE_FOURCC ('I', '4', '2', '0');
        break;
      case OMX_COLOR_FormatYUV420SemiPlanar:
      case OMX_COLOR_FormatYUV420PackedSemiPlanar:
        format = GST_MAKE_FOURCC ('N', 'V', '1', '2');
        break;
      case OMX_COLOR_FormatYCbYCr:
        format = GST_MAKE_FOURCC ('Y', 'U', 'Y', '2');
        break;
      case OMX_COLOR_FormatCbYCrY:
        format = GST_MAKE_FOURCC ('U', 'Y', 'V', 'Y');
        break;
      default:
        break;
    }

    stride =
        (core->postproc) ? param.format.video.nStride : param.format.video.
        nFrameWidth;
    chroma_byte_offset = stride * param.format.video.nSliceHeight;
  }

  {
    GstCaps *new_caps;
    GstStructure *struc;

    new_caps = gst_caps_new_empty ();
    struc = gst_structure_new ("video/x-raw-yuv",
        "width", G_TYPE_INT, width,
        "height", G_TYPE_INT, height, "format", GST_TYPE_FOURCC, format, NULL);

    if (self->framerate_denom != 0)
      gst_structure_set (struc, "framerate", GST_TYPE_FRACTION,
          self->framerate_num, self->framerate_denom, NULL);
    else
      /* FIXME this is a workaround for xvimagesink */
      gst_structure_set (struc, "framerate", GST_TYPE_FRACTION, 0, 1, NULL);

#define ALIGN32(_x)     (((_x) + 31) / 32 * 32)
#define ALIGN2UP(_p, _w)                    \
    {                                       \
	    (_p) = ((_w) - 1);       \
	    (_p) = (_p) | ((_p) >> 1);      \
	    (_p) = (_p) | ((_p) >> 2);      \
	    (_p) = (_p) | ((_p) >> 4);      \
	    (_p) = (_p) | ((_p) >> 8);      \
	    (_p) = (_p) | ((_p) >> 16);     \
	    (_p) += 1;                      \
    }
    if (!core->postproc) {
      if (stride <= 256)
        stride = 256;
      else
        ALIGN2UP (stride, stride);
      chroma_byte_offset = stride * ALIGN32 (sliceheight);
#define OMXR_TILE_WIDTH	  32
#define OMXR_TILE_HEIGHT  8
      gst_structure_set (struc, "tile-width", G_TYPE_INT, OMXR_TILE_WIDTH,
          NULL);
      gst_structure_set (struc, "tile-height", G_TYPE_INT, OMXR_TILE_HEIGHT,
          NULL);
      if (omx_base->ready_cb && !omx_base->ready_cb (omx_base))
        return FALSE;

      gst_structure_set (struc, "next_field_offset", G_TYPE_INT,
          chroma_byte_offset / 2, NULL);
    }
    gst_structure_set (struc, "rowstride", G_TYPE_INT, stride, NULL);
    gst_structure_set (struc, "chroma_byte_offset", G_TYPE_INT,
        chroma_byte_offset, NULL);

    gst_caps_append_structure (new_caps, struc);

    GST_INFO_OBJECT (omx_base, "caps are: %" GST_PTR_FORMAT, new_caps);
    gst_pad_set_caps (omx_base->srcpad, new_caps);

    gst_caps_unref (new_caps);
  }

  return TRUE;
}

static gboolean
sink_setcaps (GstPad * pad, GstCaps * caps)
{
  GstStructure *structure;
  GstOmxBaseVideoDec *self;
  GstOmxBaseFilter *omx_base;
  GOmxCore *gomx;
  OMX_PARAM_PORTDEFINITIONTYPE param;
  gint width = 0;
  gint height = 0;

  self = GST_OMX_BASE_VIDEODEC (GST_PAD_PARENT (pad));
  omx_base = GST_OMX_BASE_FILTER (self);

  gomx = (GOmxCore *) omx_base->gomx;

  GST_INFO_OBJECT (self, "setcaps (sink): %" GST_PTR_FORMAT, caps);

  g_return_val_if_fail (gst_caps_get_size (caps) == 1, FALSE);

  structure = gst_caps_get_structure (caps, 0);

  gst_structure_get_int (structure, "width", &width);
  gst_structure_get_int (structure, "height", &height);

  {
    const GValue *framerate = NULL;
    framerate = gst_structure_get_value (structure, "framerate");
    if (framerate) {
      self->framerate_num = gst_value_get_fraction_numerator (framerate);
      self->framerate_denom = gst_value_get_fraction_denominator (framerate);
    }
  }

  G_OMX_INIT_PARAM (param);

  {
    const GValue *codec_data;
    GstBuffer *buffer;

    codec_data = gst_structure_get_value (structure, "codec_data");
    if (codec_data) {
      buffer = gst_value_get_buffer (codec_data);
      omx_base->codec_data = buffer;
      gst_buffer_ref (buffer);
    }
  }

  /* Input port configuration. */
  {
    param.nPortIndex = omx_base->in_port->port_index;
    OMX_GetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, &param);

    param.format.video.nFrameWidth = width;
    param.format.video.nFrameHeight = height;

    OMX_SetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, &param);
  }

  return gst_pad_set_caps (pad, caps);
}

static gboolean
ready_cb (GstOmxBaseFilter * omx_base)
{
  gboolean ret = TRUE;
  GOmxCore *gomx;
  gomx = (GOmxCore *) omx_base->gomx;

  if (!gomx->postproc)
    ret = query_tladdressing_supported (omx_base);

  return ret;
}

static void
omx_setup (GstOmxBaseFilter * omx_base)
{
  GstOmxBaseVideoDec *self;
  GOmxCore *gomx;

  self = GST_OMX_BASE_VIDEODEC (omx_base);
  gomx = (GOmxCore *) omx_base->gomx;

  GST_INFO_OBJECT (omx_base, "begin");

  {
    OMX_PARAM_PORTDEFINITIONTYPE param;

    G_OMX_INIT_PARAM (param);

    /* Input port configuration. */
    {
      param.nPortIndex = omx_base->in_port->port_index;
      OMX_GetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, &param);

      param.format.video.eCompressionFormat = self->compression_format;

      OMX_SetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, &param);
    }

    memset (&param, 0, sizeof (param));
    G_OMX_INIT_PARAM (param);

    /* Output port configuration */
    {
      param.nSize = sizeof (OMX_PARAM_PORTDEFINITIONTYPE);

      param.nPortIndex = 1;
      OMX_GetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, &param);

      param.format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;

      OMX_SetParameter (gomx->omx_handle, OMX_IndexParamPortDefinition, &param);
    }
  }

  GST_INFO_OBJECT (omx_base, "end");
}

static void
type_instance_init (GTypeInstance * instance, gpointer g_class)
{
  GstOmxBaseFilter *omx_base;

  omx_base = GST_OMX_BASE_FILTER (instance);

  omx_base->omx_setup = omx_setup;
  omx_base->ready_cb = ready_cb;

  omx_base->gomx->settings_changed_cb = settings_changed_cb;

  gst_pad_set_setcaps_function (omx_base->sinkpad, sink_setcaps);
}
