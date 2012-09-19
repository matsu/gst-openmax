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

#include "gstomx_amrwbenc.h"
#include "gstomx_base_filter.h"
#include "gstomx.h"

enum
{
  ARG_0,
  ARG_BITRATE,
};

#define DEFAULT_BITRATE 64000

GSTOMX_BOILERPLATE (GstOmxAmrWbEnc, gst_omx_amrwbenc, GstOmxBaseFilter,
    GST_OMX_BASE_FILTER_TYPE);

static void
type_base_init (gpointer g_class)
{
  GstElementClass *element_class;

  element_class = GST_ELEMENT_CLASS (g_class);

  gst_element_class_set_details_simple (element_class,
      "OpenMAX IL AMR-WB audio encoder",
      "Codec/Encoder/Audio",
      "Encodes audio in AMR-WB format with OpenMAX IL", "Felipe Contreras");

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
          gstomx_template_caps (G_TYPE_FROM_CLASS (g_class), "sink")));

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
          gstomx_template_caps (G_TYPE_FROM_CLASS (g_class), "src")));
}

static void
set_property (GObject * obj,
    guint prop_id, const GValue * value, GParamSpec * pspec)
{
  GstOmxAmrWbEnc *self;

  self = GST_OMX_AMRWBENC (obj);

  switch (prop_id) {
    case ARG_BITRATE:
      self->bitrate = g_value_get_uint (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, prop_id, pspec);
      break;
  }
}

static void
get_property (GObject * obj, guint prop_id, GValue * value, GParamSpec * pspec)
{
  GstOmxAmrWbEnc *self;

  self = GST_OMX_AMRWBENC (obj);

  switch (prop_id) {
    case ARG_BITRATE:
            /** @todo propagate this to OpenMAX when processing. */
      g_value_set_uint (value, self->bitrate);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, prop_id, pspec);
      break;
  }
}

static void
type_class_init (gpointer g_class, gpointer class_data)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (g_class);

  /* Properties stuff */
  {
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    g_object_class_install_property (gobject_class, ARG_BITRATE,
        g_param_spec_uint ("bitrate", "Bit-rate",
            "Encoding bit-rate",
            0, G_MAXUINT, DEFAULT_BITRATE,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  }
}

static gboolean
settings_changed_cb (GOmxCore * core)
{
  GstOmxBaseFilter *omx_base;
  guint channels;

  omx_base = core->object;

  GST_DEBUG_OBJECT (omx_base, "settings changed");

  {
    OMX_AUDIO_PARAM_AMRTYPE param;

    G_OMX_INIT_PARAM (param);

    param.nPortIndex = omx_base->out_port->port_index;
    OMX_GetParameter (omx_base->gomx->omx_handle, OMX_IndexParamAudioAmr,
        &param);

    channels = param.nChannels;
  }

  {
    GstCaps *new_caps;

    new_caps = gst_caps_new_simple ("audio/AMR-WB",
        "channels", G_TYPE_INT, channels, "rate", G_TYPE_INT, 16000, NULL);

    GST_INFO_OBJECT (omx_base, "caps are: %" GST_PTR_FORMAT, new_caps);
    gst_pad_set_caps (omx_base->srcpad, new_caps);
  }

  return TRUE;
}

static gboolean
sink_setcaps (GstPad * pad, GstCaps * caps)
{
  GstStructure *structure;
  GstOmxBaseFilter *omx_base;
  GOmxCore *gomx;
  gint rate = 0;
  gint channels = 0;

  omx_base = GST_OMX_BASE_FILTER (GST_PAD_PARENT (pad));
  gomx = (GOmxCore *) omx_base->gomx;

  GST_INFO_OBJECT (omx_base, "setcaps (sink): %" GST_PTR_FORMAT, caps);

  g_return_val_if_fail (gst_caps_get_size (caps) == 1, FALSE);

  structure = gst_caps_get_structure (caps, 0);

  gst_structure_get_int (structure, "rate", &rate);
  gst_structure_get_int (structure, "channels", &channels);

  /* Input port configuration. */
  {
    OMX_AUDIO_PARAM_PCMMODETYPE param;

    G_OMX_INIT_PARAM (param);

    param.nPortIndex = omx_base->in_port->port_index;
    OMX_GetParameter (gomx->omx_handle, OMX_IndexParamAudioPcm, &param);

    param.nSamplingRate = rate;
    param.nChannels = channels;

    OMX_SetParameter (gomx->omx_handle, OMX_IndexParamAudioPcm, &param);
  }

  return gst_pad_set_caps (pad, caps);
}

static void
type_instance_init (GTypeInstance * instance, gpointer g_class)
{
  GstOmxBaseFilter *omx_base;
  GstOmxAmrWbEnc *self;

  omx_base = GST_OMX_BASE_FILTER (instance);
  self = GST_OMX_AMRWBENC (instance);

  omx_base->gomx->settings_changed_cb = settings_changed_cb;

  gst_pad_set_setcaps_function (omx_base->sinkpad, sink_setcaps);

  self->bitrate = DEFAULT_BITRATE;
}
