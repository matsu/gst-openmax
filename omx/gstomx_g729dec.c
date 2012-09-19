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

#include "gstomx_g729dec.h"
#include "gstomx.h"

GSTOMX_BOILERPLATE (GstOmxG729Dec, gst_omx_g729dec, GstOmxBaseAudioDec,
    GST_OMX_BASE_AUDIODEC_TYPE);

static void
type_base_init (gpointer g_class)
{
  GstElementClass *element_class;

  element_class = GST_ELEMENT_CLASS (g_class);

  gst_element_class_set_details_simple (element_class,
      "OpenMAX IL G.729 audio decoder",
      "Codec/Decoder/Audio",
      "Decodes audio in G.729 format with OpenMAX IL", "Felipe Contreras");

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
          gstomx_template_caps (G_TYPE_FROM_CLASS (g_class), "sink")));

  gst_element_class_add_pad_template (element_class,
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
          gstomx_template_caps (G_TYPE_FROM_CLASS (g_class), "src")));
}

static void
type_class_init (gpointer g_class, gpointer class_data)
{
}

/* should we be overriding the settings_changed_cb from parent class like this?? */
static gboolean
settings_changed_cb (GOmxCore * core)
{
  GstOmxBaseFilter *omx_base;

  omx_base = core->object;

  GST_DEBUG_OBJECT (omx_base, "settings changed");

  {
    GstCaps *new_caps;

    new_caps = gst_caps_new_simple ("audio/x-raw-int",
        "endianness", G_TYPE_INT, G_BYTE_ORDER,
        "width", G_TYPE_INT, 16,
        "depth", G_TYPE_INT, 16,
        "rate", G_TYPE_INT, 8000,
        "signed", G_TYPE_BOOLEAN, TRUE, "channels", G_TYPE_INT, 1, NULL);

    GST_INFO_OBJECT (omx_base, "caps are: %" GST_PTR_FORMAT, new_caps);
    gst_pad_set_caps (omx_base->srcpad, new_caps);
  }

  return TRUE;
}

static void
type_instance_init (GTypeInstance * instance, gpointer g_class)
{
  GstOmxBaseFilter *omx_base;

  omx_base = GST_OMX_BASE_FILTER (instance);

  omx_base->gomx->settings_changed_cb = settings_changed_cb;
}
