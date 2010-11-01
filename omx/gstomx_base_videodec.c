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

GSTOMX_BOILERPLATE (GstOmxBaseVideoDec, gst_omx_base_videodec, GstOmxBaseFilter,
    GST_OMX_BASE_FILTER_TYPE);

static void
type_base_init (gpointer g_class)
{
}

static void
type_class_init (gpointer g_class, gpointer class_data)
{
}

static void
settings_changed_cb (GOmxCore * core)
{
  GstOmxBaseFilter *omx_base;
  GstOmxBaseVideoDec *self;
  guint width;
  guint height;
  guint32 format = 0;

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
    switch (param.format.video.eColorFormat) {
      case OMX_COLOR_FormatYUV420PackedPlanar:
        format = GST_MAKE_FOURCC ('I', '4', '2', '0');
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

    gst_caps_append_structure (new_caps, struc);

    GST_INFO_OBJECT (omx_base, "caps are: %" GST_PTR_FORMAT, new_caps);
    gst_pad_set_caps (omx_base->srcpad, new_caps);
  }
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
  }

  GST_INFO_OBJECT (omx_base, "end");
}

static void
type_instance_init (GTypeInstance * instance, gpointer g_class)
{
  GstOmxBaseFilter *omx_base;

  omx_base = GST_OMX_BASE_FILTER (instance);

  omx_base->omx_setup = omx_setup;

  omx_base->gomx->settings_changed_cb = settings_changed_cb;

  gst_pad_set_setcaps_function (omx_base->sinkpad, sink_setcaps);
}
